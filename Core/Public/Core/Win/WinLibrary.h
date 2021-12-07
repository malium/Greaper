/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_LIBRARY_H
#define CORE_WIN_LIBRARY_H 1

#include "../PHAL.h"

namespace greaper
{
	class WinLibrary
	{
	public:
		using LibraryHandle = HMODULE;

		static LibraryHandle Load(StringView libraryName)
		{
			return LoadLibraryA(libraryName.data());
		}

		static LibraryHandle Load(WStringView libraryName)
		{
			return LoadLibraryW(libraryName.data());
		}

		static void Unload(LibraryHandle handle)
		{
			FreeLibrary(handle);
		}

		static FuncPtr FuncLoad(LibraryHandle handle, StringView procName)
		{
			const auto proc = GetProcAddress(handle, procName.data());
			/*if (proc == nullptr)
				return nullptr;*/
			return reinterpret_cast<FuncPtr>(proc);
		}
	};
	using OSLibrary = WinLibrary;
}

#endif /* CORE_IWINLIBRARY_H */
