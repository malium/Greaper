/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H 1

#include <utility>
//#include <span>

#include "Uuid.h"
#include "Event.h"
#include "Enumeration.h"

ENUMERATION(InitState, Stopped, Starting, Started, Stopping);

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
	protected:
		virtual void OnInitialization()noexcept = 0;

		virtual void OnDeinitialization()noexcept = 0;

		virtual void OnActivation(const SPtr<IInterface>& oldDefault)noexcept = 0;

		virtual void OnDeactivation(const SPtr<IInterface>& newDefault)noexcept = 0;

	public:
		using InitializationEvt_t = Event<bool>;
		using ActivationEvt_t = Event<bool, IInterface*, SPtr<IInterface>>;

		IInterface()noexcept;
		virtual ~IInterface()noexcept = default;

		static constexpr Uuid InterfaceUUID = Uuid{  };
		static constexpr StringView InterfaceName = StringView{ "INullInterface" };

		virtual const Uuid& GetInterfaceUUID()const noexcept = 0;
		
		virtual const StringView& GetInterfaceName()const noexcept = 0;
		
		WPtr<IGreaperLibrary> GetLibrary()const noexcept;

		void Initialize(WPtr<IGreaperLibrary> library)noexcept;

		void Deinitialize()noexcept;

		void Activate(const SPtr<IInterface>& oldDefault)noexcept;

		void Deactivate(const SPtr<IInterface>& newDefault)noexcept;

		virtual void InitProperties()noexcept = 0;

		virtual void DeinitProperties()noexcept = 0;

		virtual void InitSerialization()noexcept = 0;

		virtual void DeinitSerialization()noexcept = 0;

		bool IsActive()const noexcept;
		
		bool IsInitialized()const noexcept;

		InitState_t GetActivationState()const noexcept;

		InitState_t GetInitializationState()const noexcept;

		InitializationEvt_t* GetInitializationEvent()const noexcept;

		ActivationEvt_t* GetActivationEvent()const noexcept;

		CSpan<WPtr<IProperty>> GetProperties()const noexcept;

	protected:
		WPtr<IGreaperLibrary> m_Library;
		mutable InitializationEvt_t m_InitEvent;
		mutable ActivationEvt_t m_ActivationEvent;
		Vector<WPtr<IProperty>> m_Properties;

	private:
		InitState_t m_InitializationState;
		InitState_t m_ActiveState;
	};

	INLINE IInterface::IInterface() noexcept
		:m_InitEvent("Interface_Initialize"sv)
		,m_ActivationEvent("Interface_Activation"sv)
		,m_InitializationState(InitState_t::Stopped)
		,m_ActiveState(InitState_t::Stopped)
	{

	}

	INLINE WPtr<IGreaperLibrary> IInterface::GetLibrary()const noexcept { return m_Library; }

	INLINE void IInterface::Initialize(WPtr<IGreaperLibrary> library) noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Stopped, "Trying to initialize an already initialized Interface '%s'.", GetInterfaceName().data());
		
		m_InitializationState = InitState_t::Starting;
		m_Library = std::move(library);

		OnInitialization();

		m_InitializationState = InitState_t::Started;
		m_InitEvent.Trigger(true);
	}

	INLINE void IInterface::Deinitialize() noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Started, "Trying to deinitialize an already deinitialized Interface '%s'.", GetInterfaceName().data());

		m_InitializationState = InitState_t::Stopping;
		OnDeinitialization();

		m_Library.reset();

		m_InitializationState = InitState_t::Stopped;
		m_InitEvent.Trigger(false);
	}

	INLINE void IInterface::Activate(const SPtr<IInterface>& oldDefault) noexcept
	{
		VerifyEqual(m_ActiveState, InitState_t::Stopped, "Trying to activate an already activated Interface '%s'.", GetInterfaceName().data());

		m_ActiveState = InitState_t::Starting;
		OnActivation(oldDefault);

		m_ActiveState = InitState_t::Started;
		m_ActivationEvent.Trigger(true, this, (SPtr<IInterface>)oldDefault);
	}

	INLINE void IInterface::Deactivate(const SPtr<IInterface>& newDefault) noexcept
	{
		VerifyEqual(m_ActiveState, InitState_t::Started, "Trying to deactivate an already deactivated Interface '%s'.", GetInterfaceName().data());

		m_ActiveState = InitState_t::Stopping;
		OnDeactivation(newDefault);

		m_ActiveState = InitState_t::Stopped;
		m_ActivationEvent.Trigger(false, this, (SPtr<IInterface>)newDefault);
	}

	INLINE bool IInterface::IsActive() const noexcept { return m_ActiveState == InitState_t::Started; }

	INLINE bool IInterface::IsInitialized() const noexcept { return m_InitializationState == InitState_t::Started; }

	INLINE InitState_t IInterface::GetActivationState() const noexcept { return m_ActiveState; }

	INLINE InitState_t IInterface::GetInitializationState() const noexcept { return m_InitializationState; }

	INLINE IInterface::InitializationEvt_t* IInterface::GetInitializationEvent() const noexcept { return &m_InitEvent; }

	INLINE IInterface::ActivationEvt_t* IInterface::GetActivationEvent() const noexcept { return &m_ActivationEvent; }

	INLINE CSpan<WPtr<IProperty>> IInterface::GetProperties() const noexcept { return CreateSpan(m_Properties); }

	using PInterface = SPtr<IInterface>;
	using WInterface = WPtr<IInterface>;

	template<class T>
	class TInterface : public IInterface
	{
	public:
		virtual ~TInterface()noexcept = default;

		const Uuid& GetInterfaceUUID()const noexcept override { return T::InterfaceUUID; }

		const StringView& GetInterfaceName()const noexcept override { return T::InterfaceName; }
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