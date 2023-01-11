/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#ifndef GREAPER_CORE_DLL_H
#define GREAPER_CORE_DLL_H 1

#pragma once

#if GREAPER_CORE_DLL

#include "ImplPrerequisites.h"
#include <Core/IGreaperLibrary.h>

namespace greaper::core
{
	class GreaperCoreLibrary : public TGreaperLibrary<GreaperCoreLibrary>
	{
	public:
		static constexpr Uuid LibraryUUID = Uuid{ 0xDAC703FC, 0x16BD4F59, 0xB62D28ED, 0x3C9DE087 };
		static constexpr StringView LibraryName = "GreaperCore"sv;
		
		GreaperCoreLibrary() = default;

		void AddManagers()noexcept override;

		void AddProperties()noexcept override;

		void RemoveProperties()noexcept override;

		void RemoveManagers()noexcept override;

		void Deinitialize()noexcept override;

		uint32 GetLibraryVersion()const noexcept override { return GREAPER_CORE_VERSION; }
	};
}

#endif // GREAPER_CORE_DLL

#endif /* GREAPER_CORE_DLL_H */