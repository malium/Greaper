/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_LIBRARY_H
#define CORE_LNX_LIBRARY_H 1

#include "../PHAL.h"
#include "../StringUtils.h"

namespace greaper
{
	class LnxLibrary
	{
	public:
		using LibraryHandle = void*;

		static LibraryHandle Load(StringView libraryName)
		{
			return dlopen(libraryName.data(), RTLD_LAZY | RTLD_LOCAL);
		}

		static LibraryHandle Load(WStringView libraryName)
		{
            const auto libName = StringUtils::FromWIDE(WString{libraryName});
            return dlopen(libName.c_str(), RTLD_LAZY | RTLD_LOCAL);
		}

		static void Unload(LibraryHandle handle)
		{
            dlclose(handle);
		}

		static FuncPtr FuncLoad(LibraryHandle handle, StringView procName)
		{
			const auto proc = dlsym(handle, procName.data());
			/*if (proc == nullptr)
				return nullptr;*/
			return reinterpret_cast<FuncPtr>(proc);
		}
	};
	using OSLibrary = LnxLibrary;
}

#endif /* CORE_LNX_LIBRARY_H */
