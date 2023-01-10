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
#include "VideoMode.h"

namespace greaper::disp
{
	class Monitor
	{
		math::Vector2i m_Size;
		RectI m_WorkRect;
		int32 m_Index;
		bool m_IsPrimary;
		Vector<SPtr<VideoMode>> m_VideoModes;
		sizet m_MainVideoMode;

	public:
		Monitor()noexcept = default;
		Monitor(math::Vector2i size, RectI workRect, int32 index, Vector<SPtr<VideoMode>> videoModes, bool isPrimary, sizet mainVideoMode) noexcept;
		Monitor(const Monitor&) = default;
		Monitor(Monitor&&)noexcept = default;
		Monitor& operator=(const Monitor&) = default;
		Monitor& operator=(Monitor&&)noexcept = default;

		INLINE const math::Vector2i& GetSize()const noexcept { return m_Size; }

		INLINE const RectI& GetWorkRect()const noexcept { return m_WorkRect; }

		INLINE int32 GetIndex()const noexcept { return m_Index; }

		INLINE bool IsPrimary()const noexcept { return m_IsPrimary; }

		INLINE const Vector<SPtr<VideoMode>>& GetVideoModes()const noexcept { return m_VideoModes; }

		INLINE SPtr<VideoMode> GetMainVideoMode()const noexcept { if(m_VideoModes.size() > m_MainVideoMode) return m_VideoModes[m_MainVideoMode]; return SPtr<VideoMode>(); }
	};

	INLINE Monitor::Monitor(math::Vector2i size, RectI workRect, int32 index, Vector<SPtr<VideoMode>> videoModes, bool isPrimary, sizet mainVideoMode) noexcept
		:m_Size(std::move(size))
		,m_WorkRect(std::move(workRect))
		,m_Index(index)
		,m_IsPrimary(isPrimary)
		,m_VideoModes(std::move(videoModes))
		,m_MainVideoMode(mainVideoMode)
	{

	}
}

#endif /* CORE_MONITOR_H */