/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "Application.h"
#include <Core/Platform.h>
#include <Core/IGreaperLibrary.h>
#include <Core/Property.h>

using namespace greaper;
using namespace greaper::core;

void Application::AddGreaperLibrary(IGreaperLibrary* library)
{
	const auto id = m_Libraries.size();
	m_LibraryNameMap.insert_or_assign(library->GetLibraryName(), id);
	m_LibraryUuidMap.insert_or_assign(library->GetLibraryUuid(), id);
	LibInfo info;
	info.Lib = library;
	m_Libraries.push_back(std::move(info));
}

void Application::LoadConfigLibraries()
{
	/*if (m_Config.GreaperLibraries == nullptr)
		return;
	for (uint32 i = 0; i < m_Config.GreaperLibraryCount; ++i)
	{
		auto res = RegisterGreaperLibrary(m_Config.GreaperLibraries[i]);
		if (res.HasFailed())
		{
			m_Library->LogWarning(res.GetFailMessage());
			continue;
		}
	}*/
}

void Application::ClearFrameTimes()noexcept
{
	ClearMemory(m_FrameTimes);
	m_UpdateDeltaMin = FLT_MAX;
	m_UpdateDeltaMax = FLT_MIN;
	m_UpdateDeltaAvg = 0.f;
}

void Application::UpdateActiveInterfaceList()
{
	LOCK(m_ActiveMutex);

	for (sizet i = 0; i < m_InterfacesToRemove.size(); ++i)
	{
		auto* iface = m_InterfacesToRemove[i];
		const auto ifaceIDX = IndexOf(m_ActiveInterfaces, iface);
		if (ifaceIDX < 0)
			continue; // Not in vector

		m_ActiveInterfaces[ifaceIDX] = nullptr;
		iface->OnDeactivate();
	}
	for (sizet i = 0; i < m_InterfacesToAdd.size(); ++i)
	{
		auto* iface = m_InterfacesToAdd[i];
		auto uuidIT = m_ActiveInterfaceUuidMap.find(iface->GetInterfaceUUID());
		sizet ifaceIDX;
		if (uuidIT == m_ActiveInterfaceUuidMap.end())
		{
			ifaceIDX = m_ActiveInterfaces.size();
			m_ActiveInterfaces.push_back(nullptr);
			m_ActiveInterfaceNameMap.insert_or_assign(iface->GetInterfaceName(), ifaceIDX);
			m_ActiveInterfaceUuidMap.insert_or_assign(iface->GetInterfaceUUID(), ifaceIDX);
		}
		else
		{
			ifaceIDX = uuidIT->second;
		}
		m_ActiveInterfaces[ifaceIDX] = iface;
		iface->OnActivate();
	}

	for (sizet i = 0; i < m_InterfaceToChange.size(); ++i)
	{
		auto* iface = m_InterfaceToChange[i];
		auto uuidIT = m_ActiveInterfaceUuidMap.find(iface->GetInterfaceUUID());
		VerifyInequal(uuidIT, m_ActiveInterfaceUuidMap.end(), "Couldn't find the Active interafce with UUID '%s' on the ActiveInterfaces.", iface->GetInterfaceUUID().ToString().c_str());
		const auto ifaceIDX = uuidIT->second;
		auto* oiFace = m_ActiveInterfaces[ifaceIDX];
		oiFace->OnChangingDefault(iface);
		oiFace->OnDeactivate();
		m_ActiveInterfaces[ifaceIDX] = iface;
		iface->OnActivate();
	}
}

void Application::UpdateTick()
{
	// We start a new Frame/Tick
	++m_FrameCount;

	UpdateActiveInterfaceList();

	// PreUpdate
	for (sizet i = 0; i < m_ActiveInterfaces.size(); ++i)
	{
		auto* iface = m_ActiveInterfaces[i];
		iface->PreUpdate();
	}

	// FixedUpdate
	uint64 step; uint32 iterations;
	ComputeFixedUpdateStep(step, iterations);
	const auto stepSecs = float(step) * 1e-9f;
	for (uint32 i = 0; i < iterations; ++i)
	{
		for (sizet j = 0; j < m_ActiveInterfaces.size(); ++j)
		{
			auto* iface = m_ActiveInterfaces[j];
			iface->FixedUpdate();
		}

		m_LastFixedUpdateTime += std::chrono::nanoseconds(step);
	}

	// Update
	for (sizet i = 0; i < m_ActiveInterfaces.size(); ++i)
	{
		auto* iface = m_ActiveInterfaces[i];
		iface->Update();
	}

	// PostUpdate
	for (sizet i = 0; i < m_ActiveInterfaces.size(); ++i)
	{
		auto* iface = m_ActiveInterfaces[i];
		iface->PostUpdate();
	}
}

void Application::ComputeFixedUpdateStep(uint64& step, uint32& iterations)
{
	const auto nextFrameTime = m_LastFixedUpdateTime + m_FixedUpdateStepNanos;
	if (nextFrameTime <= m_LastUpdateTime)
	{
		const auto simAccum = Max(m_LastUpdateTime - m_LastFixedUpdateTime, m_FixedUpdateStepNanos);
		auto stepn = m_FixedUpdateStepNanos.count();
		iterations = (uint32)DivideAndRoundUp(simAccum.count(), stepn);

		if (iterations > m_RemainingFixedUpdates)
		{
			stepn = DivideAndRoundUp(simAccum.count(), (int64)m_RemainingFixedUpdates);
			iterations = (uint32)DivideAndRoundUp(simAccum.count(), stepn);
		}
		VerifyLessEqual(iterations, m_RemainingFixedUpdates, "");
		m_RemainingFixedUpdates -= iterations;
		m_RemainingFixedUpdates = Min(MAX_ACCUM_FIXED_UPDATES, m_RemainingFixedUpdates + NEW_FIXED_UPDATES_PER_FRAME);
		step = (uint64)stepn;
	}
	step = 0;
	iterations = 0;
}

static constexpr float StoredFrameINV = 1.f / StoredFrameTimeCount;

void Application::UpdateFrameTimes()noexcept
{
	m_FrameTimes[m_FrameCount % StoredFrameTimeCount] = m_LastUpdateDelta;

	float accum = 0.f;
	for (int i = 0; i < StoredFrameTimeCount; ++i)
		accum += m_FrameTimes[i];
	
	m_UpdateDeltaAvg = accum * StoredFrameINV;
	m_UpdateDeltaMax = Max(m_UpdateDeltaMax, m_LastUpdateDelta);
	m_UpdateDeltaMin = Min(m_UpdateDeltaMin, m_LastUpdateDelta);
}

Application::Application()
	:m_Library(nullptr)
	, m_OnClose("OnClose"sv)
	, m_OnInterfaceActivation("OnInterfaceActivation"sv)
	, m_OnInitialization("OnInitialization"sv)
	, m_OnActivation("OnActivation"sv)
	,m_IsActive(false)
	,m_IsInitialized(false)
	,m_HasToStop(false)
	,m_FrameCount(0)
	,m_UpdateStep(1.f / 200.f)
	,m_UpdateStepNanos((uint64)(1e9/200.0))
	,m_FixedUpdateStep(1.f / 50.f)
	,m_FixedUpdateStepNanos((uint64)(1e9/50.0))
	,m_LastUpdateDelta(0.f)
	,m_FrameTimes()
	,m_UpdateDeltaAvg(0.f)
	,m_UpdateDeltaMin(0.f)
	,m_UpdateDeltaMax(0.f)
	,m_RemainingFixedUpdates(MAX_ACCUM_FIXED_UPDATES)
{

}

Application::~Application()
{
	// No-op
}

void Application::Initialize(IGreaperLibrary* library)
{
	if (m_IsInitialized)
		return;

	m_Library = library;

	// Reset timings...
	m_StartTime = Clock_t::now();
	ClearFrameTimes();
	m_LastUpdateDelta = m_UpdateDeltaAvg = m_UpdateDeltaMin = m_UpdateDeltaMax = 0.f;
	m_FrameCount = 0;
	m_LastUpdateTime = m_StartTime;
	m_LastFixedUpdateTime = m_StartTime;

	// Initialize...

	m_OnInitialization.Trigger(true);
	m_IsInitialized = true;
}

void Application::Deinitialize()
{
	if (!m_IsInitialized)
		return;
	
	// Deinitialize...

	m_OnInitialization.Trigger(false);
	m_IsInitialized = false;
}

void Application::OnActivate()
{
	if (m_IsActive)
		return;

	// Activate...

	m_OnActivation.Trigger(true);
	m_IsActive = true;
}

void Application::OnDeactivate()
{
	if (!m_IsActive)
		return;

	// Deactivate...

	m_OnActivation.Trigger(false);
	m_IsActive = false;
}

void Application::InitProperties()
{
	if (m_Library == nullptr)
		return; // no base library weird

	if (m_Properties.size() != (sizet)COUNT)
		m_Properties.resize((sizet)COUNT, nullptr);

	ApplicationNameProp_t* appNameProp = nullptr;
	auto result = m_Library->GetProperty(ApplicationNameName);
	if (result.IsOk())
		appNameProp = reinterpret_cast<ApplicationNameProp_t*>(result.GetValue());

	if (appNameProp == nullptr)
	{
		auto appNameResult = CreateProperty<greaper::String>(m_Library, ApplicationNameName, {}, ""sv, false, true, nullptr);
		Verify(appNameResult.IsOk(), "Couldn't create the property '%s' msg: %s", ApplicationNameName.data(), appNameResult.GetFailMessage().c_str());
		appNameProp = appNameResult.GetValue();
	}
	m_Properties[(sizet)ApplicationName] = appNameProp;

	ApplicationVersionProp_t* appVersionProp = nullptr;
	result = m_Library->GetProperty(ApplicationVersionName);
	if (result.IsOk())
		appVersionProp = reinterpret_cast<ApplicationVersionProp_t*>(result.GetValue());

	if (appVersionProp == nullptr)
	{
		auto appVersionResult = CreateProperty<uint32>(m_Library, ApplicationVersionName, 0, ""sv, false, true, nullptr);
		Verify(appVersionResult.IsOk(), "Couldn't create the property '%s' msg: %s", ApplicationVersionName.data(), appVersionResult.GetFailMessage().c_str());
		appVersionProp = appVersionResult.GetValue();
	}
	m_Properties[(sizet)ApplicationVersion] = appVersionProp;

	CompilationInfoProp_t* compilationInfoProp = nullptr;
	result = m_Library->GetProperty(CompilationInfoName);
	if (result.IsOk())
		compilationInfoProp = reinterpret_cast<CompilationInfoProp_t*>(result.GetValue());

	if (compilationInfoProp == nullptr)
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
		compilationInfoProp = comilationInfoResult.GetValue();
	}
	m_Properties[(sizet)CompilationInfo] = compilationInfoProp;

	LoadedLibrariesProp_t* loadedLibrariesProp = nullptr;
	result = m_Library->GetProperty(LoadedLibrariesName);
	if (result.IsOk())
		loadedLibrariesProp = reinterpret_cast<LoadedLibrariesProp_t*>(result.GetValue());

	if (loadedLibrariesProp == nullptr)
	{
		auto loadedLibrariesResult = CreateProperty<greaper::WStringVec>(m_Library, LoadedLibrariesName, {}, ""sv, false, true, nullptr);
		Verify(loadedLibrariesResult.IsOk(), "Couldn't create the property '%s' msg: %s", LoadedLibrariesName.data(), loadedLibrariesResult.GetFailMessage().c_str());
		loadedLibrariesProp = loadedLibrariesResult.GetValue();
	}
	m_Properties[(sizet)LoadedLibraries] = loadedLibrariesProp;

	UpdateMaxRateProp_t* updateMaxRateProp = nullptr;
	result = m_Library->GetProperty(UpdateMaxRateName);
	if (result.IsOk())
		updateMaxRateProp = reinterpret_cast<UpdateMaxRateProp_t*>(result.GetValue());

	if (updateMaxRateProp == nullptr)
	{
		auto updateMaxRateResult = CreateProperty<uint32>(m_Library, UpdateMaxRateName, 200, "Maximum amount of updates per second."sv, false, false, nullptr);
		Verify(updateMaxRateResult.IsOk(), "Couldn't create the property '%s' msg: %s", UpdateMaxRateName.data(), updateMaxRateResult.GetFailMessage().c_str());
		updateMaxRateProp = updateMaxRateResult.GetValue();
	}
	updateMaxRateProp->GetOnModificationEvent()->Connect(m_OnUpdateMaxRateEvtHnd, std::bind(&Application::OnUpdateMaxRateChange, this, std::placeholders::_1));
	m_Properties[(sizet)UpdateMaxRate] = updateMaxRateProp;

	FixedUpdateRateProp_t* fixedUpdateRateProp = nullptr;
	result = m_Library->GetProperty(FixedUpdateRateName);
	if (result.IsOk())
		fixedUpdateRateProp = reinterpret_cast<FixedUpdateRateProp_t*>(result.GetValue());

	if (fixedUpdateRateProp == nullptr)
	{
		auto fixedDeltaRateResult = CreateProperty<uint32>(m_Library, FixedUpdateRateName, 50, "Amount of fixed updates per second."sv, false, false, nullptr);
		Verify(fixedDeltaRateResult.IsOk(), "Couldn't create the property '%s' msg: %s", FixedUpdateRateName.data(), fixedDeltaRateResult.GetFailMessage().c_str());
		fixedUpdateRateProp = fixedDeltaRateResult.GetValue();
	}
	fixedUpdateRateProp->GetOnModificationEvent()->Connect(m_OnFixedUpdateMaxRateEvtHnd, std::bind(&Application::OnFixedUpdateRateChange, this, std::placeholders::_1));
	m_Properties[(sizet)FixedUpdateRate] = fixedUpdateRateProp;
}

void Application::DeinitProperties()
{

}

void Application::PreUpdate()
{
	Break("Trying to call a PreUpdate to Application, which is forbidden.");
}

void Application::Update()
{
	auto curTime = Clock_t::now();
	
	if (m_UpdateStep > 0.f)
	{
		const auto nextFrameTime = m_LastUpdateTime + std::chrono::nanoseconds(m_UpdateStepNanos);
		while (nextFrameTime > curTime)
		{
			const auto waitTimeNanos = nextFrameTime - curTime;
			const auto waitTimeMillis = (uint32)(waitTimeNanos.count() / 1'000'000);

			if (waitTimeMillis >= 2)
			{
				OSPlatform::Sleep(waitTimeMillis);
				curTime = Clock_t::now();
			}
			else
			{
				while (nextFrameTime > curTime) //! Spin to wait the exactly time needed
					curTime = Clock_t::now();
			}
		}
	}
	const auto timeDiff = curTime - m_LastUpdateTime;
	m_LastUpdateDelta = (float)((double)timeDiff.count() * 1e-9);
	m_LastUpdateTime = curTime;

	UpdateTick();
}

void Application::PostUpdate()
{
	Break("Trying to call a PostUpdate to Application, which is forbidden.");
}

void Application::FixedUpdate()
{
	Break("Trying to call a FixedUpdate to Application, which is forbidden.");
}

//void Application::SetConfig(ApplicationConfig config)
//{
//	m_Config = std::move(config);
//	// Update Properties
//	UpdateConfigProperties();
//	// Apply config
//	LoadConfigLibraries();
//}

Result<IGreaperLibrary*> Application::RegisterGreaperLibrary(const WStringView& libPath)
{
	Library lib{ libPath };

	if (!lib.IsOpen())
	{
		return CreateFailure<IGreaperLibrary*>(Format(
			"Trying to register a GreaperLibrary with path '%S', but couldn't be openned.", libPath.data()));
	}
	auto fn = lib.GetFunctionT<void*>("_Greaper"sv);
	if (fn == nullptr)
	{
		return CreateFailure<IGreaperLibrary*>(Format(
			"Trying to register a GreaperLibrary with path '%S', but does not comply with Greaper modular protocol.",
			libPath.data()));
	}
	auto gLib = reinterpret_cast<IGreaperLibrary*>(fn());
	if (gLib == nullptr)
	{
		return CreateFailure<IGreaperLibrary*>(Format(
			"Trying to register a GreaperLibrary with path '%S', but the library returned a nullptr GreaperLibrary.",
			libPath.data()));
	}
	auto uLib = GetGreaperLibrary(gLib->GetLibraryUuid());
	if (uLib.IsOk() && uLib.GetValue() != nullptr)
	{
		return CreateFailure<IGreaperLibrary*>(Format(
			"Trying to register a GreaperLibrary with path '%S', but its UUID '%s' its already registered.",
			libPath.data(), gLib->GetLibraryUuid().ToString().c_str()));
	}
	auto nLib = GetGreaperLibrary(gLib->GetLibraryName());
	if (nLib.IsOk() && nLib.GetValue() != nullptr)
	{
		return CreateFailure<IGreaperLibrary*>(Format(
			"Trying to register a GreaperLibrary with path '%S', but its name '%s' its already registered.",
			libPath.data(), gLib->GetLibraryName().data()));
	}

	AddGreaperLibrary(gLib);
	gLib->InitLibrary(Construct<Library>(lib.GetOSHandle()), this);
	gLib->InitManagers();
	gLib->InitProperties();
	gLib->InitReflection();
	return CreateResult(gLib);
}

Result<IGreaperLibrary*> Application::GetGreaperLibrary(const StringView& libraryName)
{
	if (auto findIT = m_LibraryNameMap.find(libraryName); findIT != m_LibraryNameMap.end())
	{
		if (m_Libraries.size() <= findIT->second)
		{
			return CreateFailure<IGreaperLibrary*>(Format("A GreaperLibrary with name '%s' was found, but the library list didn't have that library.", libraryName.data()));
		}
		auto& libInfo = m_Libraries[findIT->second];
		return CreateResult(libInfo.Lib);
	}

	return CreateFailure<IGreaperLibrary*>(Format("Couldn't find the GreaperLibrary '%s'.", libraryName.data()));
}

Result<IGreaperLibrary*> Application::GetGreaperLibrary(const Uuid& libraryUUID)
{
	if (const auto findIT = m_LibraryUuidMap.find(libraryUUID); findIT != m_LibraryUuidMap.end())
	{
		if (m_Libraries.size() <= findIT->second)
		{
			return CreateFailure<IGreaperLibrary*>(Format("A GreaperLibrary with UUID '%s' was found, but the library list didn't have that library.", libraryUUID.ToString().c_str()));
		}
		auto& libInfo = m_Libraries[findIT->second];
		return CreateResult(libInfo.Lib);
	}

	return CreateFailure<IGreaperLibrary*>(Format("Couldn't find the GreaperLibrary '%s'.", libraryUUID.ToString().c_str()));
}

EmptyResult Application::UnregisterGreaperLibrary(IGreaperLibrary* library)
{
	if (library == nullptr)
		return CreateEmptyFailure("Trying to unregister a nullptr GreaperLibrary"sv);
	
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

	if (nIndex != uIndex)
	{
		m_Library->LogWarning(Format("Trying to unregister a GreaperLibrary '%s', but its name and uuid points to different indices.", library->GetLibraryName().data()));
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
		return CreateEmptyFailure(Format("Trying to unregister a GreaperLibrary '%s', but it was not registered.", library->GetLibraryName().data()));

	auto& libInfo = m_Libraries[index];
	m_Library->Log(Format("Unregistering GraeperLibrary '%s'.", library->GetLibraryName().data()));
	for (auto*& iface : libInfo.Interfaces)
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
	libInfo.Lib->DeinitReflection();
	libInfo.Lib->DeinitManagers();
	libInfo.Lib->DeinitLibrary();
	libInfo.Lib = nullptr;
	libInfo.IntefaceNameMap.clear();
	libInfo.InterfaceUuidMap.clear();
	libInfo.Interfaces.clear();
	return CreateEmptyResult();
}

EmptyResult Application::RegisterInterface(IInterface* interface)
{
	if (interface == nullptr)
		return CreateEmptyFailure("Trying to register a nullptr interface.");

	auto* glib = interface->GetLibrary();
	if (glib == nullptr)
	{
		return CreateEmptyFailure("Trying to register an Interface without GreaperLibrary.");
	}
	
	const auto findIT = m_LibraryUuidMap.find(glib->GetLibraryUuid());
	if (findIT == m_LibraryUuidMap.end())
		return CreateEmptyFailure("Trying to register an Interface with a non-registered GreaperLibrary.");
	
	auto& libInfo = m_Libraries[findIT->second];
	
	if (libInfo.Lib != glib)
		return CreateEmptyFailure("Trying to register an Interface with a GreaperLibrary which UUID points to another GreaperLibrary.");
	
	if (const auto nameIT = libInfo.IntefaceNameMap.find(interface->GetInterfaceName()); nameIT != libInfo.IntefaceNameMap.end())
	{
		return CreateEmptyFailure("Trying to register an Interface, but that GreaperLibrary already has an Interface registered with the same name.");
	}

	if (const auto uuidIT = libInfo.InterfaceUuidMap.find(interface->GetInterfaceUUID()); uuidIT != libInfo.InterfaceUuidMap.end())
	{
		return CreateEmptyFailure("Trying to register an Interface, but that GreaperLibrary already has an Interface registered with the same UUID.");
	}

	const auto index = libInfo.Interfaces.size();
	libInfo.Interfaces.push_back(interface);
	libInfo.IntefaceNameMap.insert_or_assign(interface->GetInterfaceName(), index);
	libInfo.InterfaceUuidMap.insert_or_assign(interface->GetInterfaceUUID(), index);

	if (!interface->IsInitialized())
		interface->Initialize(glib);
	return CreateEmptyResult();
}

EmptyResult Application::UnregisterInterface(IInterface* interface)
{
	if (interface == nullptr)
		return CreateEmptyFailure("Trying to unregister a nullptr interface.");

	auto* glib = interface->GetLibrary();
	if (glib == nullptr)
	{
		return CreateEmptyFailure("Trying to unregister an Interface without GreaperLibrary.");
	}

	const auto findIT = m_LibraryUuidMap.find(glib->GetLibraryUuid());
	if (findIT == m_LibraryUuidMap.end())
		return CreateEmptyFailure("Trying to unregister an Interface with a non-registered GreaperLibrary.");

	auto& libInfo = m_Libraries[findIT->second];

	if (libInfo.Lib != glib)
		return CreateEmptyFailure("Trying to unregister an Interface with a GreaperLibrary which UUID points to another GreaperLibrary.");

	const auto nameIT = libInfo.IntefaceNameMap.find(interface->GetInterfaceName());
	const auto uuidIT = libInfo.InterfaceUuidMap.find(interface->GetInterfaceUUID());

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

	if (nIndex != uIndex)
	{
		m_Library->LogWarning(Format("Trying to unregister an Interface '%s', but its name and uuid points to different indices.", interface->GetInterfaceName().data()));
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
		return CreateEmptyFailure(Format("Trying to unregister an Interface '%s', but it was not registered.", interface->GetInterfaceName().data()));

	m_Library->Log(Format("Unregistering an Interface '%s' from '%s' GreaperLibrary.", interface->GetInterfaceName().data(), glib->GetLibraryName().data()));

	if (interface->IsActive())
		DeactivateInterface(interface->GetInterfaceUUID());
	if (interface->IsInitialized())
		interface->Deinitialize();
	libInfo.Interfaces[index] = nullptr;
	return CreateEmptyResult();
}

EmptyResult Application::ActivateInterface(IInterface* interface)
{
	if (interface == nullptr)
		return CreateEmptyFailure("Trying to make default a nullptr interface, if you want to remove an Active interface call StopInterfaceDefault.");

	LOCK(m_ActiveMutex);
	
	const auto toRemoveIdx = IndexOf(m_InterfacesToRemove, interface);
	if (toRemoveIdx >= 0)
	{
		m_InterfacesToRemove.erase(m_InterfacesToRemove.begin() + toRemoveIdx);
	}

	if (Contains(m_InterfaceToChange, interface))
	{
		return CreateEmptyResult();
	}

	if(Contains(m_InterfacesToAdd, interface))
	{
		return CreateEmptyResult();
	}

	const auto existsAnother = m_ActiveInterfaceUuidMap.contains(interface->GetInterfaceUUID());

	if (existsAnother)
		m_InterfaceToChange.push_back(interface);
	else
		m_InterfacesToAdd.push_back(interface);

	return CreateEmptyResult();
}

EmptyResult Application::DeactivateInterface(const Uuid& interfaceUUID)
{
	LOCK(m_ActiveMutex);
	IInterface* interface = nullptr;
	// Remove it from toAdd and toChange
	bool removed = false;
	for (sizet i = 0; i < m_InterfacesToAdd.size(); ++i)
	{
		auto* iface = m_InterfacesToAdd[i];
		if (iface->GetInterfaceUUID() == interfaceUUID)
		{
			m_InterfacesToAdd.erase(m_InterfacesToAdd.begin() + i);
			removed = true;
			break;
		}
	}
	for (sizet i = 0; i < m_InterfaceToChange.size(); ++i)
	{
		auto* iface = m_InterfaceToChange[i];
		if (iface->GetInterfaceUUID() == interfaceUUID)
		{
			m_InterfaceToChange.erase(m_InterfaceToChange.begin() + i);
			removed = true;
			break;
		}
	}

	const auto activeIT = m_ActiveInterfaceUuidMap.find(interfaceUUID);
	if (activeIT != m_ActiveInterfaceUuidMap.end())
	{
		interface = m_ActiveInterfaces[activeIT->second];
	}

	if (interface == nullptr)
	{
		if (removed)
			return CreateEmptyResult(); // removed from activation

		return CreateEmptyFailure(Format("Trying to deactivate an Interface with UUID '%s', but was not active.", interfaceUUID.ToString().c_str()));
	}
	m_InterfacesToRemove.push_back(interface);

	return CreateEmptyResult();
}

EmptyResult Application::DeactivateInterface(const StringView& interfaceName)
{
	LOCK(m_ActiveMutex);
	IInterface* interface = nullptr;
	// Remove it from toAdd and toChange
	bool removed = false;
	for (sizet i = 0; i < m_InterfacesToAdd.size(); ++i)
	{
		auto* iface = m_InterfacesToAdd[i];
		if (iface->GetInterfaceName() == interfaceName)
		{
			m_InterfacesToAdd.erase(m_InterfacesToAdd.begin() + i);
			removed = true;
			break;
		}
	}
	for (sizet i = 0; i < m_InterfaceToChange.size(); ++i)
	{
		auto* iface = m_InterfaceToChange[i];
		if (iface->GetInterfaceName() == interfaceName)
		{
			m_InterfaceToChange.erase(m_InterfaceToChange.begin() + i);
			removed = true;
			break;
		}
	}
	const auto activeIT = m_ActiveInterfaceNameMap.find(interfaceName);
	if (activeIT != m_ActiveInterfaceNameMap.end())
	{
		interface = m_ActiveInterfaces[activeIT->second];
	}

	if (interface == nullptr)
	{
		if (removed)
			return CreateEmptyResult(); // Removed from activation

		return CreateEmptyFailure(Format("Trying to deactivate an Interface with name '%s', but was not active.", interfaceName.data()));
	}

	m_InterfacesToRemove.push_back(interface);

	return CreateEmptyResult();
}

Result<IInterface*> Application::GetActiveInterface(const Uuid& interfaceUUID) const
{
	LOCK(m_ActiveMutex);

	const auto uuidIT = m_ActiveInterfaceUuidMap.find(interfaceUUID);
	if (uuidIT == m_ActiveInterfaceUuidMap.end())
		return CreateFailure<IInterface*>(Format("Couldn't find an active Interface with UUID '%s'.", interfaceUUID.ToString().c_str()));

	const auto index = uuidIT->second;

	if (index >= m_ActiveInterfaces.size())
		return CreateFailure<IInterface*>(Format("Interface with UUID '%s', its index was out of bounds.", interfaceUUID.ToString().c_str()));

	auto* iface = m_ActiveInterfaces[index];

	return CreateResult(iface);
}

Result<IInterface*> Application::GetActiveInterface(const StringView& interfaceName) const
{
	LOCK(m_ActiveMutex);

	const auto nameIT = m_ActiveInterfaceNameMap.find(interfaceName);
	if (nameIT == m_ActiveInterfaceNameMap.end())
		return CreateFailure<IInterface*>(Format("Couldn't find an active Interface with name '%s'.", interfaceName.data()));

	const auto index = nameIT->second;

	if (index >= m_ActiveInterfaces.size())
		return CreateFailure<IInterface*>(Format("Interface with name '%s', its index was out of bounds.", interfaceName.data()));

	auto* iface = m_ActiveInterfaces[index];

	return CreateResult(iface);
}

Result<IInterface*> Application::GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID) const
{
	const auto libUuidIT = m_LibraryUuidMap.find(libraryUUID);
	if (libUuidIT == m_LibraryUuidMap.end())
	{
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the library is not registered.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));
	}

	if(libUuidIT->second >= m_Libraries.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the library index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));

	auto& libInfo = m_Libraries[libUuidIT->second];

	const auto ifaceUuidIT = libInfo.InterfaceUuidMap.find(interfaceUUID);

	if(ifaceUuidIT == libInfo.InterfaceUuidMap.end())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the interface is not registered.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));

	if(ifaceUuidIT->second >= libInfo.Interfaces.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with UUID '%s', but the interface index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryUUID.ToString().c_str()));

	auto* iface = libInfo.Interfaces[ifaceUuidIT->second];
	return CreateResult(iface);
}

Result<IInterface*> Application::GetInterface(const StringView& interfaceName, const StringView& libraryName) const
{
	const auto libNameIT = m_LibraryNameMap.find(libraryName);
	if (libNameIT == m_LibraryNameMap.end())
	{
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the library is not registered.", interfaceName.data(), libraryName.data()));
	}

	if (libNameIT->second >= m_Libraries.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the library index is outside of bounds.", interfaceName.data(), libraryName.data()));

	auto& libInfo = m_Libraries[libNameIT->second];

	const auto ifaceNameIT = libInfo.IntefaceNameMap.find(interfaceName);

	if (ifaceNameIT == libInfo.IntefaceNameMap.end())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the interface is not registered.", interfaceName.data(), libraryName.data()));

	if (ifaceNameIT->second >= libInfo.Interfaces.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with name '%s', but the interface index is outside of bounds.", interfaceName.data(), libraryName.data()));

	auto* iface = libInfo.Interfaces[ifaceNameIT->second];
	return CreateResult(iface);
}

Result<IInterface*> Application::GetInterface(const Uuid& interfaceUUID, const StringView& libraryName) const
{
	const auto libNameIT = m_LibraryNameMap.find(libraryName);
	if (libNameIT == m_LibraryNameMap.end())
	{
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the library is not registered.", interfaceUUID.ToString().c_str(), libraryName.data()));
	}

	if (libNameIT->second >= m_Libraries.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the library index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryName.data()));

	auto& libInfo = m_Libraries[libNameIT->second];

	const auto ifaceUuidIT = libInfo.InterfaceUuidMap.find(interfaceUUID);

	if (ifaceUuidIT == libInfo.InterfaceUuidMap.end())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the interface is not registered.", interfaceUUID.ToString().c_str(), libraryName.data()));

	if (ifaceUuidIT->second >= libInfo.Interfaces.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with UUID '%s' from a library with name '%s', but the interface index is outside of bounds.", interfaceUUID.ToString().c_str(), libraryName.data()));

	auto* iface = libInfo.Interfaces[ifaceUuidIT->second];
	return CreateResult(iface);
}

Result<IInterface*> Application::GetInterface(const StringView& interfaceName, const Uuid& libraryUUID) const
{
	const auto libUuidIT = m_LibraryUuidMap.find(libraryUUID);
	if (libUuidIT == m_LibraryUuidMap.end())
	{
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the library is not registered.", interfaceName.data(), libraryUUID.ToString().c_str()));
	}

	if (libUuidIT->second >= m_Libraries.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the library index is outside of bounds.", interfaceName.data(), libraryUUID.ToString().c_str()));

	auto& libInfo = m_Libraries[libUuidIT->second];

	const auto ifaceNameIT = libInfo.IntefaceNameMap.find(interfaceName);

	if (ifaceNameIT == libInfo.IntefaceNameMap.end())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the interface is not registered.", interfaceName.data(), libraryUUID.ToString().c_str()));

	if (ifaceNameIT->second >= libInfo.Interfaces.size())
		return CreateFailure<IInterface*>(Format("Trying to get an interface with name '%s' from a library with UUID '%s', but the interface index is outside of bounds.", interfaceName.data(), libraryUUID.ToString().c_str()));

	auto* iface = libInfo.Interfaces[ifaceNameIT->second];
	return CreateResult(iface);
}

void Application::StopApplication()
{
	m_Library->Log(Format("Stopping %s...", GetApplicationName()->GetValue().c_str()));

	m_HasToStop = true;
	m_OnClose.Trigger();
}
