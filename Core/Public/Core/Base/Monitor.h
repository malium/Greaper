/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_MONITOR_H
#define CORE_MONITOR_H 1

#include <utility>

#include "../CorePrerequisites.h"
#include "Rect.h"

namespace greaper
{
	class DisplayAdapter;
#if PLT_WINDOWS
	using MonitorHandle = HMONITOR;
#else
	using MonitorHandle = void*;
#endif

	class Monitor
	{
		std::pair<uint16, uint16> m_Size;
		RectU m_WorkRect;
		MonitorHandle m_Handle;
		DisplayAdapter* m_Adapter;
		String m_MonitorName;
		String m_MonitorString;
		String m_MonitorID;
		String m_MonitorKey;
		bool m_IsPrimary;

	public:
		Monitor(std::pair<uint16, uint16>  size, const RectU& workRect, MonitorHandle handle,
			DisplayAdapter* adapter, String monitorName, String monitorString, String monitorID, 
			String monitorKey, bool isPrimary) noexcept;

		INLINE const std::pair<uint16, uint16>& GetSize()const noexcept { return m_Size; }

		INLINE const RectU& GetWorkRect()const noexcept { return m_WorkRect; }

		INLINE MonitorHandle GetHandle()const noexcept { return m_Handle; }

		INLINE DisplayAdapter* GetAdapter()const noexcept { return m_Adapter; }

		INLINE const String& GetMonitorName()const noexcept { return m_MonitorName; }

		INLINE const String& GetMonitorString()const noexcept { return m_MonitorString; }

		INLINE const String& GetMonitorID()const noexcept { return m_MonitorID; }

		INLINE const String& GetMonitorKey()const noexcept { return m_MonitorKey; }

		INLINE bool IsPrimary()const noexcept { return m_IsPrimary; }
	};

	INLINE Monitor::Monitor(std::pair<uint16, uint16>  size, const RectU& workRect, MonitorHandle handle,
				DisplayAdapter* adapter, String monitorName, String monitorString, String monitorID, 
				String monitorKey, bool isPrimary) noexcept
		:m_Size(std::move(size))
		,m_WorkRect(workRect)
		,m_Handle(handle)
		,m_Adapter(adapter)
		,m_MonitorName(std::move(monitorName))
		,m_MonitorString(std::move(monitorString))
		,m_MonitorID(std::move(monitorID))
		,m_MonitorKey(std::move(monitorKey))
		,m_IsPrimary(isPrimary)
	{

	}
}

#endif /* CORE_MONITOR_H */