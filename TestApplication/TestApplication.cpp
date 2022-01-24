/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include <Core/CorePrerequisites.h>
#include <Core/IGreaperLibrary.h>
#include <Core/IApplication.h>
#include <Core/Reflection/ReflectedPlainContainer.h>
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

#define APPLICATION_VERSION VERSION_SETTER(1, 0, 0, 0)

#define TRYEXP(exp, msg) { if(!exp) { std::cout << msg << "\nEnter any character to stop app." << std::endl; int a; std::cin >> a; exit(EXIT_FAILURE); } }

int main()
{
	using namespace greaper;
	gCoreLib = Construct<Library>(CORE_LIB_NAME);
	TRYEXP(gCoreLib->IsOpen(), "Couldn't open " CORE_LIBRARY_NAME);

	auto libFN = gCoreLib->GetFunctionT<void*>(LibFnName);
	TRYEXP(libFN, CORE_LIBRARY_NAME " does not have the _Greaper function");

	// init Greaper
	gCore = static_cast<IGreaperLibrary*>(libFN());
	gCore->Initialize(gCoreLib, nullptr);

	gApplication = gCore->GetApplication();
	TRYEXP(gApplication, CORE_LIBRARY_NAME " does not have an Application.");

	gApplication->GetApplicationName()->SetValue(String{ "TestApplication"sv });
	gApplication->GetApplicationVersion()->SetValue(APPLICATION_VERSION);
	gApplication->GetLoadedLibrariesNames()->SetValue({});
	std::cout << "Successfully started!\n " << gApplication->GetApplicationName()->GetStringValue() << " Version " << gApplication->GetApplicationVersion()->GetStringValue() << std::endl;
	
	std::cout << "Enter anything to shutdown" << std::endl;
	achar a;
	std::cin >> a;

	gApplication->StopApplication();

	gCore->Deinitialize();
	
	gApplication = nullptr;
	gCore = nullptr;
	gCoreLib->Close();
	Destroy(gCoreLib);
	gCoreLib = nullptr;

	return EXIT_SUCCESS;
}
