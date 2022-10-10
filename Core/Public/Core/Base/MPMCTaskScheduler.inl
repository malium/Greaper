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
		INLINE Task::Task(StringView name, std::function<void()> workFn)noexcept
			:m_Name(name)
			,m_WorkFn(std::move(workFn))
			,m_State(TaskState_t::Inactive)
		{

		}

		INLINE Task::Task(String name, std::function<void()> workFn)noexcept
			:m_Name(std::move(name))
			,m_WorkFn(std::move(workFn))
			,m_State(TaskState_t::Inactive)
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
}