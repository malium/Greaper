/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <cstdio>

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
#define FUNCTION_FULL __FUNCSIG__
#define GREAPER_DLLEXT ".dll"

/* Supported Windows version */
#ifndef GREAPER_MIN_WINDOWS_SUPPORTED
#define GREAPER_MIN_WINDOWS_SUPPORTED 0x0602 //! Windows 8
#endif
#define _WIN32_WINNT GREAPER_MIN_WINDOWS_SUPPORTED

/* Enable triggering a breakpoint in our IDE */
#ifndef TRIGGER_BREAKPOINT
#define TRIGGER_BREAKPOINT() DebugBreak()
#endif

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

#include "Win32Base.h"

#define PlatformAlloc(bytes) HeapAlloc(GetProcessHeap(), 0, bytes)
#define PlatformDealloc(mem) HeapFree(GetProcessHeap(), 0, mem)
#define PlatformAlignedAlloc(bytes, alignment) _aligned_malloc(bytes, alignment)
#define PlatformAlignedDealloc(mem) _aligned_free(mem)
#define DEBUG_OUTPUT(x) OutputDebugStringA(x)

INLINE LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT* pNumArgs) noexcept
{
	constexpr size_t BufferSize = 128;
	static char errorMsgBuffer[BufferSize];
	int retVal = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, nullptr, 0);
	if (!SUCCEEDED(retVal))
	{
		// Error
		if (IsDebuggerPresent() == TRUE)
		{
			const auto errorCode = GetLastError();
			snprintf(errorMsgBuffer, BufferSize, "MultiByteToWideChar failed, error code: %d.", errorCode);
			DEBUG_OUTPUT(errorMsgBuffer);
			TRIGGER_BREAKPOINT();
		}
		return nullptr;
	}

	auto lpWideCharStr = (LPWSTR)PlatformAlloc(retVal * sizeof(wchar_t));
	if (lpWideCharStr == nullptr)
	{
		if (IsDebuggerPresent() == TRUE)
		{
			snprintf(errorMsgBuffer, BufferSize, "PlatformAlloc couldn't allocate %zd bytes.", retVal * sizeof(wchar_t));
			DEBUG_OUTPUT(errorMsgBuffer);
			TRIGGER_BREAKPOINT();
		}
		return nullptr;
	}

	retVal = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retVal);
	if (!SUCCEEDED(retVal))
	{
		if (IsDebuggerPresent() == TRUE)
		{
			const auto errorCode = GetLastError();
			snprintf(errorMsgBuffer, BufferSize, "MultiByteToWideChar failed, error code: %d.", errorCode);
			DEBUG_OUTPUT(errorMsgBuffer);
			TRIGGER_BREAKPOINT();
		}
		PlatformDealloc(lpWideCharStr);
		return nullptr;
	}

	int numArgs;
	LPWSTR* args = CommandLineToArgvW(lpWideCharStr, &numArgs);
	PlatformDealloc(lpWideCharStr);
	if (!args)
	{
		if (IsDebuggerPresent() == TRUE)
		{
			const auto errorCode = GetLastError();
			snprintf(errorMsgBuffer, BufferSize, "CommandLineToArgvW returned nullptr, error code: %d.", errorCode);
			DEBUG_OUTPUT(errorMsgBuffer);
			TRIGGER_BREAKPOINT();
		}
		return nullptr;
	}

	int storage = numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++i)
	{
		BOOL lpUsedDefaultChar = FALSE;
		retVal = WideCharToMultiByte(CP_ACP, 0, args[i], -1, nullptr, 0,
			nullptr, &lpUsedDefaultChar);
		if (!SUCCEEDED(retVal))
		{
			if (IsDebuggerPresent() == TRUE)
			{
				const auto errorCode = GetLastError();
				snprintf(errorMsgBuffer, BufferSize, "WideCharToMultiByte failed, error code: %d.", errorCode);
				DEBUG_OUTPUT(errorMsgBuffer);
				TRIGGER_BREAKPOINT();
			}
			LocalFree(args);
			return nullptr;
		}
		storage += retVal;
	}
	auto* result = (LPSTR*)LocalAlloc(LMEM_FIXED, storage);
	if (!result)
	{
		LocalFree(args);
		if (IsDebuggerPresent() == TRUE)
		{
			const auto errorCode = GetLastError();
			snprintf(errorMsgBuffer, BufferSize, "LocalAlloc returned nullptr, error code: %d.", errorCode);
			DEBUG_OUTPUT(errorMsgBuffer);
			TRIGGER_BREAKPOINT();
		}
		return nullptr;
	}

	int bufLen = storage - numArgs * sizeof(LPSTR);
	LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++i)
	{
		if (bufLen <= 0)
		{
			if (IsDebuggerPresent() == TRUE)
			{
				const auto errorCode = GetLastError();
				snprintf(errorMsgBuffer, BufferSize, "LocalAlloc returned nullptr, error code: %d.", errorCode);
				DEBUG_OUTPUT(errorMsgBuffer);
				TRIGGER_BREAKPOINT();
			}
			LocalFree(result);
			LocalFree(args);
			return nullptr;
		}
		BOOL lpUsedDefaultChar = FALSE;
		retVal = WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen,
			nullptr, &lpUsedDefaultChar);
		if (!SUCCEEDED(retVal))
		{
			if (IsDebuggerPresent() == TRUE)
			{
				const auto errorCode = GetLastError();
				snprintf(errorMsgBuffer, BufferSize, "WideCharToMultiByte failed, error code: %d.", errorCode);
				DEBUG_OUTPUT(errorMsgBuffer);
				TRIGGER_BREAKPOINT();
			}
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

INLINE void FreeArgvA(LPSTR* argv) noexcept
{
	if (argv == nullptr)
		return;
	LocalFree(argv);
}