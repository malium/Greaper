/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <malloc.h>

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
//extern void CDECL __debugbreak();
//#define TRIGGER_BREAKPOINT() __debugbreak()
// Use the MinWinHeader import
#define TRIGGER_BREAKPOINT() DebugBreak()
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
#define DEBUG_OUTPUT(x) OutputDebugStringA(x)

INLINE LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT* pNumArgs)
{
	int retVal = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, nullptr, 0);
	if (!SUCCEEDED(retVal))
		return nullptr;

	LPWSTR lpWideCharStr = (LPWSTR)malloc(retVal * sizeof(wchar_t));
	if (lpWideCharStr == nullptr)
		return nullptr;
	retVal = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retVal);
	if (!SUCCEEDED(retVal))
	{
		free(lpWideCharStr);
		return nullptr;
	}

	int numArgs;
	LPWSTR* args = CommandLineToArgvW(lpWideCharStr, &numArgs);
	free(lpWideCharStr);
	if (!args)
		return nullptr;

	int storage = numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++i)
	{
		BOOL lpUsedDefaultChar = FALSE;
		retVal = WideCharToMultiByte(CP_ACP, 0, args[i], -1, nullptr, 0,
			nullptr, &lpUsedDefaultChar);
		if (!SUCCEEDED(retVal))
		{
			LocalFree(args);
			return nullptr;
		}
		storage += retVal;
	}
	LPSTR* result = (LPSTR*)LocalAlloc(LMEM_FIXED, storage);
	if (!result)
	{
		LocalFree(args);
		return nullptr;
	}

	int bufLen = storage - numArgs * sizeof(LPSTR);
	LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++i)
	{
		if (bufLen <= 0)
		{
			if (IsDebuggerPresent() == TRUE)
				TRIGGER_BREAKPOINT();
			return nullptr;
		}
		BOOL lpUsedDefaultChar = FALSE;
		retVal = WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen,
			nullptr, &lpUsedDefaultChar);
		if (!SUCCEEDED(retVal))
		{
			LocalFree(result);
			LocalFree(args);
			return nullptr;
		}

		result[i] = buffer;
		buffer += retVal;
		bufLen -= retVal;
	}

	LocalFree(args);
	*pNumArgs = numArgs;
	return result;
}

INLINE void FreeArgvA(LPSTR* argv)
{
	if (argv == nullptr)
		return;
	LocalFree(argv);
}