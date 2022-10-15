/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
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

		void OnActivation(UNUSED const SPtr<IInterface>& oldDefault)noexcept override;

		void OnDeactivation(UNUSED const SPtr<IInterface>& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		void InitSerialization()noexcept override;

		void DeinitSerialization()noexcept override;

		TResult<SPtr<IGreaperLibrary>> RegisterGreaperLibrary(const WStringView& libPath)noexcept override;

		TResult<SPtr<IGreaperLibrary>> GetGreaperLibrary(const StringView& libraryName)const noexcept override;

		TResult<SPtr<IGreaperLibrary>> GetGreaperLibrary(const Uuid& libraryUUID)const noexcept override;

		EmptyResult UnregisterGreaperLibrary(SPtr<IGreaperLibrary> library)noexcept override;

		EmptyResult RegisterInterface(const PInterface& interface)noexcept override;

		EmptyResult UnregisterInterface(const PInterface& interface)noexcept override;

		EmptyResult ActivateInterface(const PInterface& interface)noexcept override;

		EmptyResult DeactivateInterface(const Uuid& interfaceUUID)noexcept override;

		EmptyResult DeactivateInterface(const StringView& interfaceName)noexcept override;

		TResult<PInterface> GetActiveInterface(const Uuid& interfaceUUID)const noexcept override;

		TResult<PInterface> GetActiveInterface(const StringView& interfaceName)const noexcept override;

		TResult<PInterface> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const noexcept override;

		TResult<PInterface> GetInterface(const StringView& interfaceName, const StringView& libraryName)const noexcept override;

		TResult<PInterface> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const noexcept override;

		TResult<PInterface> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const noexcept override;

		OnInterfaceActivationEvent_t* GetOnInterfaceActivationEvent()const noexcept override { return &m_OnInterfaceActivation; }

		WPtr<ApplicationNameProp_t> GetApplicationName()const noexcept override { return (WPtr<ApplicationNameProp_t>)m_Properties[(std::size_t)ApplicationName]; }

		WPtr<CompilationInfoProp_t> GetCompilationInfo()const noexcept override { return (WPtr<CompilationInfoProp_t>)m_Properties[(std::size_t)CompilationInfo]; }

		WPtr<ApplicationVersionProp_t> GetApplicationVersion()const noexcept override { return (WPtr<ApplicationVersionProp_t>)m_Properties[(std::size_t)ApplicationVersion]; }

		WPtr<LoadedLibrariesProp_t> GetLoadedLibrariesNames()const noexcept override { return (WPtr<LoadedLibrariesProp_t>)m_Properties[(std::size_t)LoadedLibraries]; }

		WPtr<CommandLineProp_t> GetCommandLine()const noexcept override { return (WPtr<CommandLineProp_t>)m_Properties[(std::size_t)CommandLine]; }

		WPtr<AppInstanceProp_t> GetAppInstance()const noexcept override { return (WPtr<AppInstanceProp_t>)m_Properties[(std::size_t)AppInstance]; }

		[[nodiscard]] Vector<SPtr<IGreaperLibrary>> GetRegisteredLibrariesCopy()const noexcept override
		{
			Vector<SPtr<IGreaperLibrary>> vec{ m_Libraries.size()};
			for (const LibInfo& lib : m_Libraries)
				vec.push_back(lib.Lib);
			
			return vec;
		}

		[[nodiscard]] Vector<SPtr<IInterface>> GetActiveInterfacesCopy()const noexcept override { LOCK(m_ActiveMutex); return Vector<SPtr<IInterface>>{m_ActiveInterfaces}; }
	};
}

#endif /* CORE_APPLICATION_H */
