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
greaper::PLibrary gCoreLib;
greaper::PGreaperLib gCore;
greaper::PApplication gApplication;

#define APPLICATION_VERSION VERSION_SETTER(1, 0, 0, 0)

#define TRYEXP(exp, msg) { if(!exp) { DEBUG_OUTPUT(msg); TRIGGER_BREAKPOINT(); exit(EXIT_FAILURE); } }

static int MainCode(void* hInstance, int argc, char** argv);

#if PLT_WINDOWS
#ifdef _CONSOLE
int main(int argc, char* argv[])
{
	void* hInstance = nullptr;
	hInstance = (void*)GetModuleHandleW(nullptr);
	return MainCode(hInstance, argc, argv);
}
#else /* !_CONSOLE */
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UNUSED(nShowCmd);
	UNUSED(hPrevInstance);
	int argc = 0;

	auto argv = greaper::UPtr<LPSTR>(CommandLineToArgvA(lpCmdLine, &argc), &FreeArgvA);
	if (argv == nullptr)
		return EXIT_FAILURE;
	int retVal = MainCode(hInstance, argc, argv.Get());
	return retVal;
}
#endif 
#else /* !PLT_WINDOWS */
int main(int argc, char* argv[])
{
	void* hInstance = nullptr;
	return MainCode(nullptr, argc, argv);
}
#endif

void Init(void* hInstance, int argc, char** argv)
{
	using namespace greaper;
	gCoreLib.Reset(Construct<Library>(CORE_LIB_NAME));
	TRYEXP(gCoreLib->IsOpen(), "Couldn't open " CORE_LIBRARY_NAME);

	auto libFN = gCoreLib->GetFunctionT<void*>(LibFnName);
	TRYEXP(libFN, CORE_LIBRARY_NAME " does not have the _Greaper function.");

	// init Greaper
	auto* corePtr = static_cast<PGreaperLib*>(libFN());
	TRYEXP(corePtr, CORE_LIBRARY_NAME " does not return a IGreaperLibrary.");
	gCore = *corePtr;

	auto propAppInstanceRes = CreateProperty<ptruint>(gCore, IApplication::AppInstanceName, (ptruint)hInstance, ""sv, true, true, nullptr);

	StringVec commandLine;
	commandLine.resize(argc);
	for (int32 i = 0; i < argc; ++i)
		commandLine[i] = String{ argv[i] };
	auto propCmdLineRes = CreateProperty<StringVec>(gCore, IApplication::CommandLineName, std::move(commandLine), ""sv, true, true, nullptr);

	gCore->Initialize(gCoreLib, WApplication());

	auto app = gCore->GetApplication();
	TRYEXP(!app.Expired(), CORE_LIBRARY_NAME " does not have an Application.");
	gApplication = app.Lock();
	gApplication->GetApplicationName().Lock()->SetValue(String{ "TestApplication"sv });
	gApplication->GetApplicationVersion().Lock()->SetValue(APPLICATION_VERSION);
	gApplication->GetLoadedLibrariesNames().Lock()->SetValue({});
}

void Deinit()
{
	gApplication.Reset(nullptr);
	gCore->Deinitialize();
	gCore.Reset(nullptr);

	gCoreLib->Close();
	gCoreLib.Reset(nullptr);
}

int MainCode(void* hInstance, int argc, char** argv)
{
	Init(hInstance, argc, argv);
	
	std::cout << "Successfully started!\n " << gApplication->GetApplicationName().Lock()->GetStringValue() << " Version " << gApplication->GetApplicationVersion().Lock()->GetStringValue() << std::endl;
	
	std::cout << "Enter anything to shutdown" << std::endl;
	achar a;
	std::cin >> a;

	Deinit();

	return EXIT_SUCCESS;
}
