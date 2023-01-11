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

void* _Greaper()
{
	if (gCoreLibrary == nullptr)
	{
		gCoreLibrary.reset(greaper::Construct<greaper::core::GreaperCoreLibrary>());
		greaper::OSPlatform::PerLibraryInit();
	}
	return &gCoreLibrary;
}

void greaper::core::GreaperCoreLibrary::AddManagers() noexcept
{
	m_Application.reset(Construct<Application>());
	gApplication = m_Application;
	m_Application->Initialize((WGreaperLib)gCoreLibrary);

	gThreadManager.reset(Construct<ThreadManager>());
	m_Managers.push_back((PInterface)gThreadManager);
	gLogManager.reset(Construct<LogManager>());
	m_Managers.push_back((PInterface)gLogManager);
	gCommandManager.reset(Construct<CommandManager>());
	m_Managers.push_back((PInterface)gCommandManager);
}

void greaper::core::GreaperCoreLibrary::AddProperties() noexcept
{
	m_Application->InitProperties();
	IGreaperLibrary::AddProperties();
}

void greaper::core::GreaperCoreLibrary::RemoveProperties() noexcept
{
	m_Application->DeinitProperties();
	IGreaperLibrary::RemoveProperties();
}

void greaper::core::GreaperCoreLibrary::RemoveManagers() noexcept
{
	IGreaperLibrary::RemoveManagers();
	gCommandManager.reset();
	gLogManager.reset();
	gThreadManager.reset();
	m_Application->Deinitialize();
}

void greaper::core::GreaperCoreLibrary::Deinitialize()noexcept
{
	gCoreLibrary.reset();
}

#else

#endif