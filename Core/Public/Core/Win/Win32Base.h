/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN32_BASE_H
#define CORE_WIN32_BASE_H 1

#define WIN32_USE_GREAPER_HEADERS 1

#if WIN32_USE_GREAPER_HEADERS
extern "C" {
#define DECLSPEC_IMPORT __declspec(dllimport)
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

#if ARCHITECTURE_X64
#define UNALIGNED __unaligned
#define UNALIGNED64 __unaligned
#else
#define UNALIGNED
#define UNALIGNED64
#endif

#define CONST               const
#define far                 
#define near                
#define FAR                 
#define NEAR         
#define MAX_PATH          260

#define CONST               const
typedef int                 INT;
typedef int                 BOOL;
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

#if ARCHITECTURE_X64
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
#define __int3264   __int64
#else
typedef int INT_PTR, * PINT_PTR;
typedef unsigned int UINT_PTR, * PUINT_PTR;

typedef long LONG_PTR, * PLONG_PTR;
typedef unsigned long ULONG_PTR, * PULONG_PTR;

#define __int3264   __int32
#endif
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

typedef struct _LIST_ENTRY {
	struct _LIST_ENTRY* Flink;
	struct _LIST_ENTRY* Blink;
} LIST_ENTRY, * PLIST_ENTRY, * RESTRICTED_POINTER PRLIST_ENTRY;

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

#if ARCHITECTURE_X64

#pragma warning(push)
#pragma warning(disable : 4201)
typedef struct DECLSPEC_ALIGN(16) DECLSPEC_NOINITALL _CONTEXT {

	DWORD64 P1Home;
	DWORD64 P2Home;
	DWORD64 P3Home;
	DWORD64 P4Home;
	DWORD64 P5Home;
	DWORD64 P6Home;
	DWORD ContextFlags;
	DWORD MxCsr;
	WORD   SegCs;
	WORD   SegDs;
	WORD   SegEs;
	WORD   SegFs;
	WORD   SegGs;
	WORD   SegSs;
	DWORD EFlags;
	DWORD64 Dr0;
	DWORD64 Dr1;
	DWORD64 Dr2;
	DWORD64 Dr3;
	DWORD64 Dr6;
	DWORD64 Dr7;
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
	DWORD64 Rip;
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
		};
	};
	M128A VectorRegister[26];
	DWORD64 VectorControl;
	DWORD64 DebugControl;
	DWORD64 LastBranchToRip;
	DWORD64 LastBranchFromRip;
	DWORD64 LastExceptionToRip;
	DWORD64 LastExceptionFromRip;
} CONTEXT, * PCONTEXT;
#pragma warning(pop)
#else
#define SIZE_OF_80387_REGISTERS      80
typedef struct _FLOATING_SAVE_AREA {
	DWORD   ControlWord;
	DWORD   StatusWord;
	DWORD   TagWord;
	DWORD   ErrorOffset;
	DWORD   ErrorSelector;
	DWORD   DataOffset;
	DWORD   DataSelector;
	BYTE    RegisterArea[SIZE_OF_80387_REGISTERS];
	DWORD   Spare0;
} FLOATING_SAVE_AREA;

typedef FLOATING_SAVE_AREA* PFLOATING_SAVE_AREA;

#define MAXIMUM_SUPPORTED_EXTENSION     512

#pragma pack(push, 4)
typedef struct DECLSPEC_NOINITALL _CONTEXT {
	DWORD ContextFlags;
	DWORD   Dr0;
	DWORD   Dr1;
	DWORD   Dr2;
	DWORD   Dr3;
	DWORD   Dr6;
	DWORD   Dr7;
	FLOATING_SAVE_AREA FloatSave;
	DWORD   SegGs;
	DWORD   SegFs;
	DWORD   SegEs;
	DWORD   SegDs;
	DWORD   Edi;
	DWORD   Esi;
	DWORD   Ebx;
	DWORD   Edx;
	DWORD   Ecx;
	DWORD   Eax;
	DWORD   Ebp;
	DWORD   Eip;
	DWORD   SegCs;
	DWORD   EFlags;
	DWORD   Esp;
	DWORD   SegSs;
	BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
} CONTEXT;
typedef CONTEXT* PCONTEXT;
#pragma pack(pop)
#endif

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

#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
}

#else

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

#include <malloc.h>
#include <Windows.h>
#include <shellapi.h>
#include <fileapi.h>
#endif

#include <csetjmp>

#endif /* CORE_WIN32_BASE_H */