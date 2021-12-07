/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once
//#include "../PHAL.h"
#include <memory>

using uint8 = PlatformTypes::uint8_t;
using uint16 = PlatformTypes::uint16_t;
using uint32 = PlatformTypes::uint32_t;
using uint64 = PlatformTypes::uint64_t;
using int8 = PlatformTypes::int8_t;
using int16 = PlatformTypes::int16_t;
using int32 = PlatformTypes::int32_t;
using int64 = PlatformTypes::int64_t;
using achar = PlatformTypes::ansichar;
using wchar = PlatformTypes::widechar;
using u8char = PlatformTypes::utf8char;
using u16char = PlatformTypes::utf16char;
using u32char = PlatformTypes::utf32char;
using ptruint = PlatformTypes::ptruint_t;
using ptrint = PlatformTypes::ptrint_t;
using sizet = PlatformTypes::sizetype;
using ssizet = PlatformTypes::ssizetype;

template<class T>
using SPtr = std::shared_ptr<T>;
template<class T>
using UPtr = std::unique_ptr<T>;