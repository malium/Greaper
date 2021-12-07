/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include <Core/CorePrerequisites.h>
#include <Core/IGreaperLibrary.h>
#include <Core/IApplication.h>
#include <iostream>

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

#define CORE_LIBRARY_NAME "Core" ARCH_NAME "_" CONF_NAME ".dll"
constexpr greaper::StringView CORE_LIB_NAME = CORE_LIBRARY_NAME;
constexpr greaper::StringView LibFnName = "_Greaper";
greaper::Library* gCoreLib = nullptr;
greaper::IGreaperLibrary* gCore = nullptr;
greaper::IApplication* gApplication = nullptr;

int main()
{
	gCoreLib = greaper::Construct<greaper::Library>(CORE_LIB_NAME);
	auto libFN = gCoreLib->GetFunctionT<void*>(LibFnName);

	// init Greaper
	gCore = static_cast<greaper::IGreaperLibrary*>(libFN());
	gCore->InitLibrary(gCoreLib, nullptr);
	gCore->InitManagers();
	gCore->InitProperties();
	gCore->InitReflection();

	gApplication = gCore->GetApplication();
	//gApplication->SetConfig({.ApplicationName = "TestApplication"sv, .ApplicationVersion = 0, .GreaperLibraryCount = 0, .GreaperLibraries = nullptr, });

	std::cout << "Successfully started " << gApplication->GetApplicationName() << " Version " << gApplication->GetApplicationVersion() << std::endl;

	gApplication->StartApplication();
	while (!gApplication->AppHasToStop())
	{
		gApplication->Update();
	}

	gApplication->StopApplication();

	gCore->DeinitReflection();
	gCore->DeinitProperties();
	gCore->DeinitManagers();
	gCore->DeinitLibrary();
	
	gApplication = nullptr;
	gCore = nullptr;
	gCoreLib->Close();
	greaper::Destroy(gCoreLib);
	gCoreLib = nullptr;

	return EXIT_SUCCESS;
}
