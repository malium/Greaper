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
		Duration_t m_Duration;
		std::function<void()> m_Function;
		StringView m_Name;

	public:
		explicit Task(std::function<void()> function = nullptr, StringView name = "unnamed"sv);

		void operator()() noexcept;

		Duration_t GetTaskDuration()const noexcept { return m_Duration; }

		const StringView& GetName()const noexcept { return m_Name; }
	};

	Task::Task(std::function<void()> function, StringView name)
		:m_Function(std::move(function))
		,m_Name(std::move(name))
		,m_Duration(0)
	{

	}

	void Task::operator()() noexcept
		{
			VerifyNotNull(m_Function, "Trying to execute a nullptr task");
			const auto before = Clock_t::now();
			m_Function();
			const auto after = Clock_t::now();
			m_Duration = after - before;
			const auto dur = static_cast<double>(m_Duration.count());
			String msg;
			if (dur > 1e9)
			{
				msg = Format("Task '%s' took %fs.", m_Name.data(), dur * 1e-9);
			}
			else if (dur > 1e6)
			{
				msg = Format("Task '%s' took %fms.", m_Name.data(), dur * 1e-6);
			}
			else if (dur > 1e3)
			{
				msg = Format("Task '%s' took %fµs.", m_Name.data(), dur * 1e-3);
			}
			else
			{
				msg = Format("Task '%s' took %fns.", m_Name.data(), dur);
			}
#if PLT_WINDOWS
			OutputDebugStringA(msg.c_str());
#else
			printf(msg.c_str());
#endif
		}
}

#endif /* CORE_TASK_H */