/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_DISPLAY_ADAPTER_H
#define CORE_DISPLAY_ADAPTER_H 1

#include "../CorePrerequisites.h"
#include "VideoMode.h"
#include "Monitor.h"
#include "../Result.h"

namespace greaper
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
		DisplayAdapter(Vector<Monitor> monitors, Vector<VideoMode> videoModes, String adapterName,
			String adapterString, String adapterID, String adapterKey, bool isDefault) noexcept;

		INLINE const Vector<Monitor>& GetMonitors()const noexcept { return m_Monitors; }

		INLINE const Vector<VideoMode>& GetVideoModes()const noexcept { return m_VideoModes; }

		INLINE const String& GetAdapterName()const noexcept { return m_AdapterName; }

		INLINE const String& GetAdapterString()const noexcept { return m_AdapterString; }

		INLINE const String& GetAdapterID()const noexcept { return m_AdapterID; }

		INLINE const String& GetAdapterKey()const noexcept { return m_AdapterKey; }

		INLINE bool IsDefault()const noexcept { return m_IsDefault; }
		
		Result<Monitor> GetMonitorWithSize(const std::pair<uint16, uint16>& size)const noexcept;

		Result<Monitor> GetPrimaryMonitor()const noexcept;

		Result<VideoMode> GetDefaultVideoMode()const noexcept;

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

	INLINE Result<Monitor> DisplayAdapter::GetMonitorWithSize(const std::pair<uint16, uint16>& size) const noexcept
	{
		for(const Monitor& mon : m_Monitors)
		{
			if(mon.GetSize() == size)
				return CreateResult(mon);
		}
		return CreateFailure<Monitor>(Format("Couldn't find a monitor with size (%d, %d).", size.first, size.second));
	}
	
	INLINE Result<Monitor> DisplayAdapter::GetPrimaryMonitor() const noexcept
	{
		for(const Monitor& mon : m_Monitors)
		{
			if(mon.IsPrimary())
				return CreateResult(mon);
		}
		return CreateFailure<Monitor>("Couldn't find a primary monitor."sv);
	}

	INLINE Result<VideoMode> DisplayAdapter::GetDefaultVideoMode() const noexcept
	{
		const auto pmRes = GetPrimaryMonitor();
		if(pmRes.HasFailed())
			return CopyFailure<VideoMode>(pmRes);
		const auto monitor = pmRes.GetValue();
		auto found = VideoMode{std::make_pair<uint16, uint16>(0, 0), nullptr, 0, 0};
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
			return CreateFailure<VideoMode>("Coudn't find the default video mode.");
		return CreateResult(found);
	}
}

#endif /* CORE_DISPLAY_ADAPTER_H */