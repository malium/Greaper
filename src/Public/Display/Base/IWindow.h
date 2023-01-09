/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_I_WINDOW_H
#define DISP_I_WINDOW_H 1

#include "../DispPrerequisites.h"
#include <Core/Base/Rect.h>
#include <Core/Base/IThread.h>
#include <Core/Enumeration.h>
#include <Core/Event.h>
#include <Math/Vector2.h>

ENUMERATION(RenderBackend, OpenGL, Vulkan, Native);
ENUMERATION(WindowState, Normal, Minimized, Maximized);
ENUMERATION(AnchoredPosition, TopLeft, Top, TopRight, Left, Center, Right, BottomLeft, Bottom, BottomRight);
ENUMERATION(WindowMode, Windowed, Borderless, FullScreen);

namespace greaper::disp
{
	struct WindowDesc
	{
		StringView Title = "GreaperWindow"sv;
		math::Vector2i Size = math::Vector2i(-1, -1); // Don't care
		AnchoredPosition_t Position = AnchoredPosition_t::COUNT; // Don't care
		RenderBackend_t Backend = RenderBackend_t::Native;

		
	};

	class IWindow
	{

	};
}


#if 0 // Old window class
ENUMERATION(WindowState, Normal, Minimized, Maximized);
	
ENUMERATION(AnchoredPosition, TopLeft, Top, TopRight, Left, Center, Right, BottomLeft, Bottom, BottomRight);
	
ENUMERATION(WindowMode, Windowed, Borderless, FullScreen);

namespace greaper::disp
{
	struct WindowDesc
	{
		WStringView Title = L"GreaperWindow"sv;

		int16 Width = -1; // < 0 == don't care
		int16 Height = -1; // < 0 == don't care
		WindowMode_t FullScreen = WindowMode_t::Windowed;
		AnchoredPosition_t Position = AnchoredPosition_t::COUNT; // COUNT == don't care
		int MonitorIndex = -1; // < 0 == don't care

		constexpr WindowDesc() noexcept = default;
	};

	class IWindow
	{
	public:
		using WindowResizedEvt_t = Event<IWindow*, math::Vector2u>;
		using WindowMovedEvt_t = Event<IWindow*, math::Vector2i>;
		using WindowClosedEvt_t = Event<IWindow*>;
		using WindowFullScreenChangedEvt_t = Event<IWindow*, WindowMode_t>;
		using WindowStateChangedEvt_t = Event<IWindow*, WindowState_t>;

		virtual ~IWindow() = default;

		virtual math::Vector2i GetWindowPosition()const = 0;

		virtual math::Vector2u GetWindowSize()const = 0;

		virtual RectI GetNCWindowRect()const = 0;

		virtual RectU GetWindowRect()const = 0;

		virtual WindowState_t GetWindowState()const = 0;

		virtual void SetWindowState(WindowState_t state) = 0;

		virtual void SetWindowPosition(const math::Vector2i& position) = 0;

		virtual void SetWindowSize(const math::Vector2u& size) = 0;

		virtual void SetTitle(const WString& title) = 0;

		virtual const WString& GetTitle()const = 0;

		virtual math::Vector2i ScreenToWindowPos(const math::Vector2i& pos)const = 0;

		virtual math::Vector2i WindowPosToScreen(const math::Vector2i& pos)const = 0;

		virtual WindowMode_t GetFullScreenType()const = 0;

		virtual void SetFullScreenType(WindowMode_t type) = 0;

		virtual bool IsFocused()const = 0;

		virtual void RequestWindowFocus() = 0;

		virtual void EnableCursor(bool enable) = 0;

		virtual bool IsCursorEnabled()const = 0;

		virtual ThreadID_t GetThreadID()const = 0;

		virtual void SetActive(bool active) = 0;            

		virtual bool IsActive()const = 0;

		virtual void SetHidden(bool hidden) = 0;

		virtual bool IsHidden()const = 0;


		// TODO:
		// SetWindowIcon(Image...)
		// SetCursor(CursorType, Image...)
	};
}
#endif

#endif /* DISP_I_WINDOW_H */