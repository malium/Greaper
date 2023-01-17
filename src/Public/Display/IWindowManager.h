/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_I_WINDOW_MANAGER_H
#define DISP_I_WINDOW_MANAGER_H 1

#include "DispPrerequisites.h"
#include <Core/Interface.h>
#include "Base/Monitor.h"
#include "Base/IWindow.h"

namespace greaper::disp
{
	class IWindowManager : public TInterface<IWindowManager>
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0xC47AC974, 0x86A811ED, 0xA1EB0242, 0xAC120002 };
		static constexpr StringView InterfaceName = "WindowManager"sv;

		using WindowCreationEvent_t = Event<const PWindow&>;
		using WindowDestructionEvent_t = Event<const PWindow&>;
		using WindowMovedEvent_t = Event<const PWindow&, math::Vector2i, math::Vector2i>;
		using WindowResizedEvent_t = Event<const PWindow&, math::Vector2u, math::Vector2u>;
		using WindowModeChangedEvent_t = Event<const PWindow&, WindowMode_t, WindowMode_t>;
		using WindowStateChangedEvent_t = Event<const PWindow&, WindowState_t, WindowState_t>;

		virtual TResult<PWindow> CreateWindow(const WindowDesc& desc) = 0;

		virtual void PollEvents() = 0;

		virtual SPtr<Monitor> GetMainMonitor()const = 0;
		
		virtual void AccessMonitors(const std::function<void(CSpan<SPtr<Monitor>>)>& accessFn)const = 0;

		virtual void AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn)const = 0;
	};
}

#endif /* DISP_I_WINDOW_MANAGER_H */