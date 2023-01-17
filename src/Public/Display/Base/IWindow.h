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
#include <Core/MPMCTaskScheduler.h>

ENUMERATION(RenderBackend, OpenGL, Vulkan, Native);
ENUMERATION(WindowState, Normal, Minimized, Maximized);
ENUMERATION(AnchoredPosition, TopLeft, Top, TopRight, Left, Center, Right, BottomLeft, Bottom, BottomRight);
ENUMERATION(WindowMode, Windowed, Borderless, FullScreen);

ENUMERATION(OpenGLCreationAPI, Native, EGL, OSMESA);
ENUMERATION(OpenGLProfile, Compatibility, Core);
ENUMERATION(OpenGLContextRobustness, NoRobustness, NoResetNotification, LooseContextOnReset);
ENUMERATION(OpenGLReleaseBehaviour, Flush, None);

namespace greaper::disp
{
	struct FramebufferDesc
	{
		int32 RedBits = 8;
		int32 GreenBits = 8;
		int32 BlueBits = 8;
		int32 AlphaBits = 8;
		int32 DepthBits;
		int32 StencilBits;

		int32 MSAASamples;
		bool SRGBCapable;
		bool DoubleBuffer;
	};
	struct OpenGLDesc
	{
		OpenGLCreationAPI_t CreationAPI = OpenGLCreationAPI_t::Native;
		int32 VersionMajor = -1;
		int32 VersionMinor = -1;
		OpenGLProfile_t Profile = OpenGLProfile_t::Core;
		bool ContextDebug = GREAPER_DEBUG; // GL_KHR_debug
		OpenGLContextRobustness_t ContextRobustness = OpenGLContextRobustness_t::NoRobustness;
		OpenGLReleaseBehaviour_t ContextReleaseBehaviour = OpenGLReleaseBehaviour_t::Flush; // GL_KHR_context_flush_control
		bool ContextGenerateErrors = true; // GL_KHR_no_error
	};
	struct VulkanDesc
	{

	};
	struct WindowDesc
	{
		StringView Title = "GreaperWindow"sv;
		math::Vector2i Size = math::Vector2i(1280, 720);
		AnchoredPosition_t Position = AnchoredPosition_t::Center;
		RenderBackend_t Backend = RenderBackend_t::Native;
		WindowMode_t Mode = WindowMode_t::Windowed;
		WindowState_t State = WindowState_t::Normal;
		bool AllowResizing = true;
		bool StartVisible = true;
		bool StartFocused = true;
		int32 MonitorIndex = 0;
		PTaskScheduler Scheduler = PTaskScheduler(); // Scheduler running on the thread which the window is running, if nullptr WindowManager will create one
		PWindow SharedWindow = PWindow(); // Window that its context can share information with, needs to have the same scheduler on both
		
		StringView X11ClassName = ""sv;
		StringView X11InstanceName = ""sv;
		FramebufferDesc Framebuffer = FramebufferDesc();
		OpenGLDesc OpenGL = OpenGLDesc();
		VulkanDesc Vulkan = VulkanDesc();

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

		virtual PTaskScheduler GetTaskScheduler()const = 0;
	};
}

#endif /* DISP_I_WINDOW_H */