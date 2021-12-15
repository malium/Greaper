/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include <Core/CorePrerequisites.h>
#include <Core/IGreaperLibrary.h>
#include <Core/IApplication.h>
#include <Core/Property.h>
#include <iostream>

#if PLT_WINDOWS
#define PLT_NAME "Win"
#else
#define PLT_NAME "Lnx"
#endif
#if ARCHITECTURE_X64
#define ARCH_NAME "64"
#else
#define ARCH_NAME "32"
#endif
#if GREAPER_DEBUG
#define CONF_NAME "DebugDLL"
#else
#define CONF_NAME "ReleaseDLL"
#endif

#define CORE_LIBRARY_NAME "Core" PLT_NAME ARCH_NAME "_" CONF_NAME GREAPER_DLLEXT
constexpr greaper::StringView CORE_LIB_NAME = CORE_LIBRARY_NAME;
constexpr greaper::StringView LibFnName = "_Greaper";
greaper::Library* gCoreLib = nullptr;
greaper::IGreaperLibrary* gCore = nullptr;
greaper::IApplication* gApplication = nullptr;

int main()
{
	using namespace greaper;
	gCoreLib = Construct<Library>(CORE_LIB_NAME);
	auto libFN = gCoreLib->GetFunctionT<void*>(LibFnName);

	// init Greaper
	gCore = static_cast<IGreaperLibrary*>(libFN());
	gCore->InitLibrary(gCoreLib, nullptr);
	gCore->InitManagers();
	gCore->InitProperties();
	gCore->InitReflection();

	gApplication = gCore->GetApplication();
	//gApplication->SetConfig({.ApplicationName = "TestApplication"sv, .ApplicationVersion = 0, .GreaperLibraryCount = 0, .GreaperLibraries = nullptr, });
	gApplication->GetApplicationName()->SetValue(String{ "TestApplication"sv });
	gApplication->GetApplicationVersion()->SetValue(0);
	gApplication->GetLoadedLibrariesNames()->SetValue({});
	std::cout << "Successfully started!\n " << gApplication->GetApplicationName()->GetStringValue() << " Version " << gApplication->GetApplicationVersion()->GetStringValue() << std::endl;
	
	std::cout << "Enter anything to shutdown" << std::endl;
	gApplication->StartApplication();
	achar a;
	std::cin >> a;

	gApplication->StopApplication();

	gCore->DeinitReflection();
	gCore->DeinitProperties();
	gCore->DeinitManagers();
	gCore->DeinitLibrary();
	
	gApplication = nullptr;
	gCore = nullptr;
	gCoreLib->Close();
	Destroy(gCoreLib);
	gCoreLib = nullptr;

	return EXIT_SUCCESS;
}
