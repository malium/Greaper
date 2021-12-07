/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

struct WinTypes : BasicTypes
{
	typedef unsigned __int64	uint64_t;
	typedef signed __int64		int64_t;
#if ARCHITECTURE_X64
	typedef uint64_t			ptruint_t;
	typedef int64_t				ptrint_t;
	typedef uint64_t			sizetype;
	typedef int64_t				ssizetype;
#endif
};

typedef WinTypes PlatformTypes;

/* Function type macros */
#define INLINE __forceinline									/** Forces code to be inlined */
#define NOINLINE __declspec(noinline)							/** Forces code to NOT be inlined */
#define FUNCTION_NO_RETURN_START __declspec(noreturn)			/** Indicate that the function never returns. */
#define FUNCTION_NO_RETURN_END
#define FUNCTION_FULL __FUNCTION__
#define GREAPER_DLLEXT ".dll"

/* Supported Windows version */
#ifndef GREAPER_MIN_WINDOWS_SUPPORTED
#define GREAPER_MIN_WINDOWS_SUPPORTED 0x0601
#define _WIN32_WINNT 0x0601 //! Windows 7
#else
#define _WIN32_WINNT GREAPER_MIN_WINDOWS_SUPPORTED
#endif

/* Windows extra headers remove */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN 1
#endif

/* Enable triggering a breakpoint in our IDE */
#ifndef TRIGGER_BREAKPOINT
extern void CDECL __debugbreak();
#define TRIGGER_BREAKPOINT() __debugbreak()
#endif

#define NOVTABLE __declspec(novtable)

/* String constants */
#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR '\\'
#endif
#ifndef PATH_SEPARATOR_WIDE
#define PATH_SEPARATOR_WIDE L'\\'
#endif

#ifndef DLLIMPORT
#define DLLIMPORT __declspec(dllimport)
#endif
#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif

#define FUNCTION_VARARGS_END(...)

#include "MinWinHeader.h"

#define PlatformAlloc(bytes) HeapAlloc(GetProcessHeap(), 0, bytes)
#define PlatformDealloc(mem) HeapFree(GetProcessHeap(), 0, mem)
#define PlatformAlignedAlloc(bytes, alignment) _aligned_malloc(bytes, alignment)
#define PlatformAlignedDealloc(mem) _aligned_free(mem)