/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_APPLICATION_H
#define CORE_I_APPLICATION_H 1

#include "Interface.h"
#include "Result.h"

namespace greaper
{
	/*** The base of Greaper, provides all the necesary to run a real-time application
	*	Providing all the plumbing required to have multiple plugins working at the same time
	*	and allowing interface interchange at run-time, except for itself.
	* 
	*	On a GreaperApplication, all the interfaces are stored here, then you can activate them
	*	as you will, but only one of each type can be active at once.
	*	Active interfaces will receive PreUpdate, FixedUpdate, Update and PostUpdate events each 
	*	update tick.
	*	Also, GreaperLibraries are stored here, so you can access from anywhere other libraries,
	*	their Properties or their interfaces.
	* 
	*	Application has several Properties:
	*	- ApplicationName -> Stores the name of this application, mutable static
	*	- CompilationInfo -> Stores information about the configuration of the compilation, inmutable static
	*	- ApplicationVersion -> Stores the version of the Application, mutable static
	*	- LoadedLibraries -> Stores a list of the loaded libraries, mutable static
	*	- UpdateMaxRate -> Stores the maximum amount of updates per second, mutable external
	*	- FixedUpdateMaxRate -> Stores the amount of fixed updates per second, mutable external
	*/
	class IApplication : public TInterface<IApplication>
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0xF79C882D, 0x506B4307, 0xBE036194, 0x9F58B3BC };
		static constexpr StringView InterfaceName = "Application"sv;

		DEF_PROP(CommandLine, StringVec);
		DEF_PROP(AppInstance, ptruint);
		DEF_PROP(ApplicationName, String);
		DEF_PROP(CompilationInfo, String);
		DEF_PROP(ApplicationVersion, uint32);
		DEF_PROP(LoadedLibraries, WStringVec);
		
		using OnCloseEvent_t = Event<void>;

		using OnInterfaceActivationEvent_t = Event<WInterface>;

		virtual ~IApplication()noexcept = default;

		virtual Result<SPtr<IGreaperLibrary>> RegisterGreaperLibrary(const WStringView& libPath) = 0;

		virtual Result<SPtr<IGreaperLibrary>> GetGreaperLibrary(const StringView& libraryName) = 0;

		virtual Result<SPtr<IGreaperLibrary>> GetGreaperLibrary(const Uuid& libraryUUID) = 0;

		virtual EmptyResult UnregisterGreaperLibrary(SPtr<IGreaperLibrary> library) = 0;

		virtual EmptyResult RegisterInterface(PInterface interface) = 0;

		virtual EmptyResult UnregisterInterface(PInterface interface) = 0;

		virtual EmptyResult ActivateInterface(PInterface interface) = 0;
		
		virtual EmptyResult DeactivateInterface(const Uuid& interfaceUUID) = 0;

		virtual EmptyResult DeactivateInterface(const StringView& interfaceName) = 0;

		virtual Result<PInterface> GetActiveInterface(const Uuid& interfaceUUID)const = 0;

		virtual Result<PInterface> GetActiveInterface(const StringView& interfaceName)const = 0;

		virtual Result<PInterface> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const = 0;

		virtual Result<PInterface> GetInterface(const StringView& interfaceName, const StringView& libraryName)const = 0;

		virtual Result<PInterface> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const = 0;

		virtual Result<PInterface> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const = 0;

		virtual OnInterfaceActivationEvent_t* GetOnInterfaceActivationEvent()const noexcept = 0;

		virtual WPtr<ApplicationNameProp_t> GetApplicationName()const noexcept = 0;

		virtual WPtr<CompilationInfoProp_t> GetCompilationInfo()const noexcept = 0;

		virtual WPtr<ApplicationVersionProp_t> GetApplicationVersion()const noexcept = 0;

		virtual WPtr<LoadedLibrariesProp_t> GetLoadedLibrariesNames()const noexcept = 0;

		virtual WPtr<AppInstanceProp_t> GetAppInstance()const noexcept = 0;

		virtual WPtr<CommandLineProp_t> GetCommandLine()const noexcept = 0;

		template<class T>
		Result<SPtr<T>> RegisterGreaperLibraryT(const WStringView& libPath)
		{
			static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to register a GreaperLibrary "
				"but its implementation doesn't derive from IGreaperLibrary.");
			auto res = RegisterGreaperLibrary(libPath);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto lib = (SPtr<T>)res.GetValue();
			return CreateResult(lib);
		}

		template<class T>
		Result<WPtr<T>> GetGreaperLibraryT(const StringView& libraryName)
		{
			static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to get a GreaperLibrary "
				"but its implementation doesn't derive from IGreaperLibrary.");
			auto res = GetGreaperLibrary(libraryName);
			if(res.HasFailed())
				return CopyFailure<WPtr<T>>(res);
			auto lib = (WPtr<T>)res.GetValue();
			return CreateResult(lib);
		}

		template<class T>
		Result<WPtr<T>> GetGreaperLibraryT(const Uuid& libraryUUID)
		{
			static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to get a GreaperLibrary "
				"but its implementation doesn't derive from IGreaperLibrary.");
			auto res = GetGreaperLibrary(libraryUUID);
			if(res.HasFailed())
				return CopyFailure<WPtr<T>>(res);
			auto lib = (WPtr<T>)res.GetValue();
			return CreateResult(lib);
		}

		template<class T>
		Result<SPtr<T>> GetActiveInterfaceT(const Uuid& interfaceUUID)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetActiveInterface(interfaceUUID);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto interface = (SPtr<T>)res.GetValue();
			return CreateResult(interface);
		}

		template<class T>
		Result<SPtr<T>> GetActiveInterfaceT(const StringView& interfaceName)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetActiveInterface(interfaceName);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto interface = (SPtr<T>)res.GetValue();
			return CreateResult(interface);
		}

		template<class T>
		Result<SPtr<T>> GetInterfaceT(const Uuid& interfaceUUID, const Uuid& libraryUUID)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceUUID, libraryUUID);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto interface = (SPtr<T>)res.GetValue();
			return CreateResult(interface);
		}

		template<class T>
		Result<SPtr<T>> GetInterfaceT(const StringView& interfaceName, const StringView& libraryName)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceName, libraryName);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto interface = (SPtr<T>)res.GetValue();
			return CreateResult(interface);
		}

		template<class T>
		Result<SPtr<T>> GetInterfaceT(const Uuid& interfaceUUID, const StringView& libraryName)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceUUID, libraryName);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto interface = (SPtr<T>)res.GetValue();
			return CreateResult(interface);
		}

		template<class T>
		Result<SPtr<T>> GetInterfaceT(const StringView& interfaceName, const Uuid& libraryUUID)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceName, libraryUUID);
			if(res.HasFailed())
				return CopyFailure<SPtr<T>>(res);
			auto interface = (SPtr<T>)res.GetValue();
			return CreateResult(interface);
		}
	};
	using WApplication = WPtr<IApplication>;
	using PApplication = SPtr<IApplication>;
}

#endif /* CORE_I_APPLICATION_H */
