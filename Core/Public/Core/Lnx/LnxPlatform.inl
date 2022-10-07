﻿/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <thread>

void greaper::LnxOSPlatform::Sleep(uint32 millis) noexcept
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}