/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H 1

#include "ImplPrerequisites.h"
#include <Core/IApplication.h>
#include <Core/Event.h>

namespace greaper::core
{
	class Application final : public IApplication
	{
		enum PropertiesIndices
		{
			ApplicationName,
			ApplicationVersion,
			CompilationInfo,
			LoadedLibraries,

			COUNT
		};

		IGreaperLibrary* m_Library;
		OnCloseEvent_t m_OnClose;
		OnInterfaceActivationEvent_t m_OnInterfaceActivation;
		InitializationEvt_t m_OnInitialization;
		ActivationEvt_t m_OnActivation;

		Vector<IProperty*> m_Properties;

		bool m_IsActive;
		bool m_IsInitialized;
		bool m_HasToStop;

		struct LibInfo
		{
			IGreaperLibrary* Lib;
			UnorderedMap<StringView, size_t> IntefaceNameMap;
			UnorderedMap<Uuid, size_t> InterfaceUuidMap;
			Vector<IInterface*> Interfaces;
		};

		UnorderedMap<StringView, size_t> m_LibraryNameMap;
		UnorderedMap<Uuid, size_t> m_LibraryUuidMap;
		Vector<LibInfo> m_Libraries;

		mutable Mutex m_ActiveMutex;
		UnorderedMap<StringView, size_t> m_ActiveInterfaceNameMap;
		UnorderedMap<Uuid, size_t> m_ActiveInterfaceUuidMap;
		Vector<IInterface*> m_ActiveInterfaces;

		mutable Mutex m_ToAddMutex;
		Vector<IInterface*> m_InterfacesToAdd;

		void AddGreaperLibrary(IGreaperLibrary* library);
		void LoadConfigLibraries();

	public:
		Application();
		~Application();

		const Uuid& GetInterfaceUUID()const override { return InterfaceUUID; }

		const StringView& GetInterfaceName()const override { return InterfaceName; }

		IGreaperLibrary* GetLibrary()const override { return m_Library; }

		void Initialize(IGreaperLibrary* library)override;

		void Deinitialize()override;

		void OnActivate()override;

		void OnDeactivate()override;

		void InitProperties()override;

		void DeinitProperties()override;

		bool IsActive()const override { return m_IsActive; }

		bool IsInitialized()const override { return m_IsInitialized; }

		void PreUpdate()override;

		void Update()override;

		void PostUpdate()override;

		void FixedUpdate()override;

		InitializationEvt_t* const GetInitializationEvent()override { return &m_OnInitialization; }

		ActivationEvt_t* const GetActivationEvent()override { return &m_OnActivation; }

		/*void SetConfig(ApplicationConfig config)override;

		const ApplicationConfig& GetConfig()const override { return m_Config; }*/

		void OnChangingDefault(IInterface* newDefault)override
		{
			UNUSED(newDefault);
			Break("Cannot change the full application at runtime.");
		}

		ChangingDefaultEvt_t* const GetChangingDefaultEvent() { return nullptr; }

		Result<IGreaperLibrary*> RegisterGreaperLibrary(const WStringView& libPath)override;

		Result<IGreaperLibrary*> GetGreaperLibrary(const StringView& libraryName)override;

		Result<IGreaperLibrary*> GetGreaperLibrary(const Uuid& libraryUUID)override;

		EmptyResult UnregisterGreaperLibrary(IGreaperLibrary* library)override;

		EmptyResult RegisterInterface(IInterface* interface)override;

		EmptyResult UnregisterInterface(IInterface* interface)override;

		EmptyResult ActivateInterface(IInterface* interface)override;

		EmptyResult DeactivateInterface(const Uuid& interfaceUUID)override;

		EmptyResult DeactivateInterface(const StringView& interfaceName)override;

		Result<IInterface*> GetActiveInterface(const Uuid& interfaceUUID)const  override;

		Result<IInterface*> GetActiveInterface(const StringView& interfaceName)const override;

		Result<IInterface*> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const override;

		Result<IInterface*> GetInterface(const StringView& interfaceName, const StringView& libraryName)const override;

		Result<IInterface*> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const override;

		Result<IInterface*> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const override;

		void StartApplication()override;

		bool AppHasToStop()const override { return m_HasToStop; }

		void StopApplication()override;

		OnCloseEvent_t* const GetOnCloseEvent()override { return &m_OnClose; }

		OnInterfaceActivationEvent_t* const GetOnInterfaceActivationEvent() { return &m_OnInterfaceActivation; }

		ApplicationNameProp_t* GetApplicationName()override { return reinterpret_cast<ApplicationNameProp_t*>(m_Properties[(sizet)ApplicationName]); }

		CompilationInfoProp_t* GetCompilationInfo()override { return reinterpret_cast<CompilationInfoProp_t*>(m_Properties[(sizet)CompilationInfo]); }

		ApplicationVersionProp_t* GetApplicationVersion()override { return reinterpret_cast<ApplicationVersionProp_t*>(m_Properties[(sizet)ApplicationVersion]); }

		LoadedLibrariesProp_t* GetLoadedLibrariesNames()override { return reinterpret_cast<LoadedLibrariesProp_t*>(m_Properties[(sizet)LoadedLibraries]); }

		CRange<IProperty*> GetProperties() const override { return CreateRange(m_Properties); }
};
}

#endif /* CORE_APPLICATION_H */
