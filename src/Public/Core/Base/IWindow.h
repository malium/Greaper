/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_WINDOW_H
#define CORE_I_WINDOW_H 1

#include "../Memory.h"
#include "Rect.h"
#include "IThread.h"
#include "../Enumeration.h"
#include "../Event.h"

ENUMERATION(WindowState, Normal, Minimized, Maximized);
	
ENUMERATION(AnchoredPosition, TopLeft, Top, TopRight, Left, Center, Right, BottomLeft, Bottom, BottomRight);
	
ENUMERATION(FullScreenType, Windowed, Borderless, FullScreen);

namespace greaper
{
	struct WindowDesc
	{
		WStringView Title = L"GreaperWindow"sv;

		int16 Width = -1; // < 0 == don't care
		int16 Height = -1; // < 0 == don't care
		FullScreenType_t FullScreen = FullScreenType_t::Windowed;
		AnchoredPosition_t Position = AnchoredPosition_t::COUNT; // COUNT == don't care
		int MonitorIndex = -1; // < 0 == don't care

		constexpr WindowDesc() noexcept = default;
	};

	class IWindow
	{
	public:
		using Position_t = std::pair<int32, int32>;
		using Size_t = std::pair<uint32, uint32>;
		using WindowResizedEvt_t = Event<IWindow*, Size_t>;
		using WindowMovedEvt_t = Event<IWindow*, Position_t>;
		using WindowClosedEvt_t = Event<IWindow*>;
		using WindowFullScreenChangedEvt_t = Event<IWindow*, FullScreenType_t>;
		using WindowStateChangedEvt_t = Event<IWindow*, WindowState_t>;

		virtual ~IWindow() = default;

		virtual Position_t GetWindowPosition()const = 0;

		virtual Size_t GetWindowSize()const = 0;

		virtual RectI GetNCWindowRect()const = 0;

		virtual RectU GetWindowRect()const = 0;

		virtual WindowState_t GetWindowState()const = 0;

		virtual void SetWindowState(WindowState_t state) = 0;

		virtual void SetWindowPosition(const Position_t& position) = 0;

		virtual void SetWindowSize(const Size_t& size) = 0;

		virtual void SetTitle(const WString& title) = 0;

		virtual const WString& GetTitle()const = 0;

		virtual Position_t ScreenToWindowPos(const Position_t& pos)const = 0;

		virtual Position_t WindowPosToScreen(const Position_t& pos)const = 0;

		virtual FullScreenType_t GetFullScreenType()const = 0;

		virtual void SetFullScreenType(FullScreenType_t type) = 0;

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

#endif /* CORE_I_WINDOW_H */