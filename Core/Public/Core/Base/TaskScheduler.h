/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_TASK_SCHEDULER_H
#define CORE_TASK_SCHEDULER_H 1

#include "ThreadPool.h"
#include "../Concurrency.h"
#include "IThread.h"
#include "Task.h"

namespace greaper
{
	class TaskScheduler;

	class HTask
	{
		TaskScheduler* m_Scheduler;
		Task* m_Task;

		INLINE HTask(TaskScheduler* taskScheduler, Task* task);

		friend class TaskScheduler;
	public:
		INLINE HTask();

		INLINE void WaitUntilTaskFinished();
	};

	class TaskScheduler
	{
		WThreadManager m_ThreadManager;
		Queue<Task> m_PendingTasks;
		Vector<PThread> m_TaskHandlers;
		Signal m_TaskSignal;
		RWMutex m_TaskMutex;
		uint64 m_NextTaskID;
		bool m_Shutdown;
		String m_Name;

		INLINE void RunFn();

		INLINE void WaitUntilTaskDone(Task* task);

		friend class HTask;

	public:
		INLINE explicit TaskScheduler(WThreadManager thManager, uint32 taskHandlerCount = 1, StringView schedulerName = "TaskScheduler"sv);

		INLINE ~TaskScheduler()noexcept;

		INLINE uint32 GetHandlerCount()const noexcept { return (uint32)m_TaskHandlers.size(); }

		INLINE HTask SendTask(Task task)noexcept;

		INLINE void WaitUntilAllTasksFinished()noexcept;

		INLINE const String& GetName()const noexcept { return m_Name; }
	};

	INLINE HTask::HTask(TaskScheduler* scheduler, Task* task)
		:m_Scheduler(scheduler)
		,m_Task(task)
	{

	}

	INLINE HTask::HTask()
		:m_Scheduler(nullptr)
		,m_Task(nullptr)
	{

	}

	INLINE void HTask::WaitUntilTaskFinished()
	{
		if (m_Scheduler == nullptr || m_Task == nullptr)
			return;

		m_Scheduler->WaitUntilTaskDone(m_Task);
	}

	INLINE void TaskScheduler::RunFn()
	{

	}

	INLINE void TaskScheduler::WaitUntilTaskDone(UNUSED Task* task)
	{

	}

	INLINE TaskScheduler::TaskScheduler(WThreadManager thManager, uint32 taskHandlerCount, StringView schedulerName)
		:m_ThreadManager(thManager)
		,m_NextTaskID(0)
		,m_Shutdown(false)
		,m_Name(schedulerName)
	{
		if (taskHandlerCount > 0)
			VerifyNot(m_ThreadManager.expired(), "Trying to create a multithreaded TaskScheduler, but not ThreadManager was provided.");


		m_TaskHandlers.reserve(taskHandlerCount);

		ThreadConfig cfg;
		cfg.ThreadFN = std::bind(&TaskScheduler::RunFn, this);
		auto thmgr = m_ThreadManager.lock();

		for (uint32 i = 0; i < taskHandlerCount; ++i)
		{
			const auto name = Format("%s_%d", m_Name.data(), i);
			cfg.Name = name;
			auto res = thmgr->CreateThread(cfg);
			if (res.HasFailed())
				continue;
			auto th = res.GetValue();
			m_TaskHandlers.push_back(th);
		}
	}

	INLINE TaskScheduler::~TaskScheduler() noexcept
	{
		if (m_TaskHandlers.empty())
			return;


	}

	INLINE HTask TaskScheduler::SendTask(Task task) noexcept
	{
		if (m_TaskHandlers.empty())
		{
			task();
			return {this, nullptr};
		}


		return {};
	}

	INLINE void TaskScheduler::WaitUntilAllTasksFinished() noexcept
	{

	}
}

#endif /* CORE_TASK_SCHEDULER_H */