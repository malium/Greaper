/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_VIDEO_MODE_H
#define CORE_VIDEO_MODE_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
	class DisplayAdapter;
	class VideoMode
	{
		std::pair<uint16, uint16> m_Resolution;
		DisplayAdapter* m_Adapter;
		uint16 m_Frequency;
		uint8 m_PixelDepth;

	public:
		constexpr VideoMode(const std::pair<uint16, uint16>& resolution, DisplayAdapter* adpater, uint16 frequency, uint8 pixelDepth)noexcept;

		INLINE constexpr const std::pair<uint16, uint16>& GetResolution()const noexcept { return m_Resolution; }

		INLINE constexpr DisplayAdapter* GetAdapter()const noexcept { return m_Adapter; }

		INLINE constexpr uint16 GetFrequency()const noexcept { return m_Frequency; }

		INLINE constexpr uint8 GetPixelDepth()const noexcept { return m_PixelDepth; }
	};

	INLINE constexpr VideoMode::VideoMode(const std::pair<uint16, uint16>& resolution, DisplayAdapter* adpater,
		uint16 frequency, uint8 pixelDepth) noexcept
		:m_Resolution(resolution)
		,m_Adapter(adpater)
		,m_Frequency(frequency)
		,m_PixelDepth(pixelDepth)
	{

	}
}

#endif /* CORE_VIDEO_MODE_H */