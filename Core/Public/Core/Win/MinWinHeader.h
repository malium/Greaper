/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#if 1
extern "C" {
#define DECLSPEC_IMPORT __declspec(dllimport)
#define RPCRTAPI DECLSPEC_IMPORT
#define WINUSERAPI DECLSPEC_IMPORT
#define WINBASEAPI DECLSPEC_IMPORT
#define NTSYSAPI     DECLSPEC_IMPORT
#define NTSYSCALLAPI DECLSPEC_IMPORT
#ifndef _ACRTIMP
#define _ACRTIMP __declspec(dllimport)
#endif
#define WINAPI      __stdcall
#define NTAPI __stdcall
#define RESTRICTED_POINTER
#define FALSE 0
#define TRUE 1
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#define RTL_CONDITION_VARIABLE_LOCKMODE_SHARED  0x1     
#define CREATE_SUSPENDED                  0x00000004
#define CONDITION_VARIABLE_LOCKMODE_SHARED RTL_CONDITION_VARIABLE_LOCKMODE_SHARED
#define DECLSPEC_ALLOCATOR __declspec(allocator)
#define DECLSPEC_ALIGN(x)   __declspec(align(x))
#define DECLSPEC_NOINITALL __pragma(warning(push)) __pragma(warning(disable:4845)) __declspec(no_init_all) __pragma(warning(pop))

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
typedef unsigned int ULONG32, * PULONG32;
typedef unsigned int DWORD32, * PDWORD32;
typedef unsigned __int64 ULONG64, * PULONG64;
typedef unsigned __int64 DWORD64, * PDWORD64;
typedef unsigned __int64    UINT64;
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#if ARCHITECTURE_X64
typedef unsigned __int64  uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif
#endif

typedef DWORD near* PDWORD;
typedef DWORD far* LPDWORD;

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
HANDLE
WINAPI
GetCurrentProcess(
	);
WINBASEAPI
DWORD
WINAPI
GetCurrentProcessId(
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
HANDLE
WINAPI
HeapCreate(
	DWORD flOptions,
	SIZE_T dwInitialSize,
	SIZE_T dwMaximumSize
);

WINBASEAPI
BOOL
WINAPI
HeapDestroy(
	HANDLE hHeap
);

WINBASEAPI
VOID
WINAPI
Sleep(
	DWORD dwMilliseconds
);
typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
_ACRTIMP uintptr_t CDECL _beginthreadex(
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

typedef struct DECLSPEC_ALIGN(16) _M128A {
	ULONGLONG Low;
	LONGLONG High;
} M128A, * PM128A;

typedef struct DECLSPEC_ALIGN(16) _XSAVE_FORMAT {
	WORD   ControlWord;
	WORD   StatusWord;
	BYTE  TagWord;
	BYTE  Reserved1;
	WORD   ErrorOpcode;
	DWORD ErrorOffset;
	WORD   ErrorSelector;
	WORD   Reserved2;
	DWORD DataOffset;
	WORD   DataSelector;
	WORD   Reserved3;
	DWORD MxCsr;
	DWORD MxCsr_Mask;
	M128A FloatRegisters[8];

#if ARCHITECTURE_X64

	M128A XmmRegisters[16];
	BYTE  Reserved4[96];

#else

	M128A XmmRegisters[8];
	BYTE  Reserved4[224];

#endif

} XSAVE_FORMAT, * PXSAVE_FORMAT;

typedef XSAVE_FORMAT XMM_SAVE_AREA32, * PXMM_SAVE_AREA32;

typedef struct DECLSPEC_ALIGN(16) DECLSPEC_NOINITALL _CONTEXT {

	//
	// Register parameter home addresses.
	//
	// N.B. These fields are for convience - they could be used to extend the
	//      context record in the future.
	//

	DWORD64 P1Home;
	DWORD64 P2Home;
	DWORD64 P3Home;
	DWORD64 P4Home;
	DWORD64 P5Home;
	DWORD64 P6Home;

	//
	// Control flags.
	//

	DWORD ContextFlags;
	DWORD MxCsr;

	//
	// Segment Registers and processor flags.
	//

	WORD   SegCs;
	WORD   SegDs;
	WORD   SegEs;
	WORD   SegFs;
	WORD   SegGs;
	WORD   SegSs;
	DWORD EFlags;

	//
	// Debug registers
	//

	DWORD64 Dr0;
	DWORD64 Dr1;
	DWORD64 Dr2;
	DWORD64 Dr3;
	DWORD64 Dr6;
	DWORD64 Dr7;

	//
	// Integer registers.
	//

	DWORD64 Rax;
	DWORD64 Rcx;
	DWORD64 Rdx;
	DWORD64 Rbx;
	DWORD64 Rsp;
	DWORD64 Rbp;
	DWORD64 Rsi;
	DWORD64 Rdi;
	DWORD64 R8;
	DWORD64 R9;
	DWORD64 R10;
	DWORD64 R11;
	DWORD64 R12;
	DWORD64 R13;
	DWORD64 R14;
	DWORD64 R15;

	//
	// Program counter.
	//

	DWORD64 Rip;

	//
	// Floating point state.
	//

	union {
		XMM_SAVE_AREA32 FltSave;
		struct {
			M128A Header[2];
			M128A Legacy[8];
			M128A Xmm0;
			M128A Xmm1;
			M128A Xmm2;
			M128A Xmm3;
			M128A Xmm4;
			M128A Xmm5;
			M128A Xmm6;
			M128A Xmm7;
			M128A Xmm8;
			M128A Xmm9;
			M128A Xmm10;
			M128A Xmm11;
			M128A Xmm12;
			M128A Xmm13;
			M128A Xmm14;
			M128A Xmm15;
		} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME;

	//
	// Vector registers.
	//

	M128A VectorRegister[26];
	DWORD64 VectorControl;

	//
	// Special debug control registers.
	//

	DWORD64 DebugControl;
	DWORD64 LastBranchToRip;
	DWORD64 LastBranchFromRip;
	DWORD64 LastExceptionToRip;
	DWORD64 LastExceptionFromRip;
} CONTEXT, * PCONTEXT;

#define EXCEPTION_MAXIMUM_PARAMETERS 15 // maximum number of exception parameters

typedef struct _EXCEPTION_RECORD {
	DWORD    ExceptionCode;
	DWORD ExceptionFlags;
	struct _EXCEPTION_RECORD* ExceptionRecord;
	PVOID ExceptionAddress;
	DWORD NumberParameters;
	ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD;

typedef EXCEPTION_RECORD* PEXCEPTION_RECORD;

typedef struct _EXCEPTION_RECORD32 {
	DWORD    ExceptionCode;
	DWORD ExceptionFlags;
	DWORD ExceptionRecord;
	DWORD ExceptionAddress;
	DWORD NumberParameters;
	DWORD ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD32, * PEXCEPTION_RECORD32;

typedef struct _EXCEPTION_RECORD64 {
	DWORD    ExceptionCode;
	DWORD ExceptionFlags;
	DWORD64 ExceptionRecord;
	DWORD64 ExceptionAddress;
	DWORD NumberParameters;
	DWORD __unusedAlignment;
	DWORD64 ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD64, * PEXCEPTION_RECORD64;

typedef struct _EXCEPTION_POINTERS {
	PEXCEPTION_RECORD ExceptionRecord;
	PCONTEXT ContextRecord;
} EXCEPTION_POINTERS, * PEXCEPTION_POINTERS;

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

typedef struct _MODULEINFO {
	LPVOID lpBaseOfDll;
	DWORD SizeOfImage;
	LPVOID EntryPoint;
} MODULEINFO, * LPMODULEINFO;

#define STATUS_DATATYPE_MISALIGNMENT     ((DWORD   )0x80000002L)   
#define STATUS_ACCESS_VIOLATION          ((DWORD   )0xC0000005L)    
#define STATUS_IN_PAGE_ERROR             ((DWORD   )0xC0000006L)    
#define STATUS_INVALID_HANDLE            ((DWORD   )0xC0000008L)
#define STATUS_ILLEGAL_INSTRUCTION       ((DWORD   )0xC000001DL)
#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((DWORD   )0xC000008CL)
#define STATUS_FLOAT_DENORMAL_OPERAND    ((DWORD   )0xC000008DL)    
#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((DWORD   )0xC000008EL)    
#define STATUS_FLOAT_INVALID_OPERATION   ((DWORD   )0xC0000090L)    
#define STATUS_FLOAT_OVERFLOW            ((DWORD   )0xC0000091L)    
#define STATUS_FLOAT_STACK_CHECK         ((DWORD   )0xC0000092L)    
#define STATUS_FLOAT_UNDERFLOW           ((DWORD   )0xC0000093L)    
#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((DWORD   )0xC0000094L)    
#define STATUS_INTEGER_OVERFLOW          ((DWORD   )0xC0000095L)    
#define STATUS_STACK_OVERFLOW            ((DWORD   )0xC00000FDL)
#define STATUS_POSSIBLE_DEADLOCK         ((DWORD   )0xC0000194L)

#define EXCEPTION_ACCESS_VIOLATION          STATUS_ACCESS_VIOLATION
#define EXCEPTION_IN_PAGE_ERROR             STATUS_IN_PAGE_ERROR
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED     STATUS_ARRAY_BOUNDS_EXCEEDED
#define EXCEPTION_DATATYPE_MISALIGNMENT     STATUS_DATATYPE_MISALIGNMENT
#define EXCEPTION_FLT_DENORMAL_OPERAND      STATUS_FLOAT_DENORMAL_OPERAND
#define EXCEPTION_FLT_DIVIDE_BY_ZERO        STATUS_FLOAT_DIVIDE_BY_ZERO
#define EXCEPTION_FLT_INVALID_OPERATION     STATUS_FLOAT_INVALID_OPERATION
#define EXCEPTION_FLT_OVERFLOW              STATUS_FLOAT_OVERFLOW
#define EXCEPTION_FLT_UNDERFLOW             STATUS_FLOAT_UNDERFLOW
#define EXCEPTION_FLT_STACK_CHECK           STATUS_FLOAT_STACK_CHECK
#define EXCEPTION_ILLEGAL_INSTRUCTION       STATUS_ILLEGAL_INSTRUCTION
#define EXCEPTION_INT_DIVIDE_BY_ZERO        STATUS_INTEGER_DIVIDE_BY_ZERO
#define EXCEPTION_INT_OVERFLOW              STATUS_INTEGER_OVERFLOW
#define EXCEPTION_STACK_OVERFLOW            STATUS_STACK_OVERFLOW
#define EXCEPTION_INVALID_HANDLE            STATUS_INVALID_HANDLE
#define EXCEPTION_POSSIBLE_DEADLOCK         STATUS_POSSIBLE_DEADLOCK

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
	DWORD ThreadId;
	PEXCEPTION_POINTERS ExceptionPointers;
	BOOL ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION, * PMINIDUMP_EXCEPTION_INFORMATION;

typedef struct _MINIDUMP_EXCEPTION_INFORMATION64 {
	DWORD ThreadId;
	ULONG64 ExceptionRecord;
	ULONG64 ContextRecord;
	BOOL ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION64, * PMINIDUMP_EXCEPTION_INFORMATION64;

typedef enum _MINIDUMP_TYPE {
	MiniDumpNormal = 0x00000000,
	MiniDumpWithDataSegs = 0x00000001,
	MiniDumpWithFullMemory = 0x00000002,
	MiniDumpWithHandleData = 0x00000004,
	MiniDumpFilterMemory = 0x00000008,
	MiniDumpScanMemory = 0x00000010,
	MiniDumpWithUnloadedModules = 0x00000020,
	MiniDumpWithIndirectlyReferencedMemory = 0x00000040,
	MiniDumpFilterModulePaths = 0x00000080,
	MiniDumpWithProcessThreadData = 0x00000100,
	MiniDumpWithPrivateReadWriteMemory = 0x00000200,
	MiniDumpWithoutOptionalData = 0x00000400,
	MiniDumpWithFullMemoryInfo = 0x00000800,
	MiniDumpWithThreadInfo = 0x00001000,
	MiniDumpWithCodeSegs = 0x00002000,
	MiniDumpWithoutAuxiliaryState = 0x00004000,
	MiniDumpWithFullAuxiliaryState = 0x00008000,
	MiniDumpWithPrivateWriteCopyMemory = 0x00010000,
	MiniDumpIgnoreInaccessibleMemory = 0x00020000,
	MiniDumpWithTokenInformation = 0x00040000,
	MiniDumpWithModuleHeaders = 0x00080000,
	MiniDumpFilterTriage = 0x00100000,
	MiniDumpWithAvxXStateContext = 0x00200000,
	MiniDumpWithIptTrace = 0x00400000,
	MiniDumpScanInaccessiblePartialPages = 0x00800000,
	MiniDumpFilterWriteCombinedMemory = 0x01000000,
	MiniDumpValidTypeFlags = 0x01ffffff,
} MINIDUMP_TYPE;

typedef struct _MINIDUMP_USER_STREAM {
	ULONG32 Type;
	ULONG BufferSize;
	PVOID Buffer;

} MINIDUMP_USER_STREAM, * PMINIDUMP_USER_STREAM;

typedef struct _MINIDUMP_USER_STREAM_INFORMATION {
	ULONG UserStreamCount;
	PMINIDUMP_USER_STREAM UserStreamArray;
} MINIDUMP_USER_STREAM_INFORMATION, * PMINIDUMP_USER_STREAM_INFORMATION;

typedef struct _MINIDUMP_THREAD_CALLBACK {
	ULONG ThreadId;
	HANDLE ThreadHandle;
	CONTEXT Context;
	ULONG SizeOfContext;
	ULONG64 StackBase;
	ULONG64 StackEnd;
} MINIDUMP_THREAD_CALLBACK, * PMINIDUMP_THREAD_CALLBACK;

typedef struct _MINIDUMP_THREAD_EX_CALLBACK {
	ULONG ThreadId;
	HANDLE ThreadHandle;
	CONTEXT Context;
	ULONG SizeOfContext;
	ULONG64 StackBase;
	ULONG64 StackEnd;
	ULONG64 BackingStoreBase;
	ULONG64 BackingStoreEnd;
} MINIDUMP_THREAD_EX_CALLBACK, * PMINIDUMP_THREAD_EX_CALLBACK;

typedef struct tagVS_FIXEDFILEINFO
{
	DWORD   dwSignature;            /* e.g. 0xfeef04bd */
	DWORD   dwStrucVersion;         /* e.g. 0x00000042 = "0.42" */
	DWORD   dwFileVersionMS;        /* e.g. 0x00030075 = "3.75" */
	DWORD   dwFileVersionLS;        /* e.g. 0x00000031 = "0.31" */
	DWORD   dwProductVersionMS;     /* e.g. 0x00030010 = "3.10" */
	DWORD   dwProductVersionLS;     /* e.g. 0x00000031 = "0.31" */
	DWORD   dwFileFlagsMask;        /* = 0x3F for version "0.42" */
	DWORD   dwFileFlags;            /* e.g. VFF_DEBUG | VFF_PRERELEASE */
	DWORD   dwFileOS;               /* e.g. VOS_DOS_WINDOWS16 */
	DWORD   dwFileType;             /* e.g. VFT_DRIVER */
	DWORD   dwFileSubtype;          /* e.g. VFT2_DRV_KEYBOARD */
	DWORD   dwFileDateMS;           /* e.g. 0 */
	DWORD   dwFileDateLS;           /* e.g. 0 */
} VS_FIXEDFILEINFO;

typedef struct _MINIDUMP_MODULE_CALLBACK {
	PWCHAR FullPath;
	ULONG64 BaseOfImage;
	ULONG SizeOfImage;
	ULONG CheckSum;
	ULONG TimeDateStamp;
	VS_FIXEDFILEINFO VersionInfo;
	PVOID CvRecord;
	ULONG SizeOfCvRecord;
	PVOID MiscRecord;
	ULONG SizeOfMiscRecord;
} MINIDUMP_MODULE_CALLBACK, * PMINIDUMP_MODULE_CALLBACK;

typedef struct _MINIDUMP_INCLUDE_THREAD_CALLBACK {
	ULONG ThreadId;
} MINIDUMP_INCLUDE_THREAD_CALLBACK, * PMINIDUMP_INCLUDE_THREAD_CALLBACK;

typedef struct _MINIDUMP_INCLUDE_MODULE_CALLBACK {
	ULONG64 BaseOfImage;
} MINIDUMP_INCLUDE_MODULE_CALLBACK, * PMINIDUMP_INCLUDE_MODULE_CALLBACK;

typedef struct _MINIDUMP_IO_CALLBACK {
	HANDLE Handle;
	ULONG64 Offset;
	PVOID Buffer;
	ULONG BufferBytes;
} MINIDUMP_IO_CALLBACK, * PMINIDUMP_IO_CALLBACK;

typedef struct _MINIDUMP_READ_MEMORY_FAILURE_CALLBACK
{
	ULONG64 Offset;
	ULONG Bytes;
	HRESULT FailureStatus;
} MINIDUMP_READ_MEMORY_FAILURE_CALLBACK,
* PMINIDUMP_READ_MEMORY_FAILURE_CALLBACK;

typedef struct _MINIDUMP_VM_QUERY_CALLBACK
{
	ULONG64 Offset;
} MINIDUMP_VM_QUERY_CALLBACK, * PMINIDUMP_VM_QUERY_CALLBACK;

typedef struct _MINIDUMP_VM_PRE_READ_CALLBACK
{
	ULONG64 Offset;
	PVOID Buffer;
	ULONG Size;
} MINIDUMP_VM_PRE_READ_CALLBACK, * PMINIDUMP_VM_PRE_READ_CALLBACK;

typedef struct _MINIDUMP_VM_POST_READ_CALLBACK
{
	ULONG64 Offset;
	PVOID Buffer;
	ULONG Size;
	ULONG Completed;
	HRESULT Status;
} MINIDUMP_VM_POST_READ_CALLBACK, * PMINIDUMP_VM_POST_READ_CALLBACK;

typedef struct _MINIDUMP_MEMORY_INFO {
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG32 AllocationProtect;
	ULONG32 __alignment1;
	ULONG64 RegionSize;
	ULONG32 State;
	ULONG32 Protect;
	ULONG32 Type;
	ULONG32 __alignment2;
} MINIDUMP_MEMORY_INFO, * PMINIDUMP_MEMORY_INFO;

typedef struct _MINIDUMP_CALLBACK_INPUT {
	ULONG ProcessId;
	HANDLE ProcessHandle;
	ULONG CallbackType;
	union {
		HRESULT Status;
		MINIDUMP_THREAD_CALLBACK Thread;
		MINIDUMP_THREAD_EX_CALLBACK ThreadEx;
		MINIDUMP_MODULE_CALLBACK Module;
		MINIDUMP_INCLUDE_THREAD_CALLBACK IncludeThread;
		MINIDUMP_INCLUDE_MODULE_CALLBACK IncludeModule;
		MINIDUMP_IO_CALLBACK Io;
		MINIDUMP_READ_MEMORY_FAILURE_CALLBACK ReadMemoryFailure;
		ULONG SecondaryFlags;
		MINIDUMP_VM_QUERY_CALLBACK VmQuery;
		MINIDUMP_VM_PRE_READ_CALLBACK VmPreRead;
		MINIDUMP_VM_POST_READ_CALLBACK VmPostRead;
	};
} MINIDUMP_CALLBACK_INPUT, * PMINIDUMP_CALLBACK_INPUT;

#pragma warning(push)
#pragma warning(disable: 4201)

typedef struct _MINIDUMP_CALLBACK_OUTPUT {
	union {
		ULONG ModuleWriteFlags;
		ULONG ThreadWriteFlags;
		ULONG SecondaryFlags;
		struct {
			ULONG64 MemoryBase;
			ULONG MemorySize;
		};
		struct {
			BOOL CheckCancel;
			BOOL Cancel;
		};
		HANDLE Handle;
		struct {
			MINIDUMP_MEMORY_INFO VmRegion;
			BOOL Continue;
		};
		struct {
			HRESULT VmQueryStatus;
			MINIDUMP_MEMORY_INFO VmQueryResult;
		};
		struct {
			HRESULT VmReadStatus;
			ULONG VmReadBytesCompleted;
		};
		HRESULT Status;
	};
} MINIDUMP_CALLBACK_OUTPUT, * PMINIDUMP_CALLBACK_OUTPUT;

#pragma warning(pop)
typedef
BOOL
(WINAPI* MINIDUMP_CALLBACK_ROUTINE) (
	PVOID CallbackParam,
	PMINIDUMP_CALLBACK_INPUT CallbackInput,
	PMINIDUMP_CALLBACK_OUTPUT CallbackOutput
	);

typedef struct _MINIDUMP_CALLBACK_INFORMATION {
	MINIDUMP_CALLBACK_ROUTINE CallbackRoutine;
	PVOID CallbackParam;
} MINIDUMP_CALLBACK_INFORMATION, * PMINIDUMP_CALLBACK_INFORMATION;

typedef struct _SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, * PSECURITY_ATTRIBUTES, * LPSECURITY_ATTRIBUTES;

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  
#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
#define FILE_ATTRIBUTE_INTEGRITY_STREAM     0x00008000  
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  
#define FILE_ATTRIBUTE_NO_SCRUB_DATA        0x00020000  
#define FILE_ATTRIBUTE_EA                   0x00040000  
#define FILE_ATTRIBUTE_PINNED               0x00080000  
#define FILE_ATTRIBUTE_UNPINNED             0x00100000  
#define FILE_ATTRIBUTE_RECALL_ON_OPEN       0x00040000  
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS 0x00400000

WINBASEAPI
HANDLE
WINAPI
CreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
);

WINBASEAPI
HANDLE
WINAPI
CreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
);

WINBASEAPI
DWORD
WINAPI
GetFullPathNameA(
	LPCSTR lpFileName,
	DWORD nBufferLength,
	LPSTR lpBuffer,
	LPSTR* lpFilePart
);

WINBASEAPI
BOOL
WINAPI
DeleteFileA(
	LPCSTR lpFileName
);

WINBASEAPI
BOOL
WINAPI
DeleteFileW(
	LPCWSTR lpFileName
);

WINBASEAPI
BOOL
WINAPI
CreateDirectoryA(
	LPCSTR lpPathName,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

WINBASEAPI
BOOL
WINAPI
CreateDirectoryW(
	LPCWSTR lpPathName,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

WINBASEAPI
BOOL
WINAPI
CloseHandle(
	HANDLE hObject
);

WINBASEAPI
BOOL
WINAPI
CompareObjectHandles(
	HANDLE hFirstObjectHandle,
	HANDLE hSecondObjectHandle
);

NTSYSAPI
void
NTAPI
RtlCaptureContext(
	PCONTEXT ContextRecord
);

#define SYMOPT_LOAD_LINES                0x00000010
#define SYMOPT_UNDNAME                   0x00000002
#define SYMOPT_FAIL_CRITICAL_ERRORS      0x00000200
#define SYMOPT_EXACT_SYMBOLS             0x00000400
#define SYMOPT_NO_PROMPTS                0x00080000

typedef enum {
	SymNone = 0,
	SymCoff,
	SymCv,
	SymPdb,
	SymExport,
	SymDeferred,
	SymSym,       // .sym file
	SymDia,
	SymVirtual,
	NumSymTypes
} SYM_TYPE;

typedef struct _IMAGEHLP_MODULE64 {
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE64)
	DWORD64  BaseOfImage;            // base load address of module
	DWORD    ImageSize;              // virtual size of the loaded module
	DWORD    TimeDateStamp;          // date/time stamp from pe header
	DWORD    CheckSum;               // checksum from the pe header
	DWORD    NumSyms;                // number of symbols in the symbol table
	SYM_TYPE SymType;                // type of symbols loaded
	CHAR     ModuleName[32];         // module name
	CHAR     ImageName[256];         // image name
	CHAR     LoadedImageName[256];   // symbol file name
	// new elements: 07-Jun-2002
	CHAR     LoadedPdbName[256];     // pdb file name
	DWORD    CVSig;                  // Signature of the CV record in the debug directories
	CHAR     CVData[MAX_PATH * 3];   // Contents of the CV record
	DWORD    PdbSig;                 // Signature of PDB
	GUID     PdbSig70;               // Signature of PDB (VC 7 and up)
	DWORD    PdbAge;                 // DBI age of pdb
	BOOL     PdbUnmatched;           // loaded an unmatched pdb
	BOOL     DbgUnmatched;           // loaded an unmatched dbg
	BOOL     LineNumbers;            // we have line number information
	BOOL     GlobalSymbols;          // we have internal symbol information
	BOOL     TypeInfo;               // we have type information
	// new elements: 17-Dec-2003
	BOOL     SourceIndexed;          // pdb supports source server
	BOOL     Publics;                // contains public symbols
	// new element: 15-Jul-2009
	DWORD    MachineType;            // IMAGE_FILE_MACHINE_XXX from ntimage.h and winnt.h
	DWORD    Reserved;               // Padding - don't remove.
} IMAGEHLP_MODULE64, * PIMAGEHLP_MODULE64;

typedef enum {
	AddrMode1616,
	AddrMode1632,
	AddrModeReal,
	AddrModeFlat
} ADDRESS_MODE;

typedef struct _tagADDRESS64 {
	DWORD64       Offset;
	WORD          Segment;
	ADDRESS_MODE  Mode;
} ADDRESS64, * LPADDRESS64;

typedef struct _KDHELP64 {

	//
	// address of kernel thread object, as provided in the
	// WAIT_STATE_CHANGE packet.
	//
	DWORD64   Thread;

	//
	// offset in thread object to pointer to the current callback frame
	// in kernel stack.
	//
	DWORD   ThCallbackStack;

	//
	// offset in thread object to pointer to the current callback backing
	// store frame in kernel stack.
	//
	DWORD   ThCallbackBStore;

	//
	// offsets to values in frame:
	//
	// address of next callback frame
	DWORD   NextCallback;

	// address of saved frame pointer (if applicable)
	DWORD   FramePointer;


	//
	// Address of the kernel function that calls out to user mode
	//
	DWORD64   KiCallUserMode;

	//
	// Address of the user mode dispatcher function
	//
	DWORD64   KeUserCallbackDispatcher;

	//
	// Lowest kernel mode address
	//
	DWORD64   SystemRangeStart;

	//
	// Address of the user mode exception dispatcher function.
	// Added in API version 10.
	//
	DWORD64   KiUserExceptionDispatcher;

	//
	// Stack bounds, added in API version 11.
	//
	DWORD64   StackBase;
	DWORD64   StackLimit;

	//
	// Target OS build number. Added in API version 12.
	//
	DWORD     BuildVersion;
	DWORD     RetpolineStubFunctionTableSize;
	DWORD64   RetpolineStubFunctionTable;
	DWORD     RetpolineStubOffset;
	DWORD     RetpolineStubSize;
	DWORD64   Reserved0[2];

} KDHELP64, * PKDHELP64;

typedef struct _tagSTACKFRAME64 {
	ADDRESS64   AddrPC;               // program counter
	ADDRESS64   AddrReturn;           // return address
	ADDRESS64   AddrFrame;            // frame pointer
	ADDRESS64   AddrStack;            // stack pointer
	ADDRESS64   AddrBStore;           // backing store pointer
	PVOID       FuncTableEntry;       // pointer to pdata/fpo or NULL
	DWORD64     Params[4];            // possible arguments to the function
	BOOL        Far;                  // WOW far call
	BOOL        Virtual;              // is this a virtual frame?
	DWORD64     Reserved[3];
	KDHELP64    KdHelp;
} STACKFRAME64, * LPSTACKFRAME64;

#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.

typedef
BOOL
(__stdcall* PREAD_PROCESS_MEMORY_ROUTINE64)(
	HANDLE hProcess,
	DWORD64 qwBaseAddress,
	PVOID lpBuffer,
	DWORD nSize,
	LPDWORD lpNumberOfBytesRead
	);

typedef
PVOID
(__stdcall* PFUNCTION_TABLE_ACCESS_ROUTINE64)(
	HANDLE ahProcess,
	DWORD64 AddrBase
	);

typedef
DWORD64
(__stdcall* PGET_MODULE_BASE_ROUTINE64)(
	HANDLE hProcess,
	DWORD64 Address
	);

typedef
DWORD64
(__stdcall* PTRANSLATE_ADDRESS_ROUTINE64)(
	HANDLE hProcess,
	HANDLE hThread,
	LPADDRESS64 lpaddr
	);

typedef struct _IMAGEHLP_SYMBOL64 {
	DWORD   SizeOfStruct;           // set to sizeof(IMAGEHLP_SYMBOL64)
	DWORD64 Address;                // virtual address including dll base address
	DWORD   Size;                   // estimated size of symbol, can be zero
	DWORD   Flags;                  // info about the symbols, see the SYMF defines
	DWORD   MaxNameLength;          // maximum size of symbol name in 'Name'
	CHAR    Name[1];                // symbol name (null terminated string)
} IMAGEHLP_SYMBOL64, * PIMAGEHLP_SYMBOL64;

#define MAX_SYM_NAME            2000

typedef struct _IMAGEHLP_LINE64 {
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID    Key;                    // internal
	DWORD    LineNumber;             // line number in file
	PCHAR    FileName;               // full filename
	DWORD64  Address;                // first instruction of line
} IMAGEHLP_LINE64, * PIMAGEHLP_LINE64;
}

#else
#include <malloc.h>
#include <Windows.h>
#include <rpc.h>
#include <Psapi.h>
#include <DbgHelp.h>
#include <fileapi.h>
#endif

#include <csetjmp>