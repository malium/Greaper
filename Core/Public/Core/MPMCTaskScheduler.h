/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_MPMC_TASK_SCHEDULER_H
#define CORE_MPMC_TASK_SCHEDULER_H 1

#include "CorePrerequisites.h"
#include "Concurrency.h"
#include "IThreadManager.h"
#include "Base/IThread.h"
#include "Enumeration.h"

ENUMERATION(TaskState, Inactive, InProgress, Completed);

namespace greaper
{
	namespace Impl
	{
		class Task
		{
		public:
			Task(StringView name, std::function<void()> workFn)noexcept;
			Task(String name, std::function<void()> workFn)noexcept;

			TaskState_t GetCurrentState()const noexcept;

			friend MPMCTaskScheduler;

		private:
			String m_Name;
			std::function<void()> m_WorkFn;
			TaskState_t m_State;
		};

		class HTask
		{
			WPtr<Task> m_Task;
			WPtr<MPMCTaskScheduler> m_Scheduler;

			friend MPMCTaskScheduler;

		public:
			void WaitUntilFinish()noexcept;
		};
	}

	class MPMCTaskScheduler
	{
	public:
		static SPtr<MPMCTaskScheduler> Create(sizet workerCount)noexcept;

		~MPMCTaskScheduler()noexcept;

		MPMCTaskScheduler(const MPMCTaskScheduler&) = delete;
		MPMCTaskScheduler& operator=(const MPMCTaskScheduler&) = delete;

		sizet GetWorkerCount()const noexcept;
		void SetWorkerCount(sizet count)noexcept;

		Impl::HTask AddTask(StringView name, std::function<void()> workFn)noexcept;
		Impl::HTask AddTask(String name, std::function<void()> workFn)noexcept;

		Vector<Impl::HTask> AddTasks(const Vector<std::tuple<StringView, std::function<void()>>>& tasks)noexcept;
		Vector<Impl::HTask> AddTasks(const Vector<std::tuple<String, std::function<void()>>>& tasks)noexcept;

		void WaitUntilTaskIsFinish(const Impl::HTask& task)noexcept;
		void WaitUntilFinishAllTasks()noexcept;

	private:
		Vector<PThread> m_TaskWorkers;
		mutable Mutex m_TaskWorkersMutex;

		Vector<SPtr<Impl::Task>> m_TaskQueue;
		mutable Mutex m_QueueMutex;
		Signal m_QueueSignal;

		Vector<Impl::Task*> m_FreeTaskPool;
		mutable Mutex m_FreeTaskPoolMutex;

		SPtr<MPMCTaskScheduler> m_This;
		
		explicit MPMCTaskScheduler(sizet workerCount = 0)noexcept;

		static void WorkerFn(MPMCTaskScheduler& scheduler, PThread thread)noexcept;
	};
}

#include "Base/MPMCTaskScheduler.inl"

#endif /* CORE_MPMC_TASK_SCHEDULER_H */