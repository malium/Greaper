/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#ifndef GREAPER_CORE_DLL_H
#define GREAPER_CORE_DLL_H 1

#pragma once

#if GREAPER_CORE_DLL

#include "ImplPrerequisites.h"
#include <Core/IGreaperLibrary.h>
#include <Core/ILogManager.h>

namespace greaper::core
{
	class GreaperCoreLibrary : public TGreaperLibrary<GreaperCoreLibrary>
	{
	public:
		static constexpr Uuid LibraryUUID = Uuid{ 0xDAC703FC, 0x16BD4F59, 0xB62D28ED, 0x3C9DE087 };
		static constexpr StringView LibraryName = "GreaperCore"sv;
		
		GreaperCoreLibrary() = default;

		void Initialize()noexcept override;

		void InitManagers()noexcept override;

		void InitProperties()noexcept override;

		void InitReflection()noexcept override;

		void DeinitReflection()noexcept override;

		void DeinitProperties()noexcept override;

		void DeinitManagers()noexcept override;

		void Deinitialize()noexcept override;

		uint32 GetLibraryVersion()const noexcept override { return 0; }
	};
}

#endif // GREAPER_CORE_DLL

#endif /* GREAPER_CORE_DLL_H */