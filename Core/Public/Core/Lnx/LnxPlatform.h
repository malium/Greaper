/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_PLATFORM_H
#define CORE_LNX_PLATFORM_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
	struct LnxOSPlatform
	{
		static void Sleep(uint32 millis) noexcept;
	};
	using OSPlatform = LnxOSPlatform;
}

#endif /* CORE_LNX_PLATFORM_H */