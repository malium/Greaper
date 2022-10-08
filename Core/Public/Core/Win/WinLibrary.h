/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_LIBRARY_H
#define CORE_WIN_LIBRARY_H 1

#include "../PHAL.h"
#include "../Result.h"

namespace greaper
{
	class WinLibrary
	{
	public:
		using LibraryHandle = HMODULE;

		static TResult<LibraryHandle> Load(StringView libraryName)
		{
			auto lib = LoadLibraryA(libraryName.data());
			if (lib != nullptr)
				return Result::CreateSuccess(lib);

			const auto errorCode = GetLastError();
			return Result::CreateFailure<LibraryHandle>(Format("Couldn't load the library '%s', error code " I32_HEX_FMT ".", libraryName.data(), errorCode));
		}

		static TResult<LibraryHandle> Load(WStringView libraryName)
		{
			auto lib = LoadLibraryW(libraryName.data());
			if (lib != nullptr)
				return Result::CreateSuccess(lib);

			const auto errorCode = GetLastError();
			return Result::CreateFailure<LibraryHandle>(Format("Couldn't load the library '%S', error code " I32_HEX_FMT ".", libraryName.data(), errorCode));
		}

		static EmptyResult Unload(LibraryHandle handle)
		{
			if (FreeLibrary(handle) == TRUE)
				return Result::CreateSuccess();

			const auto errorCode = GetLastError();
			return Result::CreateFailure(Format("Couldn't unload a library handle, error code " I32_HEX_FMT ".", errorCode));
		}

		static TResult<FuncPtr> FuncLoad(LibraryHandle handle, StringView procName)
		{
			auto proc = GetProcAddress(handle, procName.data());
			if (proc != nullptr)
				return Result::CreateSuccess(reinterpret_cast<FuncPtr>(proc));

			const auto errorCode = GetLastError();
			return Result::CreateFailure<FuncPtr>(Format("Couldn't obtain the function '%s' from a library, error code " I32_HEX_FMT ".", procName.data(), errorCode));
		}
	};
	using OSLibrary = WinLibrary;
}

#endif /* CORE_IWINLIBRARY_H */
