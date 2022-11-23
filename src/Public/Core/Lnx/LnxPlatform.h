/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_PLATFORM_H
#define CORE_LNX_PLATFORM_H 1

#include "../CorePrerequisites.h"
#include "../Base/CPUInfo.h"

namespace greaper
{
	class LnxOSPlatform
	{
		static void Sleep(uint32 millis) noexcept;

		static String GetStackTrace();

		static uint64 GetPhysicalRAMAmountKB()noexcept;

		static OSType_t GetOSType()noexcept { return OSType_t::Linux; }

	protected:

		static void _PerThreadInit();

		static void _PerLibraryInit();
	};
}

#include "LnxPlatform.inl"

#endif /* CORE_LNX_PLATFORM_H */