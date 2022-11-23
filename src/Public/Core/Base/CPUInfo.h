/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_CPU_INFO_H
#define CORE_CPU_INFO_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
	struct CPUFeatures
	{
		uint64 AES : 1;
		uint64 ERMS : 1;
		uint64 F16C : 1;
		uint64 FMA3 : 1;
		uint64 FMA4 : 1;
		uint64 VPCLMULQDQ : 1;
		uint64 BMI1 : 1;
		uint64 BMI2 : 1;
		uint64 SHA : 1;
		uint64 VAES : 1;
		uint64 HTT : 1;

		uint64 SSE3 : 1;
		uint64 SSSE3 : 1;
		uint64 SSE41 : 1;
		uint64 SSE42 : 1;
		uint64 SSE4A : 1;

		uint64 AVX : 1;
		uint64 AVX2 : 1;

		uint64 AVX512F : 1;		
		uint64 AVX512CD : 1;		
		uint64 AVX512ER : 1;		
		uint64 AVX512PF : 1;		
		uint64 AVX512BW : 1;		
		uint64 AVX512DQ : 1;		
		uint64 AVX512VL : 1;		
		uint64 AVX512IFMA : 1;		
		uint64 AVX512VBMI : 1;		
		uint64 AVX512VBMI2 : 1;	
		uint64 AVX512VNNI : 1;		
		uint64 AVX512BITALG : 1;	
		uint64 AVX512VPOPCNTDQ : 1;
		uint64 AVX512FP16 : 1;

		uint64 SMX : 1;
		uint64 SGX : 1;
		uint64 CX16 : 1;

		uint64 HYBRID : 1;
	};

	namespace ECPUVendor
	{
		enum Type
		{
			INTEL,
			AMD,
			UNKNOWN
		};
	}
	using CPUVendor_t = ECPUVendor::Type;

	struct CPUInfo
	{
		CPUFeatures Features = { 0 };
		CPUVendor_t Vendor = CPUVendor_t::UNKNOWN;

		constexpr CPUInfo()noexcept = default;
	};
}

#endif /* CORE_CPU_INFO_H */