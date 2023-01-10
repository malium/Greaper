/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "GreaperDisplayDLL.h"
#include "WindowManager.h"
#include <Core/Platform.h>
#include <Core/FileStream.h>
#include <Core/Reflection/Property.h>
#include <External/SDL/SDL.h>

#if GREAPER_DISP_DLL

greaper::SPtr<greaper::disp::GreaperDispLibrary> gDispLibrary{};
extern greaper::SPtr<greaper::disp::WindowManager> gWindowManager;

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
const auto configFilePath = configPath / "GreaperDisplay.json";

void* _Greaper()
{
	if (gDispLibrary == nullptr)
	{
		gDispLibrary.reset(greaper::Construct<greaper::disp::GreaperDispLibrary>());
		greaper::OSPlatform::PerLibraryInit();
	}
	return &gDispLibrary;
}

void greaper::disp::GreaperDispLibrary::Initialize() noexcept
{
	auto rtn = SDL_Init(SDL_INIT_VIDEO);
	VerifyGreaterEqual(rtn, 0, "Couldn't initialize SDL, error: %s.", SDL_GetError());
}

void greaper::disp::GreaperDispLibrary::InitManagers()noexcept
{
	// add more managers
	gWindowManager.reset(Construct<WindowManager>());
	m_Managers.push_back((PInterface)gWindowManager);




	for (const auto& mgr : m_Managers)
	{
		mgr->Initialize((WGreaperLib)gDispLibrary);
		m_Application->RegisterInterface(mgr);
	}
}

void greaper::disp::GreaperDispLibrary::InitProperties()noexcept
{
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

void greaper::disp::GreaperDispLibrary::DeinitProperties()noexcept
{
	for (auto it = m_Managers.rbegin(); it < m_Managers.rend(); ++it)
		(*it)->DeinitProperties();

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

void greaper::disp::GreaperDispLibrary::DeinitManagers()noexcept
{
	for (auto it = m_Managers.rbegin(); it < m_Managers.rend(); ++it)
	{
		auto& mgr = (*it);
		if (mgr->IsActive())
			m_Application->DeactivateInterface(mgr->GetInterfaceUUID());
		m_Application->UnregisterInterface(mgr);

		mgr.reset();
	}

	gWindowManager.reset();
	m_Managers.clear();
	m_Application.reset();
}

void greaper::disp::GreaperDispLibrary::Deinitialize()noexcept
{
	SDL_Quit();

	gDispLibrary.reset();
}

#else

#endif // GREAPER_DISP_DLL