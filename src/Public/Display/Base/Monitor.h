/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_MONITOR_H
#define CORE_MONITOR_H 1

#include "../DispPrerequisites.h"
#include <Core/Base/Rect.h>
#include <Math/Vector2.h>

namespace greaper::disp
{
	class DisplayAdapter;
#if PLT_WINDOWS
	using MonitorHandle = HMONITOR;
#else
	using MonitorHandle = void*;
#endif

	class Monitor
	{
		math::Vector2u m_Size;
		RectU m_WorkRect;
		MonitorHandle m_Handle;
		DisplayAdapter* m_Adapter;
		String m_MonitorName;
		String m_MonitorString;
		String m_MonitorID;
		String m_MonitorKey;
		bool m_IsPrimary;

	public:
		Monitor()noexcept = default;
		Monitor(math::Vector2u  size, const RectU& workRect, MonitorHandle handle,
			DisplayAdapter* adapter, String monitorName, String monitorString, String monitorID, 
			String monitorKey, bool isPrimary) noexcept;
		Monitor(const Monitor&) = default;
		Monitor(Monitor&&)noexcept = default;
		Monitor& operator=(const Monitor&) = default;
		Monitor& operator=(Monitor&&)noexcept = default;

		INLINE const math::Vector2u& GetSize()const noexcept { return m_Size; }

		INLINE const RectU& GetWorkRect()const noexcept { return m_WorkRect; }

		INLINE MonitorHandle GetHandle()const noexcept { return m_Handle; }

		INLINE DisplayAdapter* GetAdapter()const noexcept { return m_Adapter; }

		INLINE const String& GetMonitorName()const noexcept { return m_MonitorName; }

		INLINE const String& GetMonitorString()const noexcept { return m_MonitorString; }

		INLINE const String& GetMonitorID()const noexcept { return m_MonitorID; }

		INLINE const String& GetMonitorKey()const noexcept { return m_MonitorKey; }

		INLINE bool IsPrimary()const noexcept { return m_IsPrimary; }
	};

	INLINE Monitor::Monitor(math::Vector2u size, const RectU& workRect, MonitorHandle handle,
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