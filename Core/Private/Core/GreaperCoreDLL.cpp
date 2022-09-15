/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "GreaperCoreDLL.h"
#include "Application.h"
#include "LogManager.h"
#include "ThreadManager.h"
#include <Core/Property.h>

#if GREAPER_CORE_DLL
//extern greaper::core::GreaperCoreLibrary* gCoreLibrary = nullptr;
extern greaper::SPtr<greaper::core::GreaperCoreLibrary> gCoreLibrary = greaper::SPtr<greaper::core::GreaperCoreLibrary>();
#if PLT_WINDOWS
#define DLL_PROCESS_ATTACH   1
#define DLL_THREAD_ATTACH    2
#define DLL_THREAD_DETACH    3
#define DLL_PROCESS_DETACH   0

int __stdcall DllMain(HINSTANCE hModule,
	unsigned long  ul_reason_for_call,
	void* lpReserved
)
{
	UNUSED(hModule);
	UNUSED(lpReserved);
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
DLLEXPORT void* CDECL _Greaper();
END_C

void* _Greaper()
{
	if (gCoreLibrary == nullptr)
	{
		gCoreLibrary.reset(greaper::Construct<greaper::core::GreaperCoreLibrary>());
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
	m_Application->Initialize((WGreaperLib)gCoreLibrary);

	// add more managers
	m_Managers.push_back(SPtr<IInterface>(Construct<LogManager>()));
	m_Managers.push_back(SPtr<IInterface>(Construct<ThreadManager>()));



	for (auto mgr : m_Managers)
	{
		mgr->Initialize((WGreaperLib)gCoreLibrary);
		m_Application->RegisterInterface(mgr);
	}
}

void greaper::core::GreaperCoreLibrary::InitProperties()noexcept
{
	m_Application->InitProperties();
	for (auto mgr : m_Managers)
		mgr->InitProperties();
}

void greaper::core::GreaperCoreLibrary::InitReflection()noexcept
{

}

void greaper::core::GreaperCoreLibrary::DeinitReflection()noexcept
{

}

void greaper::core::GreaperCoreLibrary::DeinitProperties()noexcept
{
	for (auto it = m_Managers.rbegin(); it < m_Managers.rend(); ++it)
		(*it)->DeinitProperties();

	m_Application->DeinitProperties();

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