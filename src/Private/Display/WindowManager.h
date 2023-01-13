/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_WINDOW_MANAGER_H
#define DISP_WINDOW_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Display/IWindowManager.h>
#include <Core/Concurrency.h>

struct GLFWmonitor;

namespace greaper::disp
{
	class WindowManager final : public IWindowManager
	{
		mutable Mutex m_MonitorMutex;
		Vector<PMonitor> m_Monitors;
		sizet m_MainMonitor;

		mutable Mutex m_WindowMutex;
		Vector<PWindow> m_Windows;

		friend void OnMonitorChange(GLFWmonitor* monitor, int32 event);
	public:
		WindowManager()noexcept = default;
		~WindowManager()noexcept = default;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(const PInterface& oldDefault)noexcept override;

		void OnDeactivation(const PInterface& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;
		
		void QueryMonitors();

		PMonitor GetMainMonitor() const override;
		
		void AccessMonitors(const std::function<void(CSpan<PMonitor>)>& accessFn) const override;

		TResult<PWindow> CreateWindow(const WindowDesc& desc)override;

		void AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const override;
	};
}

#endif /* DISP_WINDOW_MANAGER_H */