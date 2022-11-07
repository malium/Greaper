/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN32_CONCURRENCY_H
#define CORE_WIN32_CONCURRENCY_H 1

#include "Win32Base.h"

#if WIN32_USE_GREAPER_HEADERS

extern "C" {
typedef struct _RTL_SRWLOCK {
		PVOID Ptr;
} RTL_SRWLOCK, * PRTL_SRWLOCK;
typedef RTL_SRWLOCK SRWLOCK, * PSRWLOCK;

typedef struct _RTL_CONDITION_VARIABLE {
	PVOID Ptr;
} RTL_CONDITION_VARIABLE, * PRTL_CONDITION_VARIABLE;
typedef RTL_CONDITION_VARIABLE CONDITION_VARIABLE, * PCONDITION_VARIABLE;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
	WORD   Type;
	WORD   CreatorBackTraceIndex;
	struct _RTL_CRITICAL_SECTION* CriticalSection;
	LIST_ENTRY ProcessLocksList;
	DWORD EntryCount;
	DWORD ContentionCount;
	DWORD Flags;
	WORD   CreatorBackTraceIndexHigh;
	WORD   SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG, * PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, * PRTL_RESOURCE_DEBUG;

#define RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO         0x01000000
#define RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN          0x02000000
#define RTL_CRITICAL_SECTION_FLAG_STATIC_INIT           0x04000000
#define RTL_CRITICAL_SECTION_FLAG_RESOURCE_TYPE         0x08000000
#define RTL_CRITICAL_SECTION_FLAG_FORCE_DEBUG_INFO      0x10000000
#define RTL_CRITICAL_SECTION_ALL_FLAG_BITS              0xFF000000
#define RTL_CRITICAL_SECTION_FLAG_RESERVED              (RTL_CRITICAL_SECTION_ALL_FLAG_BITS & (~(RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO | RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN | RTL_CRITICAL_SECTION_FLAG_STATIC_INIT | RTL_CRITICAL_SECTION_FLAG_RESOURCE_TYPE | RTL_CRITICAL_SECTION_FLAG_FORCE_DEBUG_INFO)))

#define RTL_CRITICAL_SECTION_DEBUG_FLAG_STATIC_INIT     0x00000001

#pragma pack(push, 8)

typedef struct _RTL_CRITICAL_SECTION {
	PRTL_CRITICAL_SECTION_DEBUG DebugInfo;

	//
	//  The following three fields control entering and exiting the critical
	//  section for the resource
	//

	LONG LockCount;
	LONG RecursionCount;
	HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
	HANDLE LockSemaphore;
	ULONG_PTR SpinCount;        // force size on 64-bit systems when packed
} RTL_CRITICAL_SECTION, * PRTL_CRITICAL_SECTION;

#pragma pack(pop)

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

WINBASEAPI
VOID
WINAPI
InitializeSRWLock(
	PSRWLOCK SRWLock
);

WINBASEAPI
VOID
WINAPI
ReleaseSRWLockExclusive(
	PSRWLOCK SRWLock
);

WINBASEAPI
VOID
WINAPI
ReleaseSRWLockShared(
	PSRWLOCK SRWLock
);

WINBASEAPI
VOID
WINAPI
AcquireSRWLockExclusive(
	PSRWLOCK SRWLock
);

WINBASEAPI
VOID
WINAPI
AcquireSRWLockShared(
	PSRWLOCK SRWLock
);

WINBASEAPI
BOOLEAN
WINAPI
TryAcquireSRWLockExclusive(
	PSRWLOCK SRWLock
);

WINBASEAPI
BOOLEAN
WINAPI
TryAcquireSRWLockShared(
	PSRWLOCK SRWLock
);


WINBASEAPI
VOID
WINAPI
InitializeConditionVariable(
	PCONDITION_VARIABLE ConditionVariable
);

WINBASEAPI
VOID
WINAPI
WakeConditionVariable(
	PCONDITION_VARIABLE ConditionVariable
);

WINBASEAPI
VOID
WINAPI
WakeAllConditionVariable(
	PCONDITION_VARIABLE ConditionVariable
);

WINBASEAPI
BOOL
WINAPI
SleepConditionVariableCS(
	PCONDITION_VARIABLE ConditionVariable,
	PCRITICAL_SECTION CriticalSection,
	DWORD dwMilliseconds
);

WINBASEAPI
BOOL
WINAPI
SleepConditionVariableSRW(
	PCONDITION_VARIABLE ConditionVariable,
	PSRWLOCK SRWLock,
	DWORD dwMilliseconds,
	ULONG Flags
);


WINBASEAPI
VOID
WINAPI
InitializeCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection
);

WINBASEAPI
VOID
WINAPI
EnterCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection
);

WINBASEAPI
VOID
WINAPI
LeaveCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection
);

WINBASEAPI
VOID
WINAPI
DeleteCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection
);

WINBASEAPI
BOOL
WINAPI
TryEnterCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection
);

WINBASEAPI
HANDLE
WINAPI
GetCurrentThread(
	VOID
);

WINBASEAPI
DWORD
WINAPI
GetCurrentThreadId(
	VOID
);

WINBASEAPI
BOOL
WINAPI
SwitchToThread(
	VOID
);

WINBASEAPI
HANDLE
WINAPI
GetCurrentProcess(
	VOID
);

WINBASEAPI
DWORD
WINAPI
GetCurrentProcessId(
	VOID
);

WINBASEAPI
DWORD
WINAPI
WaitForSingleObject(
	HANDLE hHandle,
	DWORD dwMilliseconds
);

WINBASEAPI
BOOL
WINAPI
TerminateThread(
	HANDLE hThread,
	DWORD dwExitCode
);

WINBASEAPI
DWORD
WINAPI
ResumeThread(
	HANDLE hThread
);

#ifndef _INC_PROCESS

typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
DECLSPEC_IMPORT uintptr_t __cdecl _beginthreadex(
	void* _Security,
	unsigned                 _StackSize,
	_beginthreadex_proc_type _StartAddress,
	void* _ArgList,
	unsigned                 _InitFlag,
	unsigned* _ThrdAddr
);
DECLSPEC_IMPORT void __cdecl _endthreadex(
	unsigned _ReturnCode
);
#endif


}
#else

#endif

#endif /* CORE_WIN32_CONCURRENCY_H */