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
		math::Vector2i Size = math::Vector2i(1280, 720);
		AnchoredPosition_t Position = AnchoredPosition_t::Center;
		RenderBackend_t Backend = RenderBackend_t::Native;
		WindowMode_t Mode = WindowMode_t::Windowed;
		int32 MonitorIndex = 0;
		
		constexpr WindowDesc()noexcept = default;
	};

	class IWindow
	{
	protected:

	public:
		virtual ~IWindow() = default;

		virtual math::Vector2i GetWindowSize()const = 0;
		virtual EmptyResult ChangeWindowSize(math::Vector2i size) = 0;
		
		virtual math::Vector2i GetWindowPosition() = 0;
		virtual EmptyResult ChangeWindowPosition(math::Vector2i size) = 0;
		virtual EmptyResult ChangeWindowPosition(AnchoredPosition_t anchor) = 0;

		virtual const WString& GetWindowTitle()const = 0;
		virtual void SetWindowTitle(WStringView title) = 0;

		virtual WindowMode_t GetWindowMode()const = 0;
		virtual EmptyResult ChangeWindowMode(WindowMode_t mode) = 0;

		virtual void ShowWindow() = 0;
		virtual void HideWindow() = 0;
		virtual bool IsWindowShown()const = 0;

		virtual bool IsWindowFocused()const = 0;
		virtual void RequestFocus() = 0;

		virtual RenderBackend_t GetRenderBackend()const = 0;

		virtual WWindowManager GetWindowManager()const = 0;

		virtual PThread GetWindowThread()const = 0;
	};
}

#endif /* DISP_I_WINDOW_H */