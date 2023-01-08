/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_DISPLAY_ADAPTER_H
#define DISP_DISPLAY_ADAPTER_H 1

#include "../DispPrerequisites.h"
#include "VideoMode.h"
#include "Monitor.h"
#include <Core/Result.h>

namespace greaper::disp
{
	class DisplayAdapter
	{
		Vector<Monitor> m_Monitors;
		Vector<VideoMode> m_VideoModes;

		String m_AdapterName;
		String m_AdapterString;
		String m_AdapterID;
		String m_AdapterKey;
		bool m_IsDefault;

	public:
		DisplayAdapter()noexcept = default;
		DisplayAdapter(Vector<Monitor> monitors, Vector<VideoMode> videoModes, String adapterName,
			String adapterString, String adapterID, String adapterKey, bool isDefault) noexcept;

		DisplayAdapter(const DisplayAdapter&) = default;
		DisplayAdapter(DisplayAdapter&&)noexcept = default;
		DisplayAdapter& operator=(const DisplayAdapter&) = default;
		DisplayAdapter& operator=(DisplayAdapter&&)noexcept = default;

		INLINE const Vector<Monitor>& GetMonitors()const noexcept { return m_Monitors; }

		INLINE const Vector<VideoMode>& GetVideoModes()const noexcept { return m_VideoModes; }

		INLINE const String& GetAdapterName()const noexcept { return m_AdapterName; }

		INLINE const String& GetAdapterString()const noexcept { return m_AdapterString; }

		INLINE const String& GetAdapterID()const noexcept { return m_AdapterID; }

		INLINE const String& GetAdapterKey()const noexcept { return m_AdapterKey; }

		INLINE bool IsDefault()const noexcept { return m_IsDefault; }
		
		TResult<Monitor> GetMonitorWithSize(const math::Vector2u& size)const noexcept;

		TResult<Monitor> GetPrimaryMonitor()const noexcept;

		TResult<VideoMode> GetDefaultVideoMode()const noexcept;

		INLINE bool HasAnyMonitorConnected()const noexcept { return !m_Monitors.empty(); }
	};

	INLINE DisplayAdapter::DisplayAdapter(Vector<Monitor> monitors, Vector<VideoMode> videoModes, String adapterName,
				String adapterString, String adapterID, String adapterKey, bool isDefault) noexcept
		:m_Monitors(std::move(monitors))
		,m_VideoModes(std::move(videoModes))
		,m_AdapterName(std::move(adapterName))
		,m_AdapterString(std::move(adapterString))
		,m_AdapterID(std::move(adapterID))
		,m_AdapterKey(std::move(adapterKey))
		,m_IsDefault(isDefault)
	{

	}

	INLINE TResult<Monitor> DisplayAdapter::GetMonitorWithSize(const math::Vector2u& size) const noexcept
	{
		for(const Monitor& mon : m_Monitors)
		{
			if(mon.GetSize() == size)
				return Result::CreateSuccess(mon);
		}
		return Result::CreateFailure<Monitor>(Format("Couldn't find a monitor with size (%" PRIu32 ", %" PRIu32 ").", size.X, size.Y));
	}
	
	INLINE TResult<Monitor> DisplayAdapter::GetPrimaryMonitor() const noexcept
	{
		for(const Monitor& mon : m_Monitors)
		{
			if(mon.IsPrimary())
				return Result::CreateSuccess(mon);
		}
		return Result::CreateFailure<Monitor>("Couldn't find a primary monitor."sv);
	}

	INLINE TResult<VideoMode> DisplayAdapter::GetDefaultVideoMode() const noexcept
	{
		const auto pmRes = GetPrimaryMonitor();
		if(pmRes.HasFailed())
			return Result::CopyFailure<VideoMode>(pmRes);
		const auto monitor = pmRes.GetValue();
		auto found = VideoMode{math::Vector2u::ZERO, nullptr, 0, 0};
		for(const VideoMode& mode : m_VideoModes)
		{
			if(mode.GetResolution() != monitor.GetSize())
				continue;
			if(found.GetFrequency() <= mode.GetFrequency() && found.GetPixelDepth() <= mode.GetPixelDepth())
			{
				found = mode;
			}
		}
		if(found.GetAdapter() == nullptr)
			return Result::CreateFailure<VideoMode>("Coudn't find the default video mode."sv);
		return Result::CreateSuccess(found);
	}
}

#endif /* DISP_DISPLAY_ADAPTER_H */