/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef _XCR_XFEATURE_ENABLED_MASK
#define _XCR_XFEATURE_ENABLED_MASK 0
#endif

INLINE void greaper::OSPlatform::PerThreadInit()
{
	_PerThreadInit();
	cJSON_Hooks hooks;
	hooks.malloc_fn = &Alloc<GenericAllocator>;
	hooks.free_fn = &Dealloc<GenericAllocator>;
	cJSON_InitHooks(&hooks);
}

INLINE void greaper::OSPlatform::PerLibraryInit()
{
	_PerLibraryInit();

	InitCPUInfo();
}

INLINE const greaper::CPUInfo& greaper::OSPlatform::GetCPUInfo() noexcept
{
	return m_CPUInfo;
}

INLINE void greaper::OSPlatform::InitCPUInfo() noexcept
{
	std::array<int32, 4> info{};
	cpuid(info.data(), 0);
	const auto nIDs = (uint32)info[0];

	std::array<achar, 13> manufacturerID{};
	memcpy(&manufacturerID[0], &info[1], 4);
	memcpy(&manufacturerID[4], &info[3], 4);
	memcpy(&manufacturerID[8], &info[2], 4);
	manufacturerID[12] = '\0';
	if (strncmp(manufacturerID.data(), "GenuineIntel", manufacturerID.size()) == 0)
		m_CPUInfo.Vendor = CPUVendor_t::INTEL;
	else if (strncmp(manufacturerID.data(), "AuthenticAMD", manufacturerID.size()) == 0)
		m_CPUInfo.Vendor = CPUVendor_t::AMD;
	else
		m_CPUInfo.Vendor = CPUVendor_t::UNKNOWN;

	cpuid(info.data(), 0x80000000);
	const auto nExIDs = (uint32)info[0];

	bool osHasXSAVE_XRESTORE = false;

	if (nIDs >= 0x00000001)
	{
		cpuid(info.data(), 0x00000001);
		m_CPUInfo.Features.HTT = (info[3] & ((int)1 << 28)) != 0;
		m_CPUInfo.Features.SSE3 = (info[2] & ((int)1 << 0)) != 0;

		m_CPUInfo.Features.SSSE3 = (info[2] & ((int)1 << 9)) != 0;
		m_CPUInfo.Features.SSE41 = (info[2] & ((int)1 << 19)) != 0;
		m_CPUInfo.Features.SSE42 = (info[2] & ((int)1 << 20)) != 0;
		m_CPUInfo.Features.AES = (info[2] & ((int)1 << 25)) != 0;

		m_CPUInfo.Features.AVX = (info[2] & ((int)1 << 28)) != 0;
		m_CPUInfo.Features.FMA3 = (info[2] & ((int)1 << 12)) != 0;

		m_CPUInfo.Features.SMX = (info[2] & ((int)1 << 6)) != 0;
		m_CPUInfo.Features.CX16 = (info[2] & ((int)1 << 13)) != 0;

		m_CPUInfo.Features.F16C = (info[2] & ((int)1 << 29)) != 0;

		osHasXSAVE_XRESTORE = (info[2] & ((int)1 << 27)) != 0;
	}
	if (nIDs >= 0x00000007) {
		cpuid(info.data(), 0x00000007);
		m_CPUInfo.Features.AVX2 = (info[1] & ((int)1 << 5)) != 0;

		m_CPUInfo.Features.SGX = (info[1] & ((int)1 << 2)) != 0;
		m_CPUInfo.Features.ERMS = (info[1] & ((int)1 << 9)) != 0;

		m_CPUInfo.Features.BMI1 = (info[1] & ((int)1 << 3)) != 0;
		m_CPUInfo.Features.BMI2 = (info[1] & ((int)1 << 8)) != 0;
		m_CPUInfo.Features.SHA = (info[1] & ((int)1 << 29)) != 0;

		m_CPUInfo.Features.AVX512F = (info[1] & ((int)1 << 16)) != 0;
		m_CPUInfo.Features.AVX512CD = (info[1] & ((int)1 << 28)) != 0;
		m_CPUInfo.Features.AVX512PF = (info[1] & ((int)1 << 26)) != 0;
		m_CPUInfo.Features.AVX512ER = (info[1] & ((int)1 << 27)) != 0;
		m_CPUInfo.Features.AVX512VL = (info[1] & ((int)1 << 31)) != 0;
		m_CPUInfo.Features.AVX512BW = (info[1] & ((int)1 << 30)) != 0;
		m_CPUInfo.Features.AVX512DQ = (info[1] & ((int)1 << 17)) != 0;
		m_CPUInfo.Features.AVX512IFMA = (info[1] & ((int)1 << 21)) != 0;
		m_CPUInfo.Features.AVX512VBMI = (info[2] & ((int)1 << 1)) != 0;
		m_CPUInfo.Features.AVX512VBMI2 = (info[2] & ((int)1 << 6)) != 0;
		m_CPUInfo.Features.VAES = (info[2] & ((int)1 << 9)) != 0;
		m_CPUInfo.Features.VPCLMULQDQ = (info[2] & ((int)1 << 10)) != 0;
		m_CPUInfo.Features.AVX512VNNI = (info[2] & ((int)1 << 11)) != 0;
		m_CPUInfo.Features.AVX512BITALG = (info[2] & ((int)1 << 12)) != 0;
		m_CPUInfo.Features.AVX512VPOPCNTDQ = (info[2] & ((int)1 << 14)) != 0;
		m_CPUInfo.Features.AVX512FP16 = (info[3] & ((int)1 << 23)) != 0;

		m_CPUInfo.Features.HYBRID = (info[3] & ((int)1 << 15)) != 0;
	}
	if (nExIDs >= 0x80000001) {
		cpuid(info.data(), 0x80000001);
		m_CPUInfo.Features.SSE4A = (info[2] & ((int)1 << 6)) != 0;
		m_CPUInfo.Features.FMA4 = (info[2] & ((int)1 << 16)) != 0;
	}

	if (osHasXSAVE_XRESTORE && m_CPUInfo.Features.AVX != 0)
	{
		auto xcrFeatureMask = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
		m_CPUInfo.Features.AVX = (xcrFeatureMask & 0x6) == 0x6;
	}
}