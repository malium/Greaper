/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_PLATFORM_H
#define CORE_WIN_PLATFORM_H 1

#include "../Library.h"
#include "Win32DbgHelp.h"
#include "Win32PSAPI.h"
#include "Win32Concurrency.h"
#include <filesystem>
#include <array>

namespace greaper
{
	struct WinOSPlatform
	{
		static void Sleep(uint32 millis) noexcept;

		static String GetExceptionMessage(PEXCEPTION_RECORD record);

		static void WriteMiniDump(std::filesystem::path path, PEXCEPTION_POINTERS exceptionData);

		static String GetStackTrace();

		static void PerThreadSEHInit();

	private:
		static constexpr sizet MAX_STACKTRACE_DEPTH = 200;
		static constexpr sizet MAX_STACKTRACE_NAME_LENGTH = 1024;

		static inline Mutex m_Mutex;

		static inline Library PSAPI;

		typedef BOOL(WINAPI* EnumProcessModules_t)(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
		typedef DWORD(WINAPI* GetModuleBaseName_t)(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize);
		typedef DWORD(WINAPI* GetModuleFileNameEx_t)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
		typedef BOOL(WINAPI* GetModuleInformation_t)(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);

		static inline EnumProcessModules_t EnumProcessModules = nullptr;
		static inline GetModuleBaseName_t GetModuleBaseName = nullptr;
		static inline GetModuleFileNameEx_t GetModuleFileNameEx = nullptr;
		static inline GetModuleInformation_t GetModuleInformation = nullptr;

		static inline Library DbgHelp;

		typedef BOOL(WINAPI* MiniDumpWriteDump_t)(HANDLE hProcess, DWORD ProcessId, HANDLE hFile, MINIDUMP_TYPE DumpType,
			PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
			PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

		typedef DWORD(STDCALL* SymGetOptions_t)();
		typedef DWORD(STDCALL* SymSetOptions_t)(DWORD SymOptions);
		typedef BOOL(STDCALL* SymInitialize_t)(HANDLE hProcess, PCSTR UserSearchPath, BOOL fInvadeProcess);
		typedef BOOL(STDCALL* SymSetSearchPath_t)(HANDLE hProcess, PCSTR SearchPath);
		typedef DWORD64(STDCALL* SymLoadModule64_t)(HANDLE hProcess, HANDLE hFile, PCSTR ImageName, PCSTR ModuleName, DWORD64 BaseOfDll, DWORD SizeOfDll);
		typedef BOOL(STDCALL* SymGetModuleInfo64_t)(HANDLE hProcess, DWORD64 qwAddr, PIMAGEHLP_MODULE64 ModuleInfo);
		typedef BOOL(STDCALL* SymCleanup_t)(HANDLE hProcess);
		typedef BOOL(STDCALL* StackWalk64_t)(DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
		typedef PVOID(STDCALL* SymFunctionTableAccess64_t)(HANDLE hProcess, DWORD64 AddrBase);
		typedef DWORD64(STDCALL* SymGetModuleBase64_t)(HANDLE hProcess, DWORD64 qwAddr);
		typedef BOOL(STDCALL* SymGetSymFromAddr64_t)(HANDLE hProcess, DWORD64 qwAddr, PDWORD64 pdwDisplacement, PIMAGEHLP_SYMBOL64 Symbol);
		typedef BOOL(STDCALL* SymGetLineFromAddr64_t)(HANDLE hProcess, DWORD64 qwAddr, PDWORD pdwDisplacement, PIMAGEHLP_LINE64 Line64);

		static inline MiniDumpWriteDump_t MiniDumpWriteDump = nullptr;
		static inline SymGetOptions_t SymGetOptions = nullptr;
		static inline SymSetOptions_t SymSetOptions = nullptr;
		static inline SymInitialize_t SymInitialize = nullptr;
		static inline SymSetSearchPath_t SymSetSearchPath = nullptr;
		static inline SymLoadModule64_t SymLoadModule64 = nullptr;
		static inline SymGetModuleInfo64_t SymGetModuleInfo64 = nullptr;
		static inline SymCleanup_t SymCleanup = nullptr;
		static inline StackWalk64_t StackWalk64 = nullptr;
		static inline SymFunctionTableAccess64_t SymFunctionTableAccess64 = nullptr;
		static inline SymGetModuleBase64_t SymGetModuleBase64 = nullptr;
		static inline SymGetSymFromAddr64_t SymGetSymFromAddr64 = nullptr;
		static inline SymGetLineFromAddr64_t SymGetLineFromAddr64 = nullptr;

		static void InitPSAPI();

		static void InitDbgHelp();

		static void LoadSymbols();

		static void UnloadSymbols();

		static uint32 GetRawStackTrace(CONTEXT context, std::array<DWORD64, MAX_STACKTRACE_DEPTH>& stackTrace);

		static String GetStackTrace(CONTEXT context, uint32 skip = 0);

		static void SETranslatorFn(unsigned int se, _EXCEPTION_POINTERS* exceptPointers);

		class SEHException : public std::exception
		{
		public:
			INLINE SEHException()noexcept
				:m_SE(0)
			{

			}
			INLINE SEHException(uint32 se, PEXCEPTION_POINTERS exceptPointers)noexcept
				:m_SE(se)
			{
				if (exceptPointers != nullptr)
					m_Message.assign(WinOSPlatform::GetExceptionMessage(exceptPointers->ExceptionRecord));
			}
			INLINE ~SEHException()noexcept = default;

			INLINE char const* what()const override { return m_Message.c_str(); }

			INLINE uint32 GetSE()const noexcept { return m_SE; }
		private:
			uint32 m_SE;
			String m_Message;
		};
	};
	using OSPlatform = WinOSPlatform;
}

#include "WinPlatform.inl"

#endif /* CORE_WIN_PLATFORM_H */