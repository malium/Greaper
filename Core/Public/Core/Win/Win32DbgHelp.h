/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN32_DBGHELP_H
#define CORE_WIN32_DBGHELP_H 1

#include "Win32Uuid.h"

#if WIN32_USE_GREAPER_HEADERS

extern "C" {

#pragma pack(push,8)

#define SYMOPT_CASE_INSENSITIVE          0x00000001
#define SYMOPT_UNDNAME                   0x00000002
#define SYMOPT_DEFERRED_LOADS            0x00000004
#define SYMOPT_NO_CPP                    0x00000008
#define SYMOPT_LOAD_LINES                0x00000010
#define SYMOPT_OMAP_FIND_NEAREST         0x00000020
#define SYMOPT_LOAD_ANYTHING             0x00000040
#define SYMOPT_IGNORE_CVREC              0x00000080
#define SYMOPT_NO_UNQUALIFIED_LOADS      0x00000100
#define SYMOPT_FAIL_CRITICAL_ERRORS      0x00000200
#define SYMOPT_EXACT_SYMBOLS             0x00000400
#define SYMOPT_ALLOW_ABSOLUTE_SYMBOLS    0x00000800
#define SYMOPT_IGNORE_NT_SYMPATH         0x00001000
#define SYMOPT_INCLUDE_32BIT_MODULES     0x00002000
#define SYMOPT_PUBLICS_ONLY              0x00004000
#define SYMOPT_NO_PUBLICS                0x00008000
#define SYMOPT_AUTO_PUBLICS              0x00010000
#define SYMOPT_NO_IMAGE_SEARCH           0x00020000
#define SYMOPT_SECURE                    0x00040000
#define SYMOPT_NO_PROMPTS                0x00080000
#define SYMOPT_OVERWRITE                 0x00100000
#define SYMOPT_IGNORE_IMAGEDIR           0x00200000
#define SYMOPT_FLAT_DIRECTORY            0x00400000
#define SYMOPT_FAVOR_COMPRESSED          0x00800000
#define SYMOPT_ALLOW_ZERO_ADDRESS        0x01000000
#define SYMOPT_DISABLE_SYMSRV_AUTODETECT 0x02000000
#define SYMOPT_READONLY_CACHE            0x04000000
#define SYMOPT_SYMPATH_LAST              0x08000000
#define SYMOPT_DISABLE_FAST_SYMBOLS      0x10000000
#define SYMOPT_DISABLE_SYMSRV_TIMEOUT    0x20000000
#define SYMOPT_DISABLE_SRVSTAR_ON_STARTUP 0x40000000
#define SYMOPT_DEBUG                     0x80000000

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

typedef struct _IMAGEHLP_SYMBOL64 {
	DWORD   SizeOfStruct;           // set to sizeof(IMAGEHLP_SYMBOL64)
	DWORD64 Address;                // virtual address including dll base address
	DWORD   Size;                   // estimated size of symbol, can be zero
	DWORD   Flags;                  // info about the symbols, see the SYMF defines
	DWORD   MaxNameLength;          // maximum size of symbol name in 'Name'
	CHAR    Name[1];                // symbol name (null terminated string)
} IMAGEHLP_SYMBOL64, * PIMAGEHLP_SYMBOL64;

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

typedef struct _IMAGEHLP_LINE64 {
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID    Key;                    // internal
	DWORD    LineNumber;             // line number in file
	PCHAR    FileName;               // full filename
	DWORD64  Address;                // first instruction of line
} IMAGEHLP_LINE64, * PIMAGEHLP_LINE64;

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

#define MAX_SYM_NAME            2000

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

#pragma pack(pop)

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

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
	DWORD ThreadId;
	PEXCEPTION_POINTERS ExceptionPointers;
	BOOL ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION, * PMINIDUMP_EXCEPTION_INFORMATION;

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

#pragma warning(push)
#pragma warning(disable : 4201)
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

}
#else
#include <DbgHelp.h>
#endif

#endif /* CORE_WIN32_DBGHELP_H */