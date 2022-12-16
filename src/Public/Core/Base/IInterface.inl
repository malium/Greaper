/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

//#include "../IGreaperLibrary.h"

namespace greaper
{
	INLINE IInterface::IInterface() noexcept
		:m_InitEvent("Interface_Initialize"sv)
		,m_ActivationEvent("Interface_Activation"sv)
		,m_InitializationState(InitState_t::Stopped)
		,m_ActiveState(InitState_t::Stopped)
	{

	}

	inline const WGreaperLib& IInterface::GetLibrary()const noexcept
	{
		return m_Library;
	}

	INLINE bool IInterface::IsActive() const noexcept
	{
		return m_ActiveState == InitState_t::Started;
	}

	INLINE bool IInterface::IsInitialized() const noexcept
	{
		return m_InitializationState == InitState_t::Started;
	}

	INLINE InitState_t IInterface::GetActivationState() const noexcept
	{
		return m_ActiveState;
	}

	INLINE InitState_t IInterface::GetInitializationState() const noexcept
	{
		return m_InitializationState;
	}

	INLINE IInterface::InitializationEvt_t* IInterface::GetInitializationEvent() const noexcept
	{
		return &m_InitEvent;
	}

	INLINE IInterface::ActivationEvt_t* IInterface::GetActivationEvent() const noexcept
	{
		return &m_ActivationEvent;
	}

	INLINE CSpan<WIProperty> IInterface::GetProperties() const noexcept
	{
		return CreateSpan(m_Properties);
	}

	INLINE void IInterface::Initialize(WGreaperLib library) noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Stopped, "Trying to initialize an already initialized Interface '%s'.", GetInterfaceName().data());

		auto lib = library.lock();
		lib->Log(Format("Initializing %s from %s...", GetInterfaceName().data(), lib->GetLibraryName().data()));

		m_InitializationState = InitState_t::Starting;
		m_Library = std::move(library);

		OnInitialization();

		lib->Log(Format("%s from %s has been initialized.", GetInterfaceName().data(), lib->GetLibraryName().data()));
		m_InitializationState = InitState_t::Started;
		m_InitEvent.Trigger(true);
	}

	INLINE void IInterface::Deinitialize() noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Started, "Trying to deinitialize an already deinitialized Interface '%s'.", GetInterfaceName().data());

		auto lib = m_Library.lock();
		lib->Log(Format("Deinitializing %s from %s...", GetInterfaceName().data(), lib->GetLibraryName().data()));

		m_InitializationState = InitState_t::Stopping;
		OnDeinitialization();

		m_Library.reset();

		lib->Log(Format("%s from %s has been deinitialized.", GetInterfaceName().data(), lib->GetLibraryName().data()));

		m_InitializationState = InitState_t::Stopped;
		m_InitEvent.Trigger(false);
	}

	INLINE void IInterface::Activate(const PInterface& oldDefault) noexcept
	{
		VerifyEqual(m_ActiveState, InitState_t::Stopped, "Trying to activate an already activated Interface '%s'.", GetInterfaceName().data());

		auto lib = m_Library.lock();
		lib->Log(Format("Activating %s from %s...", GetInterfaceName().data(), lib->GetLibraryName().data()));

		m_ActiveState = InitState_t::Starting;
		OnActivation(oldDefault);

		m_ActiveState = InitState_t::Started;
		m_ActivationEvent.Trigger(true, this, oldDefault);
	}

	INLINE void IInterface::Deactivate(const PInterface& newDefault) noexcept
	{
		VerifyEqual(m_ActiveState, InitState_t::Started, "Trying to deactivate an already deactivated Interface '%s'.", GetInterfaceName().data());

		auto lib = m_Library.lock();
		lib->Log(Format("Deactivating %s from %s...", GetInterfaceName().data(), lib->GetLibraryName().data()));

		m_ActiveState = InitState_t::Stopping;
		OnDeactivation(newDefault);

		m_ActiveState = InitState_t::Stopped;
		m_ActivationEvent.Trigger(false, this, newDefault);
	}
}