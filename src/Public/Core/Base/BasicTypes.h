/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

struct BasicTypes
{
	typedef unsigned char		uint8_t;
	typedef unsigned short		uint16_t;
	typedef unsigned int		uint32_t;
	typedef unsigned long long	uint64_t;
	typedef signed char			int8_t;
	typedef signed short		int16_t;
	typedef signed int			int32_t;
	typedef signed long long	int64_t;

	typedef char				ansichar;
	typedef wchar_t				widechar;
	typedef uint8_t				utf8char;
	typedef uint16_t			utf16char;
	typedef uint32_t			utf32char;

#if ARCHITECTURE_X64
	typedef uint64_t			ptruint_t;
	typedef int64_t				ptrint_t;
#else
	typedef uint32_t			ptruint_t;
	typedef int32_t				ptrint_t;
#endif
	typedef ptruint_t			sizetype;
	typedef ptrint_t			ssizetype;
};
