/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN32_SYSINFO_H
#define CORE_WIN32_SYSINFO_H 1

#include "Win32Base.h"

#if WIN32_USE_GREAPER_HEADERS

extern "C" {

#pragma warning(push)
#pragma warning(disable : 4201)
typedef struct _SYSTEM_INFO {
    union {
        DWORD dwOemId;
        struct {
            WORD wProcessorArchitecture;
            WORD wReserved;
        };
    };
    DWORD dwPageSize;
    LPVOID lpMinimumApplicationAddress;
    LPVOID lpMaximumApplicationAddress;
    DWORD_PTR dwActiveProcessorMask;
    DWORD dwNumberOfProcessors;
    DWORD dwProcessorType;
    DWORD dwAllocationGranularity;
    WORD wProcessorLevel;
    WORD wProcessorRevision;
} SYSTEM_INFO, * LPSYSTEM_INFO;
#pragma warning(pop)

typedef struct _MEMORYSTATUSEX {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, * LPMEMORYSTATUSEX;

WINBASEAPI
BOOL
WINAPI
GlobalMemoryStatusEx(
    LPMEMORYSTATUSEX lpBuffer
);

WINBASEAPI
VOID
WINAPI
GetSystemInfo(
    LPSYSTEM_INFO lpSystemInfo
);

WINBASEAPI
VOID
WINAPI
GetSystemTime(
    LPSYSTEMTIME lpSystemTime
);

WINBASEAPI
VOID
WINAPI
GetSystemTimeAsFileTime(
    LPFILETIME lpSystemTimeAsFileTime
);

WINBASEAPI
VOID
WINAPI
GetLocalTime(
    LPSYSTEMTIME lpSystemTime
);

WINBASEAPI
ULONGLONG
WINAPI
GetTickCount64(
    VOID
);

WINBASEAPI
UINT
WINAPI
GetSystemDirectoryA(
    LPSTR lpBuffer,
    UINT uSize
);

WINBASEAPI
UINT
WINAPI
GetSystemDirectoryW(
    LPWSTR lpBuffer,
    UINT uSize
);

WINBASEAPI
UINT
WINAPI
GetWindowsDirectoryA(
    LPSTR lpBuffer,
    UINT uSize
);

WINBASEAPI
UINT
WINAPI
GetWindowsDirectoryW(
    LPWSTR lpBuffer,
    UINT uSize
);

WINBASEAPI
UINT
WINAPI
GetSystemWindowsDirectoryA(
    LPSTR lpBuffer,
    UINT uSize
);

WINBASEAPI
UINT
WINAPI
GetSystemWindowsDirectoryW(
    LPWSTR lpBuffer,
    UINT uSize
);

WINBASEAPI
BOOL
WINAPI
GetPhysicallyInstalledSystemMemory(
    PULONGLONG TotalMemoryInKilobytes
);

}

#else

#endif

#endif /* CORE_WIN32_SYSINFO_H */