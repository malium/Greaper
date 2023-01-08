/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "GreaperDisplayDLL.h"
#include "WindowManager.h"
#include <Core/Platform.h>
#include <Core/FileStream.h>
#include <Core/Reflection/Property.h>

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

greaper::EmptyResult greaper::disp::GreaperDispLibrary::InitMiniSDL() noexcept
{
	TResult<FuncPtr> fnRes;
#define GET_FUNC(fnName)\
	fnRes = m_SDLlib.GetFunction(#fnName##sv);\
	if (fnRes.HasFailed()) return Result::CopyFailure(fnRes);\
	m_SDL.fnName = reinterpret_cast<decltype(m_SDL.fnName)>(fnRes.GetValue());

	GET_FUNC(SDL_Init);
	GET_FUNC(SDL_Quit);
	GET_FUNC(SDL_GetError);
	GET_FUNC(SDL_CreateWindow);
	GET_FUNC(SDL_DestroyWindow);
	GET_FUNC(SDL_PollEvent);
	GET_FUNC(SDL_StartTextInput);
	GET_FUNC(SDL_StopTextInput);
	GET_FUNC(SDL_GetMouseState);
	GET_FUNC(SDL_GetKeyboardState);
	GET_FUNC(SDL_GL_CreateContext);
	GET_FUNC(SDL_GL_DeleteContext);

	return Result::CreateSuccess();
#undef GET_FUNC
}

void greaper::disp::GreaperDispLibrary::Initialize() noexcept
{
	auto res = m_SDLlib.Open(L"SDL.dll"sv);
	Verify(res.IsOk(), "Trying to open SDL.dll but something happened: %s.", res.GetFailMessage().c_str());

	res = InitMiniSDL();
	Verify(res.IsOk(), "Trying to initialize MiniSDL but something happened: %s.", res.GetFailMessage().c_str());
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
	gDispLibrary.reset();
}

#else

#endif // GREAPER_DISP_DLL