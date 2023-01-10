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
		RectI m_SizeRect;
		RectI m_WorkRect;
		int32 m_Index;
		String m_Name;
		Vector<PVideoMode> m_VideoModes;
		sizet m_MainVideoMode;
		float m_DiagonalDPI;
		float m_HorizontalDPI;
		float m_VerticalDPI;

	public:
		Monitor()noexcept = default;
		Monitor(RectI sizeRect, RectI workRect, int32 index, String name, Vector<PVideoMode> videoModes, sizet mainVideoMode, float ddpi, float hdpi, float vdpi)noexcept;
		Monitor(const Monitor&) = default;
		Monitor(Monitor&&)noexcept = default;
		Monitor& operator=(const Monitor&) = default;
		Monitor& operator=(Monitor&&)noexcept = default;

		INLINE const RectI& GetSizeRect()const noexcept { return m_SizeRect; }

		INLINE const RectI& GetWorkRect()const noexcept { return m_WorkRect; }

		INLINE const String& GetName()const noexcept { return m_Name; }

		INLINE int32 GetIndex()const noexcept { return m_Index; }

		INLINE bool IsPrimary()const noexcept { return m_Index == 0; }

		INLINE float GetDiagonalDPI()const noexcept { return m_DiagonalDPI; }

		INLINE float GetHorizontalDPI()const noexcept { return m_HorizontalDPI; }

		INLINE float GetVerticalDPI()const noexcept { return m_VerticalDPI; }

		INLINE const Vector<PVideoMode>& GetVideoModes()const noexcept { return m_VideoModes; }

		INLINE PVideoMode GetMainVideoMode()const noexcept { if(m_VideoModes.size() > m_MainVideoMode) return m_VideoModes[m_MainVideoMode]; return SPtr<VideoMode>(); }
	};

	INLINE Monitor::Monitor(RectI sizeRect, RectI workRect, int32 index, String name, Vector<PVideoMode> videoModes, sizet mainVideoMode, float ddpi, float hdpi, float vdpi) noexcept
		:m_SizeRect(std::move(sizeRect))
		,m_WorkRect(std::move(workRect))
		,m_Index(index)
		,m_Name(std::move(name))
		,m_VideoModes(std::move(videoModes))
		,m_MainVideoMode(mainVideoMode)
		,m_DiagonalDPI(ddpi)
		,m_HorizontalDPI(hdpi)
		,m_VerticalDPI(vdpi)
	{

	}
}

#endif /* CORE_MONITOR_H */