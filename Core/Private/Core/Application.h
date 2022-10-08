/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H 1

#include "ImplPrerequisites.h"
#include <Core/IGreaperLibrary.h>
#include <Core/IApplication.h>
#include <Core/Property.h>

namespace greaper::core
{
	class Application final : public IApplication
	{
		enum PropertiesIndices
		{
			AppInstance,
			CommandLine,
			ApplicationName,
			ApplicationVersion,
			CompilationInfo,
			LoadedLibraries,

			COUNT
		};

		OnCloseEvent_t m_OnClose;
		mutable OnInterfaceActivationEvent_t m_OnInterfaceActivation;

		struct LibInfo
		{
			SPtr<IGreaperLibrary> Lib;
			UnorderedMap<StringView, sizet> IntefaceNameMap;
			UnorderedMap<Uuid, sizet> InterfaceUuidMap;
			Vector<PInterface> Interfaces;
		};

		UnorderedMap<StringView, sizet> m_LibraryNameMap;
		UnorderedMap<Uuid, sizet> m_LibraryUuidMap;
		Vector<LibInfo> m_Libraries;

		mutable RecursiveMutex m_ActiveMutex;
		UnorderedMap<StringView, sizet> m_ActiveInterfaceNameMap;
		UnorderedMap<Uuid, sizet> m_ActiveInterfaceUuidMap;
		Vector<PInterface> m_ActiveInterfaces;

		Vector<PInterface> m_InterfaceToChange;
		Vector<PInterface> m_InterfacesToRemove;
		Vector<PInterface> m_InterfacesToAdd;

		EmptyResult RegisterGreaperLibrary(const SPtr<IGreaperLibrary>& gLib);

		void UpdateActiveInterfaceList()noexcept;

	public:
		Application();
		~Application()noexcept;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(const SPtr<IInterface>& oldDefault)noexcept override;

		void OnDeactivation(const SPtr<IInterface>& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		void InitSerialization()noexcept override;

		void DeinitSerialization()noexcept override;

		TResult<SPtr<IGreaperLibrary>> RegisterGreaperLibrary(const WStringView& libPath)override;

		TResult<SPtr<IGreaperLibrary>> GetGreaperLibrary(const StringView& libraryName)override;

		TResult<SPtr<IGreaperLibrary>> GetGreaperLibrary(const Uuid& libraryUUID)override;

		EmptyResult UnregisterGreaperLibrary(SPtr<IGreaperLibrary> library)override;

		EmptyResult RegisterInterface(const PInterface& interface)override;

		EmptyResult UnregisterInterface(const PInterface& interface)override;

		EmptyResult ActivateInterface(const PInterface& interface)override;

		EmptyResult DeactivateInterface(const Uuid& interfaceUUID)override;

		EmptyResult DeactivateInterface(const StringView& interfaceName)override;

		TResult<PInterface> GetActiveInterface(const Uuid& interfaceUUID)const  override;

		TResult<PInterface> GetActiveInterface(const StringView& interfaceName)const override;

		TResult<PInterface> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const override;

		TResult<PInterface> GetInterface(const StringView& interfaceName, const StringView& libraryName)const override;

		TResult<PInterface> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const override;

		TResult<PInterface> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const override;

		OnInterfaceActivationEvent_t* GetOnInterfaceActivationEvent()const noexcept override { return &m_OnInterfaceActivation; }

		WPtr<ApplicationNameProp_t> GetApplicationName()const noexcept override { return (WPtr<ApplicationNameProp_t>)m_Properties[(sizet)ApplicationName]; }

		WPtr<CompilationInfoProp_t> GetCompilationInfo()const noexcept override { return (WPtr<CompilationInfoProp_t>)m_Properties[(sizet)CompilationInfo]; }

		WPtr<ApplicationVersionProp_t> GetApplicationVersion()const noexcept override { return (WPtr<ApplicationVersionProp_t>)m_Properties[(sizet)ApplicationVersion]; }

		WPtr<LoadedLibrariesProp_t> GetLoadedLibrariesNames()const noexcept override { return (WPtr<LoadedLibrariesProp_t>)m_Properties[(sizet)LoadedLibraries]; }

		WPtr<CommandLineProp_t> GetCommandLine()const noexcept override { return (WPtr<CommandLineProp_t>)m_Properties[(sizet)CommandLine]; }

		WPtr<AppInstanceProp_t> GetAppInstance()const noexcept override { return (WPtr<AppInstanceProp_t>)m_Properties[(sizet)AppInstance]; }
	};
}

#endif /* CORE_APPLICATION_H */
