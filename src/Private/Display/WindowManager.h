/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_WINDOW_MANAGER_H
#define DISP_WINDOW_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Display/IWindowManager.h>

namespace greaper::disp
{
	class WindowManager final : public IWindowManager
	{
	public:
		WindowManager()noexcept = default;
		~WindowManager()noexcept = default;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(const PInterface& oldDefault)noexcept override;

		void OnDeactivation(const PInterface& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		const DisplayAdapter& GetMainDisplayAdapter() const override;
		
		void AccessDisplayAdapters(const std::function<void(CSpan<DisplayAdapter>)>& accessFn) const override;

		TResult<PWindow> CreateWindow(const WindowDesc& desc)override;

		TResult<PWindow> GetWindow(const String& windowID) const override;
		
		void AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const override;
	};
}

#endif /* DISP_WINDOW_MANAGER_H */