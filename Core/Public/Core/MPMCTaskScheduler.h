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
#include "Result.h"

ENUMERATION(TaskState, Inactive, InProgress, Completed);

namespace greaper
{
	namespace Impl
	{
		class Task
		{
		public:
			constexpr Task()noexcept = default;

			TaskState_t GetCurrentState()const noexcept;

			friend MPMCTaskScheduler;

		private:
			String m_Name{};
			std::function<void()> m_WorkFn = nullptr;
			TaskState_t m_State = TaskState_t::Inactive;
		};

		class HTask
		{
			WPtr<Task> m_Task;
			WPtr<MPMCTaskScheduler> m_Scheduler;

			friend MPMCTaskScheduler;

			constexpr HTask()noexcept = default;
			HTask(WPtr<Task> task, WPtr<MPMCTaskScheduler> scheduler)noexcept;

		public:

			HTask(const HTask&)noexcept = default;
			HTask(HTask&&)noexcept = default;
			HTask& operator=(const HTask&)noexcept = default;
			HTask& operator=(HTask&&)noexcept = default;
			~HTask()noexcept = default;

			void WaitUntilFinish()noexcept;
		};
	}

	class MPMCTaskScheduler
	{
	public:
		template<class _Alloc_ = GenericAllocator>
		static SPtr<MPMCTaskScheduler> Create(WThreadManager threadMgr, StringView name, sizet workerCount)noexcept;

		~MPMCTaskScheduler()noexcept;

		MPMCTaskScheduler(const MPMCTaskScheduler&) = delete;
		MPMCTaskScheduler& operator=(const MPMCTaskScheduler&) = delete;

		sizet GetWorkerCount()const noexcept;
		EmptyResult SetWorkerCount(sizet count)noexcept;

		TResult<Impl::HTask> AddTask(StringView name, std::function<void()> workFn)noexcept;

		TResult<Vector<Impl::HTask>> AddTasks(const Vector<std::tuple<StringView, std::function<void()>>>& tasks)noexcept;

		void WaitUntilTaskIsFinish(const Impl::HTask& task)noexcept;
		void WaitUntilFinishAllTasks()noexcept;

		const String& GetName()const noexcept;

	private:
		WThreadManager m_ThreadManager;
		String m_Name;

		Vector<PThread> m_TaskWorkers;
		mutable Mutex m_TaskWorkersMutex;

		Vector<SPtr<Impl::Task>> m_TaskQueue;
		mutable Mutex m_TaskQueueMutex;
		Signal m_TaskQueueSignal;

		Vector<Impl::Task*> m_FreeTaskPool;
		mutable Mutex m_FreeTaskPoolMutex;

		SPtr<MPMCTaskScheduler> m_This;

		void Stop()noexcept;

		bool AreThereAnyAvailableWorker()const noexcept;
		
		MPMCTaskScheduler(WThreadManager threadMgr, StringView name, sizet workerCount)noexcept;

		static void WorkerFn(MPMCTaskScheduler& scheduler, sizet id)noexcept;
	};
}

#include "Base/MPMCTaskScheduler.inl"

#endif /* CORE_MPMC_TASK_SCHEDULER_H */