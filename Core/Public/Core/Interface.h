/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H 1

#include "Uuid.h"

namespace greaper
{
	/**
	 * @brief Base class of all manager and factories
	 * 
	 * All interfaces must have a link to their GreaperLibrary.
	 * De/Initialize is called when the interface is just created, as 
	 * constructors/destructors won't be used, but they will be called.
	 * De/Activate is called once the interface is maked the default of its type,
	 * we are planning a way to migrate the resources from the previous interface
	 * of its type to the one is getting activated, so we will be able to switch
	 * interfaces at runtime, allowing to switch to another rendering engine for 
	 * example.
	 * All interfaces will receive the Pre/Post/Fixed/Update event calls, and
	 * IApplication is the resposible to trigger those as also the responsible 
	 * to manage the interfaces.
	 */
	class IInterface
	{
	public:
		using InitializationEvt_t = Event<bool>;
		using ActivationEvt_t = Event<bool>;
		using ChangingDefaultEvt_t = Event<IInterface, IInterface>;

		virtual ~IInterface()noexcept = default;

		static constexpr Uuid InterfaceUUID = Uuid{  };
		static constexpr StringView InterfaceName = StringView{ "INullInterface" };

		virtual const Uuid& GetInterfaceUUID()const = 0;
		
		virtual const StringView& GetInterfaceName()const = 0;
		
		virtual IGreaperLibrary* GetLibrary()const = 0;

		virtual void Initialize(IGreaperLibrary* library) = 0;

		virtual void Deinitialize() = 0;

		virtual void OnActivate() = 0;

		virtual void OnDeactivate() = 0;

		virtual void InitProperties() = 0;

		virtual void DeinitProperties() = 0;

		virtual bool IsActive()const = 0;
		
		virtual bool IsInitialized()const = 0;

		/*virtual void PreUpdate() = 0;

		virtual void Update() = 0;

		virtual void PostUpdate() = 0;

		virtual void FixedUpdate() = 0;*/

		virtual InitializationEvt_t* const GetInitializationEvent() = 0;

		virtual ActivationEvt_t* const GetActivationEvent() = 0;

		virtual void OnChangingDefault(IInterface* newDefault) = 0;

		virtual ChangingDefaultEvt_t* const GetChangingDefaultEvent() = 0;

		virtual CRange<IProperty*> GetProperties()const = 0;
	};

	template<class T>
	struct IsInterface
	{
		static constexpr bool value = std::is_base_of_v<IInterface, T>;
	};
	template<class T>
	struct ValidInterface
	{
		static_assert(IsInterface<T>::value, "Trying to validate an Interface that does not derive from IInterface.");
		static constexpr bool UUIDValid = T::InterfaceUUID != IInterface::InterfaceUUID;
		static constexpr bool NameValid = T::InterfaceName != IInterface::InterfaceName;

		static constexpr bool Valid = UUIDValid && NameValid;
	};
	template<class T1, class T2>
	struct CompareInterfaces
	{
		static_assert(IsInterface<T1>::value, "Trying to compare two Interfaces but T1 does not derive from IInterface.");
		static_assert(IsInterface<T2>::value, "Trying to compare two Interfaces but T2 does not derive from IInterface.");

		static constexpr bool SameUUID = T1::InterfaceUUID == T2::InterfaceUUID;
		static constexpr bool SameName = T1::InterfaceName == T2::InterfaceName;
		
		static constexpr bool Equal = SameUUID && SameName;
	};
}

#endif /* CORE_INTERFACE_H */