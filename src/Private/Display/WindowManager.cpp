/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "WindowManager.h"
#include <External/SDL/SDL.h>

using namespace greaper;
using namespace greaper::disp;

SPtr<WindowManager> gWindowManager = {};

static uint8 GetPixelDepthFromFormat(uint32 format)noexcept
{
	auto pixelFormat = SPtr<SDL_PixelFormat>(SDL_AllocFormat(format), SDL_FreeFormat);
	if(pixelFormat == nullptr)
		return 0;
	return pixelFormat->BitsPerPixel;
}

void WindowManager::QueryMonitors()
{
	Verify(!m_Library.expired(), "GreaperLibrary attached was expired!");
	auto lib = m_Library.lock();

	LOCK(m_MonitorMutex);
	m_Monitors.clear();
	auto displayCount = SDL_GetNumVideoDisplays();
	if(displayCount < 1)
	{
		lib->LogError(Format("Error on SDL_GetNumVideoDisplays, msg:%s.", SDL_GetError()));
		return;
	}
	if(m_Monitors.capacity() < displayCount)
		m_Monitors.reserve(displayCount);
	
	SDL_Rect bounds, usableBounds;
	float ddpi, hdpi, vdpi;
	for(decltype(displayCount) i = 0; i < displayCount; ++i)
	{
		String name{};
		auto nameRet = SDL_GetDisplayName(i);
		if(nameRet != nullptr)
			name.assign(nameRet);
		else
			lib->LogWarning(Format("Error on SDL_GetDisplayName, msg:%s.", SDL_GetError()));

		if(SDL_GetDisplayDPI(i, &ddpi, &hdpi, &vdpi) != 0)
		{
			lib->LogWarning(Format("Error on SDL_GetDisplayDPI, msg:%s.", SDL_GetError()));
			ddpi = hdpi = vdpi = 0.f;
		}
		if(SDL_GetDisplayBounds(i, &bounds) != 0)
		{
			lib->LogWarning(Format("Error on SDL_GetDisplayBounds, msg:%s.", SDL_GetError()));
			ClearMemory(bounds);
		}
		RectI sizeRect{math::Vector2i(bounds.x, bounds.y), math::Vector2i(bounds.w, bounds.h)};
		if(SDL_GetDisplayUsableBounds(i, &usableBounds) != 0)
		{
			lib->LogWarning(Format("Error on SDL_GetDisplayUsableBounds, msg:%s.", SDL_GetError()));
			ClearMemory(usableBounds);
		}
		RectI workRect{math::Vector2i(usableBounds.x, usableBounds.y), math::Vector2i(usableBounds.w, usableBounds.h)};
		
		auto displayModeCount = SDL_GetNumDisplayModes(i);
		Vector<PVideoMode> videoModes;
		videoModes.reserve(displayModeCount);

		auto monitor = PMonitor(AllocT<Monitor>());

		SDL_DisplayMode* displayMode;
		SDL_DisplayMode* currentDisplayMode;
		if(SDL_GetCurrentDisplayMode(i, currentDisplayMode) != 0)
		{
			lib->LogError(Format("Error on SDL_GetCurrentDisplayMode, msg:%s.", SDL_GetError()));
			currentDisplayMode = nullptr;
		}
		sizet mainVideoMode = 0;
		for(decltype(displayModeCount) j = 0; j < displayModeCount; ++j)
		{
			if(SDL_GetDisplayMode(i, j, displayMode) != 0)
			{
				lib->LogError(Format("Error on SDL_GetDisplayMode, msg:%s.", SDL_GetError()));
				continue;
			}
			math::Vector2i resolution { displayMode->w, displayMode->h };
			uint8 depth = GetPixelDepthFromFormat(displayMode->format);
			if(displayMode == currentDisplayMode)
				mainVideoMode = j;
			
			auto* videoMode = AllocT<VideoMode>();
			new(videoMode)VideoMode(resolution, (WMonitor)monitor, (uint16)displayMode->refresh_rate, depth);
			videoModes.push_back(PVideoMode(videoMode));
		}

		new(monitor.get())Monitor(sizeRect, workRect, i, std::move(name), std::move(videoModes), mainVideoMode, ddpi, hdpi, vdpi);
		m_Monitors.push_back(monitor);
	}
}

void WindowManager::OnInitialization() noexcept
{

}

void WindowManager::OnDeinitialization() noexcept
{

}

void WindowManager::OnActivation(const PInterface& oldDefault) noexcept
{
	const auto& prev = (const PWindowManager&)oldDefault;
	
	if(prev != nullptr)
	{
		LOCK(m_MonitorMutex);
		m_Monitors.clear();
		m_MainMonitor = 0;
		prev->AccessMonitors([this](CSpan<PMonitor> span)
		{
			const auto size = span.GetSizeFn();
			if(m_Monitors.capacity() < size)
				m_Monitors.reserve(size);
			for(const auto& elem : span)
			{
				if(elem->IsPrimary())
					m_MainMonitor = m_Monitors.size();
				m_Monitors.push_back(elem);
			}
		});
	}
	else
	{
		QueryMonitors();
	}
}

void WindowManager::OnDeactivation(const PInterface& newDefault) noexcept
{
	m_Monitors.clear();
}

void WindowManager::InitProperties() noexcept
{

}

void WindowManager::DeinitProperties() noexcept
{

}

SPtr<Monitor> WindowManager::GetMainMonitor() const
{
	LOCK(m_MonitorMutex);
	if(m_Monitors.size() > m_MainMonitor)
		return m_Monitors[m_MainMonitor];
	return PMonitor();
}

void WindowManager::AccessMonitors(const std::function<void(CSpan<PMonitor>)>& accessFn) const
{
	LOCK(m_MonitorMutex);
	accessFn(CreateSpan(m_Monitors));
}

TResult<PWindow> WindowManager::CreateWindow(const WindowDesc& desc)
{
	return Result::CreateFailure<PWindow>(""sv);
}

TResult<PWindow> WindowManager::GetWindow(const String& windowID) const
{
	return Result::CreateFailure<PWindow>(""sv);
}

void WindowManager::AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const
{

}