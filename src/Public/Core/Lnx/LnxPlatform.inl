/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <thread>

INLINE void greaper::LnxOSPlatform::Sleep(uint32 millis) noexcept
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

INLINE greaper::String greaper::LnxOSPlatform::GetStackTrace()
{

}

INLINE void greaper::LnxOSPlatform::PerThreadInit()
{

}

INLINE void greaper::LnxOSPlatform::PerLibraryInit()
{

}

INLINE const CPUInfo& greaper::LnxOSPlatform::GetCPUInfo() noexcept
{
	return m_CPUInfo;
}