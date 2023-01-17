/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_WINDOW_MANAGER_H
#define DISP_WINDOW_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Display/IWindowManager.h>
#include <Core/IApplication.h>
#include <Core/Concurrency.h>

struct GLFWmonitor;

namespace greaper::disp
{
	class WindowManager final : public IWindowManager
	{
		mutable RWMutex m_MonitorMutex;
		Vector<PMonitor> m_Monitors;
		sizet m_MainMonitor;

		mutable RWMutex m_WindowMutex;
		Vector<PWindow> m_Windows;

		WThreadManager m_ThreadManager;
		WThread m_MainThread;

		// ThreadManager getter and updater
		IInterface::ActivationEvt_t::HandlerType m_OnManagerActivation;
		void OnManagerActivation(bool active, IInterface* oldInterface, const PInterface& newInterface)noexcept;
		IApplication::OnInterfaceActivationEvent_t::HandlerType m_OnNewManager;
		void OnNewManager(const PInterface& newInterface)noexcept;

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

		TResult<PWindow> CreateWindow(const WindowDesc& desc)override;

		void PollEvents()override;

		PMonitor GetMainMonitor() const override;
		
		void AccessMonitors(const std::function<void(CSpan<PMonitor>)>& accessFn) const override;

		void AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const override;
	};
}

#endif /* DISP_WINDOW_MANAGER_H */