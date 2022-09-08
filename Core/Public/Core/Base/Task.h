/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_TASK_H
#define CORE_TASK_H 1

#include "../Memory.h"
#include <functional>

namespace greaper
{
	class Task
	{
		StringView m_Name;
		std::function<void()> m_Function;
		std::function<void(const Task&, Duration_t)> m_DurationReportFn;

	public:
		
		INLINE static void DefaultTaskDurationLog(const Task& task, Duration_t duration);

		INLINE explicit Task(std::function<void()> function = nullptr, StringView name = "unnamed"sv, std::function<void(const Task&, Duration_t)> durationLogFn = &Task::DefaultTaskDurationLog);

		INLINE Duration_t operator()() noexcept;

		INLINE const StringView& GetName()const noexcept { return m_Name; }
	};

	INLINE void Task::DefaultTaskDurationLog(const Task& task, Duration_t duration)
	{
		const auto dur = static_cast<double>(duration.count());
		String msg;
		if (dur > 1e9)
		{
			msg = Format("Task '%s' took %fs.", task.GetName().data(), dur * 1e-9);
		}
		else if (dur > 1e6)
		{
			msg = Format("Task '%s' took %fms.", task.GetName().data(), dur * 1e-6);
		}
		else if (dur > 1e3)
		{
			msg = Format("Task '%s' took %fµs.", task.GetName().data(), dur * 1e-3);
		}
		else
		{
			msg = Format("Task '%s' took %fns.", task.GetName().data(), dur);
		}
		DEBUG_OUTPUT(msg.c_str());
	}

	INLINE Task::Task(std::function<void()> function, StringView name, std::function<void(const Task&, Duration_t)> durationLogFn)
		:m_Name(std::move(name))
		,m_Function(std::move(function))
		,m_DurationReportFn(durationLogFn)
	{

	}

	INLINE Duration_t Task::operator()() noexcept
		{
			VerifyNotNull(m_Function, "Trying to execute a nullptr task");
			const auto before = Clock_t::now();
			m_Function();
			const auto after = Clock_t::now();
			const auto duration = after - before;
			if (m_DurationReportFn != nullptr)
				m_DurationReportFn(*this, duration);
			return duration;
		}
}

#endif /* CORE_TASK_H */