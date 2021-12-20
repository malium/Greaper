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
	class IApplication : public IInterface
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0xF79C882D, 0x506B4307, 0xBE036194, 0x9F58B3BC };
		static constexpr StringView InterfaceName = "Application"sv;

		DEF_PROP(ApplicationName, String);
		DEF_PROP(CompilationInfo, String);
		DEF_PROP(ApplicationVersion, uint32);
		DEF_PROP(LoadedLibraries, WStringVec);
		
		using OnCloseEvent_t = Event<void>;

		using OnInterfaceActivationEvent_t = Event<IInterface*>;

		virtual ~IApplication()noexcept = default;

		virtual Result<IGreaperLibrary*> RegisterGreaperLibrary(const WStringView& libPath) = 0;

		virtual Result<IGreaperLibrary*> GetGreaperLibrary(const StringView& libraryName) = 0;

		virtual Result<IGreaperLibrary*> GetGreaperLibrary(const Uuid& libraryUUID) = 0;

		virtual EmptyResult UnregisterGreaperLibrary(IGreaperLibrary* library) = 0;

		virtual EmptyResult RegisterInterface(IInterface* interface) = 0;

		virtual EmptyResult UnregisterInterface(IInterface* interface) = 0;

		virtual EmptyResult ActivateInterface(IInterface* interface) = 0;
		
		virtual EmptyResult DeactivateInterface(const Uuid& interfaceUUID) = 0;

		virtual EmptyResult DeactivateInterface(const StringView& interfaceName) = 0;

		virtual Result<IInterface*> GetActiveInterface(const Uuid& interfaceUUID)const = 0;

		virtual Result<IInterface*> GetActiveInterface(const StringView& interfaceName)const = 0;

		virtual Result<IInterface*> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const = 0;

		virtual Result<IInterface*> GetInterface(const StringView& interfaceName, const StringView& libraryName)const = 0;

		virtual Result<IInterface*> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const = 0;

		virtual Result<IInterface*> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const = 0;

		virtual void StartApplication() = 0;

		virtual bool AppHasToStop()const = 0;

		virtual void StopApplication() = 0;

		virtual OnCloseEvent_t*const GetOnCloseEvent() = 0;

		virtual OnInterfaceActivationEvent_t* const GetOnInterfaceActivationEvent() = 0;

		virtual ApplicationNameProp_t* GetApplicationName() = 0;

		virtual CompilationInfoProp_t* GetCompilationInfo() = 0;

		virtual ApplicationVersionProp_t* GetApplicationVersion() = 0;

		virtual LoadedLibrariesProp_t* GetLoadedLibrariesNames() = 0;

		template<class T>
		Result<T*> RegisterGreaperLibraryT(const WStringView& libPath)
		{
			static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to register a GreaperLibrary "
				"but its implementation doesn't derive from IGreaperLibrary.");
			auto res = RegisterGreaperLibrary(libPath);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* lib = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(lib);
		}

		template<class T>
		Result<T*> GetGreaperLibraryT(const StringView& libraryName)
		{
			static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to get a GreaperLibrary "
				"but its implementation doesn't derive from IGreaperLibrary.");
			auto res = GetGreaperLibrary(libraryName);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* lib = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(lib);
		}

		template<class T>
		Result<T*> GetGreaperLibraryT(const Uuid& libraryUUID)
		{
			static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to get a GreaperLibrary "
				"but its implementation doesn't derive from IGreaperLibrary.");
			auto res = GetGreaperLibrary(libraryUUID);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* lib = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(lib);
		}

		template<class T>
		Result<T*> GetActiveInterfaceT(const Uuid& interfaceUUID)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetActiveInterface(interfaceUUID);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* interface = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(interface);
		}

		template<class T>
		Result<T*> GetActiveInterfaceT(const StringView& interfaceName)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetActiveInterface(interfaceName);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* interface = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(interface);
		}

		template<class T>
		Result<T*> GetInterfaceT(const Uuid& interfaceUUID, const Uuid& libraryUUID)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceUUID, libraryUUID);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* interface = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(interface);
		}

		template<class T>
		Result<T*> GetInterfaceT(const StringView& interfaceName, const StringView& libraryName)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceName, libraryName);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* interface = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(interface);
		}

		template<class T>
		Result<T*> GetInterfaceT(const Uuid& interfaceUUID, const StringView& libraryName)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceUUID, libraryName);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* interface = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(interface);
		}

		template<class T>
		Result<T*> GetInterfaceT(const StringView& interfaceName, const Uuid& libraryUUID)const
		{
			static_assert(IsInterface<T>::value, "Trying to get an interface that does not derive from IInterface.");
			auto res = GetInterface(interfaceName, libraryUUID);
			if(res.HasFailed())
				return CopyFailure<T*>(res);
			T* interface = reinterpret_cast<T*>(res.GetValue());
			return CreateResult(interface);
		}
	};
}

#endif /* CORE_I_APPLICATION_H */
