/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

//#include "../MPMCTaskScheduler.h"

namespace greaper
{
	namespace Impl
	{
		INLINE HTask::HTask(WPtr<Task> task, WPtr<MPMCTaskScheduler> scheduler)noexcept
			:m_Task(std::move(task))
			, m_Scheduler(std::move(scheduler))
		{

		}
		INLINE TaskState_t Task::GetCurrentState()const noexcept { return m_State; }

		INLINE void HTask::WaitUntilFinish()noexcept
		{
			if (m_Task.expired() || m_Scheduler.expired())
				return;

			auto scheduler = m_Scheduler.lock();
			scheduler->WaitUntilTaskIsFinish(*this);
		}
	}
	
	template<class _Alloc_>
	INLINE SPtr<MPMCTaskScheduler> MPMCTaskScheduler::Create(WThreadManager threadMgr, StringView name, sizet workerCount) noexcept
	{
		auto* ptr = AllocT<MPMCTaskScheduler, _Alloc_>();
		new ((void*)ptr)MPMCTaskScheduler(threadMgr, std::move(name), workerCount);
		return SPtr<MPMCTaskScheduler>((MPMCTaskScheduler*)ptr, &Impl::DefaultDeleter<MPMCTaskScheduler, _Alloc_>);
	}

	INLINE MPMCTaskScheduler::~MPMCTaskScheduler() noexcept
	{
		Stop();
		m_This.reset();
	}

	INLINE sizet MPMCTaskScheduler::GetWorkerCount() const noexcept { auto lck = SharedLock(m_TaskWorkersMutex); return m_TaskWorkers.size(); }

	inline EmptyResult MPMCTaskScheduler::SetWorkerCount(sizet count) noexcept
	{
		m_TaskWorkersMutex.lock();
		if (m_TaskWorkers.size() == count)
		{
			m_TaskWorkersMutex.unlock();
			return Result::CreateSuccess();
		}
		// Add new workers
		if (m_TaskWorkers.size() < count)
		{
			auto lck = Lock<decltype(m_TaskWorkersMutex)>(m_TaskWorkersMutex, AdoptLock{});

			if (m_ThreadManager.expired())
				return Result::CreateFailure("Trying to add more workers to a MPMCTaskScheduler, but the ThreadManager has expired."sv);
			auto thManager = m_ThreadManager.lock();
			for (sizet i = m_TaskWorkers.size(); i < count; ++i)
			{
				ThreadConfig cfg;
				auto name = Format("%s_%" PRIuPTR "", m_Name.c_str(), i);
				cfg.Name = name;
				cfg.ThreadFN = [this, i]() { WorkerFn(*this, i); };
				auto thRes = thManager->CreateThread(cfg);
				if (thRes.HasFailed())
					return Result::CopyFailure(thRes);
				m_TaskWorkers.push_back(thRes.GetValue());
			}
		}
		// Remove workers
		else
		{
			while (m_TaskWorkers.size() > count)
			{
				const auto sz = m_TaskWorkers.size();
				PThread th = m_TaskWorkers[sz - 1];
				m_TaskWorkers[sz - 1].reset();
				m_TaskWorkersMutex.unlock();
				while (th != nullptr)
				{
					if (th->TryJoin())
					{
						th.reset();
					}
					else
					{
						m_TaskQueueSignal.notify_all();
						THREAD_YIELD();
					}
				}
				m_TaskQueueMutex.lock();
				m_TaskWorkers.erase(m_TaskWorkers.begin() + (sz - 1));
			}
		}
		return Result::CreateSuccess();
	}
	inline TResult<Impl::HTask> MPMCTaskScheduler::AddTask(StringView name, std::function<void()> workFn) noexcept
	{
		auto wkLck = SharedLock(m_TaskWorkersMutex); // we keep the lock so if there's only 1 task worker and someone wants to remove it, we can still schedule this task
		if (!AreThereAnyAvailableWorker())
		{
			return Result::CreateFailure<Impl::HTask>(
				Format("Couldn't add the task '%s', no available workers.", name.data()));
		}

		Impl::Task* taskPtr;
		{
			auto fpLck = Lock(m_FreeTaskPoolMutex);
			if (m_FreeTaskPool.empty())
			{
				taskPtr = Construct<Impl::Task>();
			}
			else
			{
				taskPtr = m_FreeTaskPool.back();
				m_FreeTaskPool.pop_back();
			}
			taskPtr->m_Name.assign(name);
			taskPtr->m_State = TaskState_t::Inactive;
			taskPtr->m_WorkFn = std::move(workFn);
		}
		SPtr<Impl::Task> task{ taskPtr , &Impl::EmptyDeleter<Impl::Task> };

		Impl::HTask hTask;
		hTask.m_Scheduler = (WPtr<MPMCTaskScheduler>)m_This;
		hTask.m_Task = (WPtr<Impl::Task>)task;

		m_TaskQueueMutex.lock();
		m_TaskQueue.push_back(task);
		m_TaskQueueMutex.unlock();
		m_TaskQueueSignal.notify_one();
		
		return Result::CreateSuccess(hTask);
	}
	inline TResult<Vector<Impl::HTask>> MPMCTaskScheduler::AddTasks(const Vector<std::tuple<StringView, std::function<void()>>>& tasks) noexcept
	{
		if(tasks.empty())
		{
			return Result::CreateFailure<Vector<Impl::HTask>>(
				Format("Trying to add multiple tasks, but an empty task vector was given."));
		}

		// we keep the lock so if there's only 1 task worker and someone wants to remove it, we can still schedule this task
		auto wkLck = SharedLock(m_TaskWorkersMutex);
		if (!AreThereAnyAvailableWorker())
		{
			return Result::CreateFailure<Vector<Impl::HTask>>(
				Format("Couldn't add multiple tasks, no available workers."));
		}

		Vector<SPtr<Impl::Task>> taskPtrs;
		Vector<Impl::HTask> hTasks;
		taskPtrs.reserve(tasks.size());
		hTasks.reserve(tasks.size());
		while(taskPtrs.size() < tasks.size())
		{
			Impl::Task* taskPtr;
			auto fpLck = Lock(m_FreeTaskPoolMutex);
			if (m_FreeTaskPool.empty())
			{
				taskPtr = Construct<Impl::Task>();
			}
			else
			{
				taskPtr = m_FreeTaskPool.back();
				m_FreeTaskPool.pop_back();
			}
			auto& tuple = tasks[taskPtrs.size()];
			taskPtr->m_Name.assign(std::get<0>(tuple));
			taskPtr->m_State = TaskState_t::Inactive;
			taskPtr->m_WorkFn = std::get<1>(tuple);
			auto task = SPtr<Impl::Task>{ taskPtr };
			hTasks.push_back(Impl::HTask{ (WPtr<Impl::Task>)task, (WPtr<MPMCTaskScheduler>)m_This });
			taskPtrs.push_back(task);
		}

		m_TaskQueueMutex.lock();
		const auto oldSize = m_TaskQueue.size();
		std::move(taskPtrs.begin(), taskPtrs.end(), m_TaskQueue.begin() + oldSize);
		m_TaskQueueMutex.unlock();
		m_TaskQueueSignal.notify_one();

		return Result::CreateSuccess(hTasks);
	}

	INLINE void MPMCTaskScheduler::WaitUntilTaskIsFinish(const Impl::HTask& hTask) noexcept
	{
		if (hTask.m_Scheduler.expired() || hTask.m_Task.expired())
			return;
		if (hTask.m_Scheduler.lock() != m_This)
			return;
		auto task = hTask.m_Task.lock();

		auto lck = UniqueLock<decltype(m_TaskQueueMutex)>(m_TaskQueueMutex);
		while (Contains(m_TaskQueue, task))
			m_TaskQueueSignal.wait(lck);
	}
	INLINE void MPMCTaskScheduler::WaitUntilFinishAllTasks() noexcept
	{
		// Don't add more tasks nor change the amount of workers
		auto wkLck = Lock(m_TaskWorkersMutex);

		// Wait until no more tasks
		auto lck = UniqueLock<decltype(m_TaskQueueMutex)>(m_TaskQueueMutex);
		while (!m_TaskQueue.empty())
			m_TaskQueueSignal.wait(lck);
	}
	INLINE const String& MPMCTaskScheduler::GetName() const noexcept { return m_Name; }
	INLINE void MPMCTaskScheduler::OnNewManager(const PInterface& newInterface) noexcept
	{
		auto lck = SharedLock(m_TaskWorkersMutex);
		if (newInterface == nullptr || newInterface->GetInterfaceUUID() != IThreadManager::InterfaceUUID)
			return;

		m_ThreadManager = (WThreadManager)newInterface;
		m_OnManagerActivation.Disconnect(); // double check we are not connected
		newInterface->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldInterface, const PInterface& newInterface) { OnManagerActivation(active, oldInterface, newInterface); });
		m_OnNewManager.Disconnect();
	}
	INLINE void MPMCTaskScheduler::OnManagerActivation(bool active, IInterface* oldInterface, const PInterface& newInterface) noexcept
	{
		auto lck = SharedLock(m_TaskWorkersMutex);
		if (active)
			return;

		// Change of ThreadManager
		if (newInterface != nullptr)
		{
			const auto& newThreadMgr = (const PThreadManager&)newInterface;
			m_OnManagerActivation.Disconnect();
			newThreadMgr->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
			m_ThreadManager = (WThreadManager)newThreadMgr;
		}
		// ThreadManager deactivated, wait until a new one is active
		else
		{
			m_OnManagerActivation.Disconnect();
			const auto& libW = oldInterface->GetLibrary();
			VerifyNot(libW.expired(), "Trying to connect to InterfaceActivationEvent but GreaperLibrary was expired.");
			auto lib = libW.lock();
			auto appW = lib->GetApplication();
			VerifyNot(appW.expired(), "Trying to connect to InterfaceActivationEvent but Application was expired.");
			auto app = appW.lock();
			m_OnNewManager.Disconnect(); // double check we are not connected
			app->GetOnInterfaceActivationEvent()->Connect(m_OnNewManager, [this](const PInterface& newManager) { OnNewManager(newManager); });
		}
	}
	INLINE void MPMCTaskScheduler::Stop() noexcept
	{
		SetWorkerCount(0);
		
		for (auto& task : m_TaskQueue)
		{
			task->m_State = TaskState_t::InProgress;
			task->m_WorkFn();
			task->m_State = TaskState_t::Completed;
			Destroy(task.get());
		}
		m_TaskQueue.clear();
		for (auto* task : m_FreeTaskPool)
		{
			Destroy(task);
		}
	}
	INLINE bool MPMCTaskScheduler::AreThereAnyAvailableWorker() const noexcept
	{
		if (m_TaskWorkers.empty())
			return false; // There are no workers!

		for (auto& worker : m_TaskWorkers)
		{
			if (worker != nullptr)
				return true; // An active worker found!
		}
		return false; // There is no active worker
	}
	INLINE MPMCTaskScheduler::MPMCTaskScheduler(WThreadManager threadMgr, StringView name, sizet workerCount)noexcept
		:m_ThreadManager(std::move(threadMgr))
		,m_Name(name)
		,m_This(this, &Impl::EmptyDeleter<MPMCTaskScheduler>)
	{
		VerifyNot(m_ThreadManager.expired(), "Trying to initialize a MPMCTaskScheduler, but an expired ThreadManager was given.");
		auto mgr = m_ThreadManager.lock();
		mgr->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldInterface, const PInterface& newInterface) { OnManagerActivation(active, oldInterface, newInterface); });
		SetWorkerCount(workerCount);
	}
	INLINE bool MPMCTaskScheduler::CanWorkerContinueWorking(sizet workerID)const noexcept
	{
		auto lck = SharedLock(m_TaskWorkersMutex);
		return m_TaskWorkers.size() > workerID && m_TaskWorkers[workerID] != nullptr;
	}
	INLINE void MPMCTaskScheduler::WorkerFn(MPMCTaskScheduler& scheduler, sizet id) noexcept
	{
		while (true)
		{
			auto taskLck = UniqueLock<decltype(m_TaskQueueMutex)>(scheduler.m_TaskQueueMutex);
			bool canWork = scheduler.CanWorkerContinueWorking(id);
			
			// Wait for work or an stop request
			while (scheduler.m_TaskQueue.empty() && canWork)
			{
				scheduler.m_TaskQueueSignal.wait(taskLck);
				canWork = scheduler.CanWorkerContinueWorking(id);
			}

			// Stop working
			if (!canWork)
				break;

			// Do one task
			if (!scheduler.m_TaskQueue.empty())
			{
				auto& task = scheduler.m_TaskQueue.front();
				task->m_State = TaskState_t::InProgress;
				task->m_WorkFn();
				task->m_State = TaskState_t::Completed;
				
				{
					auto freeLck = Lock(scheduler.m_FreeTaskPoolMutex);
					scheduler.m_FreeTaskPool.push_back(task.get());
					scheduler.m_TaskQueue.pop_front();
				}
			}
		}
	}
}