/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "GreaperCoreDLL.h"
#include "Application.h"
#include "LogManager.h"
#include "ThreadManager.h"
#include "CommandManager.h"
#include <Core/FileStream.h>

#if GREAPER_CORE_DLL

greaper::SPtr<greaper::core::GreaperCoreLibrary> gCoreLibrary = {};
extern greaper::SPtr<greaper::core::Application> gApplication;
extern greaper::SPtr<greaper::core::ThreadManager> gThreadManager;
extern greaper::SPtr<greaper::core::LogManager> gLogManager;
extern greaper::SPtr<greaper::core::CommandManager> gCommandManager;

#if PLT_WINDOWS
#define DLL_PROCESS_ATTACH   1
#define DLL_THREAD_ATTACH    2
#define DLL_THREAD_DETACH    3
#define DLL_PROCESS_DETACH   0

int __stdcall DllMain(UNUSED HINSTANCE hModule,
	unsigned long  ul_reason_for_call,
	UNUSED void* lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return 1;
}
#endif
BEGIN_C
DLLEXPORT void* _Greaper();
END_C

const auto configPath = std::filesystem::current_path() / "Config";
const auto configFilePath = configPath / "GreaperCore.json";

void* _Greaper()
{
	if (gCoreLibrary == nullptr)
	{
		gCoreLibrary.reset(greaper::Construct<greaper::core::GreaperCoreLibrary>());
		greaper::OSPlatform::PerLibraryInit();
	}
	return &gCoreLibrary;
}

void greaper::core::GreaperCoreLibrary::Initialize() noexcept
{

}

void greaper::core::GreaperCoreLibrary::InitManagers()noexcept
{
	if (m_Application != nullptr)
	{
		LogError(Format("Trying to Initialize the Managers of the GreaperLibrary '%s', but IApplication was already initialized.", LibraryName.data()));
		return;
	}

	m_Application.reset(Construct<Application>());
	gApplication = m_Application;
	m_Application->Initialize((WGreaperLib)gCoreLibrary);

	// add more managers
	gThreadManager.reset(Construct<ThreadManager>());
	m_Managers.push_back((PInterface)gThreadManager);
	gLogManager.reset(Construct<LogManager>());
	m_Managers.push_back((PInterface)gLogManager);
	gCommandManager.reset(Construct<CommandManager>());
	m_Managers.push_back((PInterface)gCommandManager);




	for (const auto& mgr : m_Managers)
	{
		mgr->Initialize((WGreaperLib)gCoreLibrary);
		m_Application->RegisterInterface(mgr);
	}
}

void greaper::core::GreaperCoreLibrary::InitProperties()noexcept
{
	m_Application->InitProperties();
	for (const auto& mgr : m_Managers)
		mgr->InitProperties();

	std::filesystem::create_directories(configPath);
	auto stream = FileStream(configFilePath, FileStream::READ);
	String fileTxt{};
	fileTxt.resize(stream.Size());
	stream.Read(fileTxt.data(), stream.Size());
	auto json = SPtr<cJSON>(cJSON_Parse(fileTxt.c_str()), cJSON_Delete);
	for (auto& prop : m_Properties)
	{
		if (prop->IsStatic())
			continue;
		refl::ComplexType<IProperty>::FromJSON(*prop, json.get(), prop->GetPropertyName().c_str());
	}
}

void greaper::core::GreaperCoreLibrary::DeinitProperties()noexcept
{
	for (auto it = m_Managers.rbegin(); it < m_Managers.rend(); ++it)
		(*it)->DeinitProperties();

	m_Application->DeinitProperties();

	auto json = SPtr<cJSON>(cJSON_CreateObject(), cJSON_Delete);
	for (const auto& prop : m_Properties)
	{
		if (prop->IsStatic())
			continue;
		refl::ComplexType<IProperty>::ToJSON(*prop, json.get(), prop->GetPropertyName());
	}
	std::filesystem::create_directories(configPath);
	std::filesystem::remove(configFilePath);
	auto stream = FileStream(configFilePath, FileStream::READ | FileStream::WRITE);
	auto text = SPtr<char>(cJSON_Print(json.get()));
	stream.Write(text.get(), strlen(text.get()));
	
	m_Properties.clear();
	m_PropertyMap.clear();
}

void greaper::core::GreaperCoreLibrary::DeinitManagers()noexcept
{
	for (auto it = m_Managers.rbegin(); it < m_Managers.rend(); ++it)
	{
		auto& mgr = (*it);
		if (mgr->IsActive())
			m_Application->DeactivateInterface(mgr->GetInterfaceUUID());
		m_Application->UnregisterInterface(mgr);

		mgr.reset();
	}

	gCommandManager.reset();
	gLogManager.reset();
	gThreadManager.reset();
	m_Application->Deinitialize();

	m_Managers.clear();
	m_Application.reset();
}

void greaper::core::GreaperCoreLibrary::Deinitialize()noexcept
{
	gCoreLibrary.reset();
}

#else

#endif