/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <thread>

void greaper::LnxOSPlatform::Sleep(uint32 millis) noexcept
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

greaper::String greaper::LnxOSPlatform::GetStackTrace()
{

}

void greaper::LnxOSPlatform::PerThreadInit()
{

}

void greaper::LnxOSPlatform::PerLibraryInit()
{

}