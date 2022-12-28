/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_VIDEO_MODE_H
#define DISP_VIDEO_MODE_H 1

#include "../DispPrerequisites.h"
#include <Math/Vector2.h>

namespace greaper::disp
{
	class VideoMode
	{
		math::Vector2u m_Resolution;
		DisplayAdapter* m_Adapter;
		uint16 m_Frequency;
		uint8 m_PixelDepth;

	public:
		constexpr VideoMode(math::Vector2u resolution, DisplayAdapter* adapter, uint16 frequency, uint8 pixelDepth)noexcept;

		INLINE constexpr const math::Vector2u& GetResolution()const noexcept { return m_Resolution; }

		INLINE DisplayAdapter* GetAdapter()const noexcept { return m_Adapter; }

		INLINE constexpr uint16 GetFrequency()const noexcept { return m_Frequency; }

		INLINE constexpr uint8 GetPixelDepth()const noexcept { return m_PixelDepth; }
	};

	INLINE constexpr VideoMode::VideoMode(math::Vector2u resolution, DisplayAdapter* adapter, uint16 frequency, uint8 pixelDepth) noexcept
		:m_Resolution(std::move(resolution))
		,m_Adapter(adapter)
		,m_Frequency(frequency)
		,m_PixelDepth(pixelDepth)
	{

	}
}

#endif /* DISP_VIDEO_MODE_H */