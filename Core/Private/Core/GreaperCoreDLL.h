/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#ifndef GREAPER_CORE_DLL_H
#define GREAPER_CORE_DLL_H 1

#pragma once

#if GREAPER_CORE_DLL

#include "ImplPrerequisites.h"
#include <Core/IGreaperLibrary.h>
#include <Core/ILogManager.h>

namespace greaper
{
	class IInterface;
}

namespace greaper::core
{
	class GreaperCoreLibrary : public IGreaperLibrary
	{
		EmptyResult RegisterProperty(SPtr<IProperty> property)override;
		SPtr<Application> m_Application;
		mutable PLibrary m_Library;
		PLogManager m_LogManager;
		bool m_LogManagerActivated = false;
		IApplication::OnInterfaceActivationEvent_t::HandlerType m_OnNewLogManager;
		EventHandler<bool> m_OnLogActivation;
		
		Vector<greaper::LogData> m_InitLogs;
		Vector<PInterface> m_Managers;
		UnorderedMap<StringView, sizet> m_PropertyMap;
		Vector<PIProperty> m_Properties;

		void DumpLogsToLogManager();
		void OnNewLogManager(WInterface nlog);
		void OnLogActivation(bool activated);

	public:
		static constexpr Uuid LibraryUUID = Uuid{ 0xDAC703FC, 0x16BD4F59, 0xB62D28ED, 0x3C9DE087 };
		static constexpr StringView LibraryName = StringView{ "GreaperCoreImpl" };
		
		GreaperCoreLibrary() = default;

		void InitLibrary(PLibrary lib, WPtr<IApplication> app)override;

		void InitManagers()override;

		void InitProperties()override;

		void InitReflection()override;

		void DeinitReflection()override;

		void DeinitProperties()override;

		void DeinitManagers()override;

		void DeinitLibrary()override;

		int GetLibraryVersion()const override { return 0; }

		const Uuid& GetLibraryUuid()const override { return LibraryUUID; }

		const StringView& GetLibraryName()const override { return LibraryName; }

		WPtr<IApplication> GetApplication()const override { return m_Application; }

		WPtr<Library> GetOSLibrary()const override { return m_Library; }

		CRange<SPtr<IProperty>> GetPropeties()const override;

		Result<WPtr<IProperty>> GetProperty(const StringView& name)const override;

		void LogVerbose(const String& message)override;

		void Log(const String& message)override;

		void LogWarning(const String& message)override;

		void LogError(const String& message)override;

		void LogCritical(const String& message)override;

		CRange<SPtr<IInterface>> GetManagers() const override;
	};
}


#endif // GREAPER_CORE_DLL

#endif /* GREAPER_CORE_DLL_H */