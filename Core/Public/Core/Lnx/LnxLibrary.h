/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_LIBRARY_H
#define CORE_LNX_LIBRARY_H 1

#include "../PHAL.h"
#include "../StringUtils.h"
#include "../Result.h"

namespace greaper
{
	class LnxLibrary
	{
	public:
		using LibraryHandle = void*;

		static TResult<LibraryHandle> Load(StringView libraryName)
		{
			auto lib = dlopen(libraryName.data(), RTLD_LAZY | RTLD_LOCAL);
			if (lib != nullptr)
				return Result::CreateSuccess(lib);

			return Result::CreateFailure<LibraryHandle>(Format("Couldn't load the library '%s', error: '%s'.", libraryName.data(), dlerror()));
		}

		static TResult<LibraryHandle> Load(WStringView libraryName)
		{
			const auto libName = StringUtils::FromWIDE(libraryName);
			return Load(libName);
		}

		static EmptyResult Unload(LibraryHandle handle)
		{
			if (dlclose(handle) == 0)
				return Result::CreateSuccess();
			
			return Result::CreateFailure(Format("Couldn't unload a library, error: '%s'.", dlerror()));
		}

		static TResult<FuncPtr> FuncLoad(LibraryHandle handle, StringView procName)
		{
			// Clear old error conditions
			dlerror();

			// obtain the procedure
			auto proc = dlsym(handle, procName.data());

			// obtain the new error message
			auto error = dlerror();
			
			if (error == nullptr) // no error
				return Result::CreateSuccess(reinterpret_cast<FuncPtr>(proc));

			// error condition triggered
			return Result::CreateFailure<FuncPtr>(Format("Couldn't obtain the function '%s' from a library, error: '%s'.", procName.data(), error));
		}
	};
	using OSLibrary = LnxLibrary;
}

#endif /* CORE_LNX_LIBRARY_H */
