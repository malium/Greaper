/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "Application.h"
#include <Core/Platform.h>
#include <Core/IGreaperLibrary.h>
#include <Core/Property.h>

using namespace greaper;
using namespace greaper::core;

SPtr<Application> gApplication = {};

EmptyResult Application::RegisterGreaperLibrary(const SPtr<IGreaperLibrary>& gLib)
{
	auto uLib = GetGreaperLibrary(gLib->GetLibraryUuid());
	if (uLib.IsOk() && uLib.GetValue() != nullptr)
	{
		return Result::CreateFailure(Format(
			"Trying to register a GreaperLibrary, but its UUID '%s' its already registered.",
			gLib->GetLibraryUuid().ToString().c_str()));
	}
	auto nLib = GetGreaperLibrary(gLib->GetLibraryName());
	if (nLib.IsOk() && nLib.GetValue() != nullptr)
	{
		return Result::CreateFailure(Format(
			"Trying to register a GreaperLibrary, but its name '%s' its already registered.",
			gLib->GetLibraryName().data()));
	}

	const auto id = m_Libraries.size();
	m_LibraryNameMap.insert_or_assign(gLib->GetLibraryName(), id);
	m_LibraryUuidMap.insert_or_assign(gLib->GetLibraryUuid(), id);
	LibInfo info;
	info.Lib = gLib;
	m_Libraries.push_back(std::move(info));

	return Result::CreateSuccess();
}

void Application::UpdateActiveInterfaceList()noexcept 
{
	LOCK(m_ActiveMutex);

	for (const auto& iface : m_InterfacesToRemove)
	{
		const auto ifaceIDX = IndexOf(m_ActiveInterfaces, iface);
		if (ifaceIDX < 0)
			continue; // Not in vector

		m_ActiveInterfaces[ifaceIDX].reset();
		iface->Deactivate(PInterface());
	}
	m_InterfacesToRemove.clear();

	for (const auto& iface : m_InterfacesToAdd)
	{
		auto uuidIT = m_ActiveInterfaceUuidMap.find(iface->GetInterfaceUUID());
		sizet ifaceIDX;
		if (uuidIT == m_ActiveInterfaceUuidMap.end())
		{
			ifaceIDX = m_ActiveInterfaces.size();
			m_ActiveInterfaces.push_back(PInterface());
			m_ActiveInterfaceNameMap.insert_or_assign(iface->GetInterfaceName(), ifaceIDX);
			m_ActiveInterfaceUuidMap.insert_or_assign(iface->GetInterfaceUUID(), ifaceIDX);
		}
		else
		{
			ifaceIDX = uuidIT->second;
		}
		m_ActiveInterfaces[ifaceIDX] = iface;
		iface->Activate(PInterface());
		m_OnInterfaceActivation.Trigger(iface);
	}
	m_InterfacesToAdd.clear();

	for (const auto& iface : m_InterfaceToChange)
	{
		auto uuidIT = m_ActiveInterfaceUuidMap.find(iface->GetInterfaceUUID());
		VerifyInequal(uuidIT, m_ActiveInterfaceUuidMap.end(), "Couldn't find the Active interafce with UUID '%s' on the ActiveInterfaces.", iface->GetInterfaceUUID().ToString().c_str());
		const auto ifaceIDX = uuidIT->second;
		auto& oiFace = m_ActiveInterfaces[ifaceIDX];
		iface->Activate(oiFace);
		m_OnInterfaceActivation.Trigger(iface);
		m_ActiveInterfaces[ifaceIDX] = iface;
		oiFace->Deactivate(iface);
	}
	m_InterfaceToChange.clear();
}

Application::Application()
	:m_OnInterfaceActivation("OnInterfaceActivation"sv)
{

}

Application::~Application()noexcept
{
	// No-op
}

void Application::OnInitialization() noexcept
{
	VerifyNot(m_Library.expired(), "Trying to initialize an application with an expired GreaperLib.");
	auto lib = m_Library.lock();
	gApplication = lib->GetApplication().lock();

	RegisterGreaperLibrary(lib);
}

void Application::OnDeinitialization() noexcept
{
	VerifyNot(m_Library.expired(), "Trying to deinitialize Application but its GreaperLibrary is expired.");
	auto ownLib = m_Library.lock();

	{
		LOCK(m_ActiveMutex);
		for (auto& mgr : m_ActiveInterfaces)
		{
			if (mgr == nullptr)
				continue;
			DeactivateInterface(mgr->GetInterfaceUUID());
		}

		m_ActiveInterfaces.clear();
		m_ActiveInterfaceNameMap.clear();
		m_ActiveInterfaceUuidMap.clear();

		m_InterfacesToAdd.clear();
		m_InterfacesToRemove.clear();
		m_InterfaceToChange.clear();
	}

	for (auto& lib : m_Libraries)
	{
		auto& gLib = lib.Lib;
		if (gLib->GetLibraryUuid() == ownLib->GetLibraryUuid())
		{
			lib.IntefaceNameMap.clear();
			lib.InterfaceUuidMap.clear();
			lib.Interfaces.clear();
			continue;
		}

		if (gLib->GetInitializationState() == InitState_t::Started)
			gLib->DeinitLibrary();

		lib.IntefaceNameMap.clear();
		lib.InterfaceUuidMap.clear();
		lib.Interfaces.clear();
	}
	m_Libraries.clear();
	m_LibraryNameMap.clear();
	m_LibraryUuidMap.clear();

	gApplication.reset();
}

void Application::OnActivation(UNUSED const SPtr<IInterface>& oldDefault) noexcept
{
	/* No-op */
}

void Application::OnDeactivation(UNUSED const SPtr<IInterface>& newDefault) noexcept
{
	/* No-op */
}

void Application::InitProperties()noexcept
{
	if (m_Library.expired())
		return; // no base library weird

	auto lib = m_Library.lock();

	if (m_Properties.size() != (sizet)COUNT)
		m_Properties.resize((sizet)COUNT, WIProperty());

	WPtr<AppInstanceProp_t> appInstanceProp;
	auto result = lib->GetProperty(AppInstanceName);
	if (result.IsOk())
		appInstanceProp = result.GetValue();
	m_Properties[(sizet)AppInstance] = std::move(appInstanceProp);

	WPtr<CommandLineProp_t> commandLineProp;
	result = lib->GetProperty(CommandLineName);
	if (result.IsOk())
		commandLineProp = result.GetValue();
	m_Properties[(sizet)CommandLine] = std::move(commandLineProp);

	WPtr<ApplicationNameProp_t> appNameProp;
	result = lib->GetProperty(ApplicationNameName);
	if (result.IsOk())
		appNameProp = result.GetValue();

	if (appNameProp.expired())
	{
		auto appNameResult = CreateProperty<greaper::String>(m_Library, ApplicationNameName, {}, ""sv, false, true, nullptr);
		Verify(appNameResult.IsOk(), "Couldn't create the property '%s' msg: %s", ApplicationNameName.data(), appNameResult.GetFailMessage().c_str());
		appNameProp = (WPtr<ApplicationNameProp_t>)appNameResult.GetValue();
	}
	m_Properties[(sizet)ApplicationName] = std::move(appNameProp);

	WPtr<ApplicationVersionProp_t> appVersionProp;
	result = lib->GetProperty(ApplicationVersionName);
	if (result.IsOk())
		appVersionProp = result.GetValue();

	if (appVersionProp.expired())
	{
		auto appVersionResult = CreateProperty<uint32>(m_Library, ApplicationVersionName, 0, ""sv, false, true, nullptr);
		Verify(appVersionResult.IsOk(), "Couldn't create the property '%s' msg: %s", ApplicationVersionName.data(), appVersionResult.GetFailMessage().c_str());
		appVersionProp = (WPtr<ApplicationVersionProp_t>)appVersionResult.GetValue();
	}
	m_Properties[(sizet)ApplicationVersion] = std::move(appVersionProp);

	WPtr<CompilationInfoProp_t> compilationInfoProp;
	result = lib->GetProperty(CompilationInfoName);
	if (result.IsOk())
		compilationInfoProp = result.GetValue();

	if (compilationInfoProp.expired())
	{
		static constexpr StringView gCompilationInfo =
#if GREAPER_DEBUG
			"DEBUG"sv;
#elif GREAPER_FRELEASE
			"PUBLIC"sv;
#else
			"RELEASE"sv;
#endif
		auto comilationInfoResult = CreateProperty<greaper::String>(m_Library, CompilationInfoName, greaper::String{ gCompilationInfo }, ""sv, true, true, nullptr);
		Verify(comilationInfoResult.IsOk(), "Couldn't create the property '%s' msg: %s", CompilationInfoName.data(), comilationInfoResult.GetFailMessage().c_str());
		compilationInfoProp = (WPtr<CompilationInfoProp_t>)comilationInfoResult.GetValue();
	}
	m_Properties[(sizet)CompilationInfo] = std::move(compilationInfoProp);

	WPtr<LoadedLibrariesProp_t> loadedLibrariesProp;
	result = lib->GetProperty(LoadedLibrariesName);
	if (result.IsOk())
		loadedLibrariesProp = result.GetValue();

	if (loadedLibrariesProp.expired())
	{
		auto loadedLibrariesResult = CreateProperty<greaper::WStringVec>(m_Library, LoadedLibrariesName, {}, ""sv, false, true, nullptr);
		Verify(loadedLibrariesResult.IsOk(), "Couldn't create the property '%s' msg: %s", LoadedLibrariesName.data(), loadedLibrariesResult.GetFailMessage().c_str());
		loadedLibrariesProp = (WPtr<LoadedLibrariesProp_t>)loadedLibrariesResult.GetValue();
	}
	m_Properties[(sizet)LoadedLibraries] = (WPtr<LoadedLibrariesProp_t>)std::move(loadedLibrariesProp);
}

void Application::DeinitProperties()noexcept
{
	for (auto& prop : m_Properties)
		prop.reset();
}

void Application::InitSerialization() noexcept
{

}

void Application::DeinitSerialization() noexcept
{
}

TResult<SPtr<IGreaperLibrary>> Application::RegisterGreaperLibrary(const WStringView& libPath)noexcept
{
	PLibrary lib{ Construct<Library>(libPath) };

	if (!lib->IsOpen())
	{
		return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format(
			"Trying to register a GreaperLibrary with path '%S', but couldn't be openned.", libPath.data()));
	}
	auto fnRes = lib->GetFunctionT<void*>("_Greaper"sv);
	if (fnRes.HasFailed())
	{
		return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format(
			"Trying to register a GreaperLibrary with path '%S', but does not comply with Greaper modular protocol.",
			libPath.data()));
	}
	auto gLibPtr = reinterpret_cast<PGreaperLib*>(fnRes.GetValue()());
	if (gLibPtr == nullptr || (*gLibPtr) == nullptr)
	{
		return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format(
			"Trying to register a GreaperLibrary with path '%S', but the library returned a nullptr GreaperLibrary.",
			libPath.data()));
	}
	auto gLib = PGreaperLib(*gLibPtr);
	auto res = RegisterGreaperLibrary(gLib);
	
	if (res.HasFailed())
		return Result::CopyFailure<SPtr<IGreaperLibrary>>(res);

	gLib->InitLibrary(lib, (SPtr<IApplication>)gApplication);
	
	return Result::CreateSuccess(gLib);
}

TResult<SPtr<IGreaperLibrary>> Application::GetGreaperLibrary(const StringView& libraryName)const noexcept
{
	if (auto findIT = m_LibraryNameMap.find(libraryName); findIT != m_LibraryNameMap.end())
	{
		if (m_Libraries.size() <= findIT->second)
		{
			return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format("A GreaperLibrary with name '%s' was found, but the library list didn't have that library.", libraryName.data()));
		}
		auto& libInfo = m_Libraries[findIT->second];
		return Result::CreateSuccess(libInfo.Lib);
	}

	return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format("Couldn't find the GreaperLibrary '%s'.", libraryName.data()));
}

TResult<SPtr<IGreaperLibrary>> Application::GetGreaperLibrary(const Uuid& libraryUUID)const noexcept
{
	if (const auto findIT = m_LibraryUuidMap.find(libraryUUID); findIT != m_LibraryUuidMap.end())
	{
		if (m_Libraries.size() <= findIT->second)
		{
			return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format("A GreaperLibrary with UUID '%s' was found, but the library list didn't have that library.", libraryUUID.ToString().c_str()));
		}
		auto& libInfo = m_Libraries[findIT->second];
		return Result::CreateSuccess(libInfo.Lib);
	}

	return Result::CreateFailure<SPtr<IGreaperLibrary>>(Format("Couldn't find the GreaperLibrary '%s'.", libraryUUID.ToString().c_str()));
}

EmptyResult Application::UnregisterGreaperLibrary(SPtr<IGreaperLibrary> library)noexcept
{
	if (library == nullptr)
		return Result::CreateFailure("Trying to unregister a nullptr GreaperLibrary"sv);
	
	const auto nameIT = m_LibraryNameMap.find(library->GetLibraryName());
	const auto uuidIT = m_LibraryUuidMap.find(library->GetLibraryUuid());

	sizet nIndex = std::numeric_limits<sizet>::max();
	sizet uIndex = std::numeric_limits<sizet>::max();
	sizet index = std::numeric_limits<sizet>::max();
	if (nameIT != m_LibraryNameMap.end())
	{
		nIndex = nameIT->second;
		m_LibraryNameMap.erase(nameIT);
	}
	if (uuidIT != m_LibraryUuidMap.end())
	{
		uIndex = uuidIT->second;
		m_LibraryUuidMap.erase(uuidIT);
	}

	auto lib = m_Library.lock();
	VerifyNotNull(lib, "Trying to unregister a GreaperLibrary, but the Application library is expired.");

	if (nIndex != uIndex)
	{
		lib->LogWarning(Format("Trying to unregister a GreaperLibrary '%s', but its name and uuid points to different indices.", library->GetLibraryName().data()));
		if (m_Libraries.size() > nIndex)
			index = nIndex;
		else if (m_Libraries.size() > uIndex)
			index = uIndex;
	}
	else
	{
		index = nIndex;
	}

	if (m_Libraries.size() <= index)
		return Result::CreateFailure(Format("Trying to unregister a GreaperLibrary '%s', but it was not registered.", library->GetLibraryName().data()));

	auto& libInfo = m_Libraries[index];
	lib->Log(Format("Unregistering GraeperLibrary '%s'.", library->GetLibraryName().data()));
	for (auto& iface : libInfo.Interfaces)
	{
		if (iface == nullptr)
			continue;

		if (iface->IsActive())
		{
			DeactivateInterface(iface->GetInterfaceUUID());
		}
		if (iface->IsInitialized())
		{
			iface->Deinitialize();
		}
	}
	libInfo.Lib->DeinitLibrary();
	libInfo.Lib.reset();
	libInfo.IntefaceNameMap.clear();
	libInfo.InterfaceUuidMap.clear();
	libInfo.Interfaces.clear();
	return Result::CreateSuccess();
}

EmptyResult Application::RegisterInterface(const PInterface& interface)noexcept
{
	if (interface == nullptr)
		return Result::CreateFailure("Trying to register a nullptr interface."sv);

	const auto& wLib = interface->GetLibrary();
	if (wLib.expired())
	{
		return Result::CreateFailure("Trying to register an Interface without GreaperLibrary."sv);
	}
	auto pLib = wLib.lock();
	
	const auto findIT = m_LibraryUuidMap.find(pLib->GetLibraryUuid());
	if (findIT == m_LibraryUuidMap.end())
		return Result::CreateFailure("Trying to register an Interface with a non-registered GreaperLibrary."sv);
	
	auto& libInfo = m_Libraries[findIT->second];
	
	if (libInfo.Lib != pLib)
		return Result::CreateFailure("Trying to register an Interface with a GreaperLibrary which UUID points to another GreaperLibrary."sv);
	
	if (const auto nameIT = libInfo.IntefaceNameMap.find(interface->GetInterfaceName()); nameIT != libInfo.IntefaceNameMap.end())
	{
		return Result::CreateFailure("Trying to register an Interface, but that GreaperLibrary already has an Interface registered with the same name."sv);
	}

	if (const auto uuidIT = libInfo.InterfaceUuidMap.find(interface->GetInterfaceUUID()); uuidIT != libInfo.InterfaceUuidMap.end())
	{
		return Result::CreateFailure("Trying to register an Interface, but that GreaperLibrary already has an Interface registered with the same UUID."sv);
	}

	const auto index = libInfo.Interfaces.size();
	libInfo.Interfaces.push_back(interface);
	libInfo.IntefaceNameMap.insert_or_assign(interface->GetInterfaceName(), index);
	libInfo.InterfaceUuidMap.insert_or_assign(interface->GetInterfaceUUID(), index);

	/*if (!interface->IsInitialized())
		interface->Initialize(std::move(pLib));*/
	return Result::CreateSuccess();
}

EmptyResult Application::UnregisterInterface(const PInterface& interface)noexcept
{
	if (interface == nullptr)
		return Result::CreateFailure("Trying to unregister a nullptr interface."sv);

	const auto& wLib = interface->GetLibrary();
	if (wLib.expired())
	{
		return Result::CreateFailure("Trying to unregister an Interface without GreaperLibrary."sv);
	}
	auto pLib = wLib.lock();

	const auto findIT = m_LibraryUuidMap.find(pLib->GetLibraryUuid());
	if (findIT == m_LibraryUuidMap.end())
		return Result::CreateFailure("Trying to unregister an Interface with a non-registered GreaperLibrary."sv);

	auto& libInfo = m_Libraries[findIT->second];

	if (libInfo.Lib != pLib)
		return Result::CreateFailure("Trying to unregister an Interface with a GreaperLibrary which UUID points to another GreaperLibrary."sv);

	const auto nameIT = libInfo.IntefaceNameMap.find(interface->GetInterfaceName());
	const auto uuidIT = libInfo.InterfaceUuidMap.find(interface->GetInterfaceUUID());

	sizet nIndex = std::numeric_limits<sizet>::max();
	sizet uIndex = std::numeric_limits<sizet>::max();
	sizet index = std::numeric_limits<sizet>::max();
	if (nameIT != libInfo.IntefaceNameMap.end())
	{
		nIndex = nameIT->second;
		libInfo.IntefaceNameMap.erase(nameIT);
	}
	if (uuidIT != libInfo.InterfaceUuidMap.end())
	{
		uIndex = uuidIT->second;
		libInfo.InterfaceUuidMap.erase(uuidIT);
	}

	auto lib = m_Library.lock();
	VerifyNotNull(lib, "Trying to unregister a GreaperLibrary, but the Application library is expired.");

	if (nIndex != uIndex)
	{
		lib->LogWarning(Format("Trying to unregister an Interface '%s', but its name and uuid points to different indices.", interface->GetInterfaceName().data()));
		if (libInfo.Interfaces.size() > nIndex)
			index = nIndex;
		else if (libInfo.Interfaces.size() > uIndex)
			index = uIndex;
	}
	else
	{
		index = nIndex;
	}

	if (libInfo.Interfaces.size() <= index)
		return Result::CreateFailure(Format("Trying to unregister an Interface '%s', but it was not registered.", interface->GetInterfaceName().data()));

	lib->Log(Format("Unregistering an Interface '%s' from '%s' GreaperLibrary.", interface->GetInterfaceName().data(), pLib->GetLibraryName().data()));

	if (interface->IsActive())
		DeactivateInterface(interface->GetInterfaceUUID());
	if (interface->IsInitialized())
		interface->Deinitialize();
	libInfo.Interfaces[index].reset();
	return Result::CreateSuccess();
}

EmptyResult Application::ActivateInterface(const PInterface& interface)noexcept
{
	if (interface == nullptr)
		return Result::CreateFailure("Trying to make default a nullptr interface, if you want to remove an Active interface call StopInterfaceDefault."sv);

	LOCK(m_ActiveMutex);
	
	const auto toRemoveIdx = IndexOf(m_InterfacesToRemove, interface);
	if (toRemoveIdx >= 0)
	{
		m_InterfacesToRemove.erase(m_InterfacesToRemove.begin() + toRemoveIdx);
	}

	if (Contains(m_InterfaceToChange, interface))
	{
		return Result::CreateSuccess();
	}

	if(Contains(m_InterfacesToAdd, interface))
	{
		return Result::CreateSuccess();
	}

	const auto existsAnother = m_ActiveInterfaceUuidMap.find(interface->GetInterfaceUUID()) != m_ActiveInterfaceUuidMap.end(); // m_ActiveInterfaceUuidMap.contains(interface->GetInterfaceUUID());

	if (existsAnother)
		m_InterfaceToChange.push_back(interface);
	else
		m_InterfacesToAdd.push_back(interface);

	UpdateActiveInterfaceList();

	return Result::CreateSuccess();
}

EmptyResult Application::DeactivateInterface(const Uuid& interfaceUUID)noexcept
{
	LOCK(m_ActiveMutex);
	PInterface interface;
	// Remove it from toAdd and toChange
	bool removed = false;
	sizet i = 0;
	for (const auto & iface : m_InterfacesToAdd)
	{
		if (iface->GetInterfaceUUID() == interfaceUUID)
		{
			m_InterfacesToAdd.erase(m_InterfacesToAdd.begin() + i);
			removed = true;
			break;
		}
		++i;
	}

	i = 0;
	for (const auto & iface : m_InterfaceToChange)
	{
		if (iface->GetInterfaceUUID() == interfaceUUID)
		{
			m_InterfaceToChange.erase(m_InterfaceToChange.begin() + i);
			removed = true;
			break;
		}
		++i;
	}

	if (const auto activeIT = m_ActiveInterfaceUuidMap.find(interfaceUUID);
		activeIT != m_ActiveInterfaceUuidMap.end())
		interface = m_ActiveInterfaces[activeIT->second];

	if (interface == nullptr)
	{
		if (removed)
			return Result::CreateSuccess(); // removed from activation

		return Result::CreateFailure(Format("Trying to deactivate an Interface with UUID '%s', but was not active.", interfaceUUID.ToString().c_str()));
	}
	m_InterfacesToRemove.push_back(interface);

	UpdateActiveInterfaceList();

	return Result::CreateSuccess();
}

EmptyResult Application::DeactivateInterface(const StringView& interfaceName)noexcept
{
	LOCK(m_ActiveMutex);
	PInterface interface;
	// Remove it from toAdd and toChange
	bool removed = false;
	sizet i = 0;
	for (const auto & iface : m_InterfacesToAdd)
	{
		if (iface->GetInterfaceName() == interfaceName)
		{
			m_InterfacesToAdd.erase(m_InterfacesToAdd.begin() + i);
			removed = true;
			break;
		}
		++i;
	}

	i = 0;
	for (const auto & iface : m_InterfaceToChange)
	{
		if (iface->GetInterfaceName() == interfaceName)
		{
			m_InterfaceToChange.erase(m_InterfaceToChange.begin() + i);
			removed = true;
			break;
		}
		++i;
	}

	if (const auto activeIT = m_ActiveInterfaceNameMap.find(interfaceName);
		activeIT != m_ActiveInterfaceNameMap.end())
		interface = m_ActiveInterfaces[activeIT->second];

	if (interface == nullptr)
	{
		if (removed)
			return Result::CreateSuccess(); // Removed from activation

		return Result::CreateFailure(Format("Trying to deactivate an Interface with name '%s', but was not active.", interfaceName.data()));
	}

	m_InterfacesToRemove.push_back(interface);

	UpdateActiveInterfaceList();

	return Result::CreateSuccess();
}

TResult<PInterface> Application::GetActiveInterface(const Uuid& interfaceUUID) const noexcept
{
	LOCK(m_ActiveMutex);

	const auto uuidIT = m_ActiveInterfaceUuidMap.find(interfaceUUID);
	if (uuidIT == m_ActiveInterfaceUuidMap.end())
		return Result::CreateFailure<PInterface>(Format("Couldn't find an active Interface with UUID '%s'.", interfaceUUID.ToString().c_str()));

	const auto index = uuidIT->second;

	if (index >= m_ActiveInterfaces.size())
		return Result::CreateFailure<PInterface>(Format("Interface with UUID '%s', its index was out of bounds.", interfaceUUID.ToString().c_str()));

	auto& iface = m_ActiveInterfaces[index];

	return Result::CreateSuccess<PInterface>(iface);
}

TResult<PInterface> Application::GetActiveInterface(const StringView& interfaceName) const noexcept
{
	LOCK(m_ActiveMutex);

	const auto nameIT = m_ActiveInterfaceNameMap.find(interfaceName);
	if (nameIT == m_ActiveInterfaceNameMap.end())
		return Result::CreateFailure<PInterface>(Format("Couldn't find an active Interface with name '%s'.", interfaceName.data()));

	const auto index = nameIT->second;

	if (index >= m_ActiveInterfaces.size())
		return Result::CreateFailure<PInterface>(Format("Interface with name '%s', its index was out of bounds.", interfaceName.data()));

	auto& iface = m_ActiveInterfaces[index];

	return Result::CreateSuccess(iface);
}

TResult<PInterface> Application::GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID) const noexcept
{
	const auto libUuidIT = m_LibraryUuidMap.find(libraryUUID);
	if (libUuidIT == m_LibraryUuidMap.end())
	{
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the library is not registered.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));
	}

	if(libUuidIT->second >= m_Libraries.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the library index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));

	auto& libInfo = m_Libraries[libUuidIT->second];

	const auto ifaceUuidIT = libInfo.InterfaceUuidMap.find(interfaceUUID);

	if(ifaceUuidIT == libInfo.InterfaceUuidMap.end())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the interface is not registered.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));

	if(ifaceUuidIT->second >= libInfo.Interfaces.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the interface index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));

	auto& iface = libInfo.Interfaces[ifaceUuidIT->second];
	return Result::CreateSuccess(iface);
}

TResult<PInterface> Application::GetInterface(const StringView& interfaceName, const StringView& libraryName) const noexcept
{
	const auto libNameIT = m_LibraryNameMap.find(libraryName);
	if (libNameIT == m_LibraryNameMap.end())
	{
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the library is not registered.", interfaceName.data(), libraryName.data()));
	}

	if (libNameIT->second >= m_Libraries.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the library index is outside of bounds.", interfaceName.data(), libraryName.data()));

	auto& libInfo = m_Libraries[libNameIT->second];

	const auto ifaceNameIT = libInfo.IntefaceNameMap.find(interfaceName);

	if (ifaceNameIT == libInfo.IntefaceNameMap.end())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the interface is not registered.", interfaceName.data(), libraryName.data()));

	if (ifaceNameIT->second >= libInfo.Interfaces.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the interface index is outside of bounds.", interfaceName.data(), libraryName.data()));

	auto& iface = libInfo.Interfaces[ifaceNameIT->second];
	return Result::CreateSuccess(iface);
}

TResult<PInterface> Application::GetInterface(const Uuid& interfaceUUID, const StringView& libraryName) const noexcept
{
	const auto libNameIT = m_LibraryNameMap.find(libraryName);
	if (libNameIT == m_LibraryNameMap.end())
	{
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the library is not registered.", interfaceUUID.ToString().c_str(), libraryName.data()));
	}

	if (libNameIT->second >= m_Libraries.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the library index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryName.data()));

	auto& libInfo = m_Libraries[libNameIT->second];

	const auto ifaceUuidIT = libInfo.InterfaceUuidMap.find(interfaceUUID);

	if (ifaceUuidIT == libInfo.InterfaceUuidMap.end())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the interface is not registered.", interfaceUUID.ToString().c_str(), libraryName.data()));

	if (ifaceUuidIT->second >= libInfo.Interfaces.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the interface index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryName.data()));

	auto& iface = libInfo.Interfaces[ifaceUuidIT->second];
	return Result::CreateSuccess(iface);
}

TResult<PInterface> Application::GetInterface(const StringView& interfaceName, const Uuid& libraryUUID) const noexcept
{
	const auto libUuidIT = m_LibraryUuidMap.find(libraryUUID);
	if (libUuidIT == m_LibraryUuidMap.end())
	{
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the library is not registered.", interfaceName.data(), libraryUUID.ToString().c_str()));
	}

	if (libUuidIT->second >= m_Libraries.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the library index is outside of bounds.", interfaceName.data(), libraryUUID.ToString().c_str()));

	auto& libInfo = m_Libraries[libUuidIT->second];

	const auto ifaceNameIT = libInfo.IntefaceNameMap.find(interfaceName);

	if (ifaceNameIT == libInfo.IntefaceNameMap.end())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the interface is not registered.", interfaceName.data(), libraryUUID.ToString().c_str()));

	if (ifaceNameIT->second >= libInfo.Interfaces.size())
		return Result::CreateFailure<PInterface>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the interface index is outside of bounds.", interfaceName.data(), libraryUUID.ToString().c_str()));

	auto& iface = libInfo.Interfaces[ifaceNameIT->second];
	return Result::CreateSuccess(iface);
}