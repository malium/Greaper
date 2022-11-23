/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H 1

#include "CorePrerequisites.h"

namespace greaper
{
	namespace EOSType
	{
		enum Type
		{
			Windows,
			Linux
		};
	};
	using OSType_t = EOSType::Type;
}

#if PLT_WINDOWS
#include "Win/WinPlatform.h"
#include <intrin.h>
#define cpuid(out, x) __cpuidex(out, x, 0)
#elif PLT_LINUX
#include "Lnx/LnxPlatform.h"
#include <cpuid.h>
#define cpuid(out, x) __cpuid_count(x, 0, out[0], out[1], out[2], out[3])
#endif

namespace greaper
{
	class OSPlatform
#if PLT_WINDOWS
		final : public WinOSPlatform
#elif PLT_LINUX
		final : public LnxOSPlatform
#endif
	{
	public:
		static void PerThreadInit();

		static void PerLibraryInit();

		static const CPUInfo& GetCPUInfo()noexcept;

	private:
		static inline CPUInfo m_CPUInfo;

		static void InitCPUInfo()noexcept;
	};
}

#include "Base/Platform.inl"

#undef cpuid

#endif /* CORE_PLATFORM_H */
