/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

//#pragma comment(lib, "Rpcrt4.lib")
//#pragma comment(lib, "uuid.lib")
//#pragma comment(lib, "Kernel32.lib")

#if true
extern "C" {
#define DECLSPEC_IMPORT __declspec(dllimport)
#define RPCRTAPI DECLSPEC_IMPORT
#define WINUSERAPI DECLSPEC_IMPORT
#define WINBASEAPI DECLSPEC_IMPORT
//#define _ACRTIMP DECLSPEC_IMPORT
#define WINAPI      __stdcall
#define RESTRICTED_POINTER
#define FALSE 0
#define TRUE 1
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#define RTL_CONDITION_VARIABLE_LOCKMODE_SHARED  0x1     
#define CREATE_SUSPENDED                  0x00000004
#define CONDITION_VARIABLE_LOCKMODE_SHARED RTL_CONDITION_VARIABLE_LOCKMODE_SHARED
#define DECLSPEC_ALLOCATOR __declspec(allocator)

#define CONST               const
typedef long RPC_STATUS;
#define  RPC_ENTRY __stdcall
#define __RPC_FAR
#define far                 
#define near                
#define FAR                 
#define NEAR         
#define MAX_PATH          260

typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;

typedef GUID UUID;
#define CONST               const
typedef int                 INT;
typedef int                 BOOL;
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
#define VOID void
typedef void* PVOID;
typedef void far* LPVOID;

typedef char CHAR;
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR, *LPSTR, *PSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

typedef wchar_t WCHAR;
typedef WCHAR *PWCHAR, *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR, *LPWSTR, *PWSTR; 
typedef CONST WCHAR *LPCWSTR, *PCWSTR;
typedef CONST WCHAR *LPCWCHAR, *PCWCHAR;

typedef long HRESULT;

typedef int                 BOOL;
typedef BOOL near*			PBOOL;
typedef BOOL far*			LPBOOL;
typedef char                CHAR;
typedef wchar_t				WCHAR;    // wc,   16-bit UNICODE character
typedef signed char         INT8;
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef __int64             LONGLONG;
typedef __int64             LONG64;
typedef signed __int64      INT64;
typedef unsigned __int64    ULONGLONG;
typedef unsigned __int64    DWORDLONG;
typedef unsigned __int64    ULONG64;
typedef unsigned __int64    DWORD64;
typedef unsigned __int64    UINT64;
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

typedef ULONG_PTR SIZE_T, * PSIZE_T;
typedef LONG_PTR SSIZE_T, * PSSIZE_T;

typedef CONST CHAR *LPCSTR, *PCSTR;
typedef CONST WCHAR *LPCWSTR, *PCWSTR;

typedef BYTE  BOOLEAN;           
typedef BOOLEAN *PBOOLEAN;

typedef INT_PTR (FAR WINAPI *FARPROC)();

//#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HMONITOR);
typedef HINSTANCE HMODULE;      /* HMODULEs can be used in place of HINSTANCEs */
typedef HANDLE              HGLOBAL;
typedef HANDLE              HLOCAL;

typedef struct _RTL_SRWLOCK {
	PVOID Ptr;
} RTL_SRWLOCK, * PRTL_SRWLOCK;

typedef struct _RTL_CONDITION_VARIABLE {
	PVOID Ptr;
} RTL_CONDITION_VARIABLE, * PRTL_CONDITION_VARIABLE;

typedef struct _LIST_ENTRY {
	struct _LIST_ENTRY* Flink;
	struct _LIST_ENTRY* Blink;
} LIST_ENTRY, * PLIST_ENTRY, * RESTRICTED_POINTER PRLIST_ENTRY;

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

typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *PRECT, NEAR *NPRECT, FAR *LPRECT;

typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

typedef struct _VCRT_ALIGN(16) _SETJMP_FLOAT128
{
	unsigned __int64 Part[2];
} SETJMP_FLOAT128;

#define _JBLEN  16
typedef SETJMP_FLOAT128 _JBTYPE;

#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_CANCELTRYCONTINUE        0x00000006L

#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L

#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND

#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND

#define MB_DEFBUTTON1               0x00000000L
#define MB_DEFBUTTON2               0x00000100L
#define MB_DEFBUTTON3               0x00000200L
#define MB_DEFBUTTON4               0x00000300L

#define MB_APPLMODAL                0x00000000L
#define MB_SYSTEMMODAL              0x00001000L
#define MB_TASKMODAL                0x00002000L

#define MB_HELP                     0x00004000L // Help Button

#define MB_NOFOCUS                  0x00008000L
#define MB_SETFOREGROUND            0x00010000L
#define MB_DEFAULT_DESKTOP_ONLY     0x00020000L

#define MB_TOPMOST                  0x00040000L
#define MB_RIGHT                    0x00080000L
#define MB_RTLREADING               0x00100000L

typedef RTL_SRWLOCK SRWLOCK, * PSRWLOCK;
typedef RTL_CONDITION_VARIABLE CONDITION_VARIABLE, * PCONDITION_VARIABLE;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;		

#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7

#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation

#define MB_PRECOMPOSED            0x00000001  // DEPRECATED: use single precomposed characters when possible.
#define MB_COMPOSITE              0x00000002  // DEPRECATED: use multiple discrete characters when possible.
#define MB_USEGLYPHCHARS          0x00000004  // DEPRECATED: use glyph chars, not ctrl chars
#define MB_ERR_INVALID_CHARS      0x00000008  // error for invalid chars

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define STDMETHODCALLTYPE       __stdcall
#define STDMETHODVCALLTYPE      __cdecl

#define STDAPICALLTYPE          __stdcall
#define STDAPIVCALLTYPE         __cdecl

#define LMEM_FIXED          0x0000
#define LMEM_MOVEABLE       0x0002
#define LMEM_NOCOMPACT      0x0010
#define LMEM_NODISCARD      0x0020
#define LMEM_ZEROINIT       0x0040
#define LMEM_MODIFY         0x0080
#define LMEM_DISCARDABLE    0x0F00
#define LMEM_VALID_FLAGS    0x0F72
#define LMEM_INVALID_HANDLE 0x8000

#define LHND                (LMEM_MOVEABLE | LMEM_ZEROINIT)
#define LPTR                (LMEM_FIXED | LMEM_ZEROINIT)

#define NONZEROLHND         (LMEM_MOVEABLE)
#define NONZEROLPTR         (LMEM_FIXED)

#define STATUS_WAIT_0                           ((DWORD   )0x00000000L) 
#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)    
#define WAIT_FAILED ((DWORD)0xFFFFFFFF)
#define WAIT_OBJECT_0       ((STATUS_WAIT_0 ) + 0 )
#define WAIT_ABANDONED         ((STATUS_ABANDONED_WAIT_0 ) + 0 )
#define WAIT_ABANDONED_0       ((STATUS_ABANDONED_WAIT_0 ) + 0 )
#define WAIT_TIMEOUT                     258L

WINBASEAPI
DECLSPEC_ALLOCATOR
HLOCAL
WINAPI
LocalAlloc(
	UINT uFlags,
	SIZE_T uBytes
);

WINBASEAPI
HLOCAL
WINAPI
LocalFree(
	HLOCAL hMem
);

WINBASEAPI
BOOL
WINAPI
IsDebuggerPresent(
	VOID
);

WINBASEAPI
VOID
WINAPI
DebugBreak(
	VOID
);

WINBASEAPI
VOID
WINAPI
OutputDebugStringA(
	LPCSTR lpOutputString
);

WINBASEAPI
VOID
WINAPI
OutputDebugStringW(
	LPCWSTR lpOutputString
);

WINUSERAPI
int
WINAPI
MessageBoxA(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType);
WINUSERAPI
int
WINAPI
MessageBoxW(
	HWND hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT uType);

WINBASEAPI
HMODULE
WINAPI
LoadLibraryW(
	LPCWSTR lpLibFileName
	);

WINBASEAPI
HMODULE
WINAPI
LoadLibraryA(
	LPCSTR lpLibFileName
	);

WINBASEAPI
BOOL
WINAPI
FreeLibrary(
	HMODULE hLibModule
	);

WINBASEAPI
HMODULE
WINAPI
GetModuleHandleA(
	LPCSTR lpModuleName
);

WINBASEAPI
HMODULE
WINAPI
GetModuleHandleW(
	LPCWSTR lpModuleName
);

WINBASEAPI
FARPROC
WINAPI
GetProcAddress(
	HMODULE hModule,
	LPCSTR lpProcName
	);

WINBASEAPI
int
WINAPI
MultiByteToWideChar(
	UINT CodePage,
	DWORD dwFlags,
	LPCCH lpMultiByteStr,
	int cbMultiByte,
	LPWSTR lpWideCharStr,
	int cchWideChar
);

WINBASEAPI
int
WINAPI
WideCharToMultiByte(
	UINT CodePage,
	DWORD dwFlags,
	LPCWCH lpWideCharStr,
	int cchWideChar,
	LPSTR lpMultiByteStr,
	int cbMultiByte,
	LPCCH lpDefaultChar,
	LPBOOL lpUsedDefaultChar
);

DECLSPEC_IMPORT
LPWSTR*
STDAPICALLTYPE
CommandLineToArgvW(
	LPCWSTR lpCmdLine,
	int* pNumArgs);

RPCRTAPI
RPC_STATUS
RPC_ENTRY
UuidCreate(
	UUID __RPC_FAR* Uuid
);

WINBASEAPI
FARPROC
WINAPI
GetProcAddress(
	HMODULE hModule,
	LPCSTR lpProcName
	);

WINBASEAPI
HMODULE
WINAPI
LoadLibraryW(
	LPCWSTR lpLibFileName
	);

WINBASEAPI
BOOL
WINAPI
FreeLibrary(
	HMODULE hLibModule
	);

WINBASEAPI
DWORD
WINAPI
GetCurrentThreadId(
	);

WINBASEAPI
HANDLE
WINAPI
GetCurrentThread(
	);

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
OutputDebugStringA(
	LPCSTR lpOutputString
	);

WINBASEAPI
VOID
WINAPI
OutputDebugStringW(
	LPCWSTR lpOutputString
	);

WINBASEAPI
BOOL
WINAPI
SwitchToThread(
	VOID
);

WINBASEAPI
DECLSPEC_ALLOCATOR
LPVOID
WINAPI
HeapAlloc(
	HANDLE hHeap,
	DWORD dwFlags,
	SIZE_T dwBytes
);

WINBASEAPI
HANDLE
WINAPI
GetProcessHeap(
	VOID
);

WINBASEAPI
BOOL
WINAPI
HeapFree(
	HANDLE hHeap,
	DWORD dwFlags,
	LPVOID lpMem
);
WINBASEAPI
VOID
WINAPI
Sleep(
	DWORD dwMilliseconds
);
typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
_ACRTIMP uintptr_t __cdecl _beginthreadex(
	void* _Security,
	unsigned                 _StackSize,
	_beginthreadex_proc_type _StartAddress,
	void* _ArgList,
	unsigned                 _InitFlag,
	unsigned* _ThrdAddr
);
_ACRTIMP void __cdecl _endthreadex(
	unsigned _ReturnCode
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

WINBASEAPI
DWORD
WINAPI
GetLastError(
	VOID
);

WINBASEAPI
VOID
WINAPI
RaiseException(
	DWORD dwExceptionCode,
	DWORD dwExceptionFlags,
	DWORD nNumberOfArguments,
	CONST ULONG_PTR* lpArguments
);

#define GetExceptionCode            _exception_code
#define exception_code              _exception_code
#define GetExceptionInformation()   ((struct _EXCEPTION_POINTERS *)_exception_info())
#define exception_info()            ((struct _EXCEPTION_POINTERS *)_exception_info())
#define AbnormalTermination         _abnormal_termination
#define abnormal_termination        _abnormal_termination

unsigned long __cdecl _exception_code(void);
void* __cdecl _exception_info(void);
int           __cdecl _abnormal_termination(void);

#define EXCEPTION_EXECUTE_HANDLER      1
#define EXCEPTION_CONTINUE_SEARCH      0
#define EXCEPTION_CONTINUE_EXECUTION (-1)

#ifndef _JMP_BUF_DEFINED
#define _JMP_BUF_DEFINED
typedef _JBTYPE jmp_buf[_JBLEN];
#endif
int __cdecl setjmp(
	jmp_buf _Buf
);
}

__declspec(noreturn) void __cdecl longjmp(
	jmp_buf _Buf,
	int     _Value
) noexcept(false);
#else
#include <Windows.h>
#include <rpc.h>
#endif
