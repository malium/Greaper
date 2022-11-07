/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
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

		static String GetStackTrace();

		static void PerThreadInit();

		static void PerLibraryInit();
	};
	using OSPlatform = LnxOSPlatform;
}

#include "LnxPlatform.inl"

#endif /* CORE_LNX_PLATFORM_H */