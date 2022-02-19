/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_PLATFORM_H
#define CORE_WIN_PLATFORM_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
	struct WinOSPlatform
	{
		static void Sleep(uint32 millis) noexcept;
	};
	using OSPlatform = WinOSPlatform;
}

#include "WinPlatform.inl"

#endif /* CORE_WIN_PLATFORM_H */