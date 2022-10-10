#include "MPMCTaskScheduler.h"
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
	}

	INLINE sizet MPMCTaskScheduler::GetWorkerCount() const noexcept { LOCK(m_TaskWorkersMutex); return m_TaskWorkers.size(); }

	INLINE EmptyResult MPMCTaskScheduler::SetWorkerCount(sizet count) noexcept
	{
		m_TaskWorkersMutex.lock();
		if (m_TaskWorkers.size() == count)
		{
			m_TaskWorkersMutex.unlock();
			return;
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
				auto name = Format("%s_%ld", m_Name.c_str(), i);
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
	}
	INLINE TResult<Impl::HTask> MPMCTaskScheduler::AddTask(StringView name, std::function<void()> workFn) noexcept
	{
		auto wkLck = Lock(m_TaskWorkersMutex); // we keep the lock so if there's only 1 task worker and someone wants to remove it, we can still schedule this task
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
		SPtr<Impl::Task> task{ taskPtr };

		Impl::HTask hTask;
		hTask.m_Scheduler = (WPtr<MPMCTaskScheduler>)m_This;
		hTask.m_Task = (WPtr<Impl::Task>)task;

		m_TaskQueueMutex.lock();
		m_TaskQueue.push_back(task);
		m_TaskQueueMutex.unlock();
		m_TaskQueueSignal.notify_one();
		
		return Result::CreateSuccess(hTask);
	}
	INLINE TResult<Vector<Impl::HTask>> MPMCTaskScheduler::AddTasks(const Vector<std::tuple<StringView, std::function<void()>>>& tasks) noexcept
	{
		if(tasks.empty())
		{
			return Result::CreateFailure<Vector<Impl::HTask>>(
				Format("Trying to add multiple tasks, but an empty task vector was given."));
		}

		// we keep the lock so if there's only 1 task worker and someone wants to remove it, we can still schedule this task
		auto wkLck = Lock(m_TaskWorkersMutex);
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
}