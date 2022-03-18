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
class ArgvDestructor
{
public:
	constexpr ArgvDestructor() noexcept = default;

	void operator()(LPSTR* argv)
	{
		FreeArgvA(argv);
	}
};
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UNUSED(nShowCmd);
	UNUSED(hPrevInstance);
	int argc = 0;

	auto argv = std::unique_ptr<LPSTR, ArgvDestructor>(CommandLineToArgvA(lpCmdLine, &argc));
	if (argv == nullptr)
		return EXIT_FAILURE;
	int retVal = MainCode(hInstance, argc, argv.get());
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

int MainCode(void* hInstance, int argc, char** argv)
{
	using namespace greaper;
	gCoreLib = Construct<Library>(CORE_LIB_NAME);
	TRYEXP(gCoreLib->IsOpen(), "Couldn't open " CORE_LIBRARY_NAME);

	auto libFN = gCoreLib->GetFunctionT<void*>(LibFnName);
	TRYEXP(libFN, CORE_LIBRARY_NAME " does not have the _Greaper function");

	// init Greaper
	gCore = static_cast<IGreaperLibrary*>(libFN());

	auto propAppInstance = CreateProperty<ptruint>(gCore, IApplication::AppInstanceName, (ptruint)hInstance, ""sv, true, true, nullptr);

	StringVec commandLine;
	commandLine.resize(argc);
	for (int32 i = 0; i < argc; ++i)
		commandLine[i] = String{ argv[i] };
	auto propRes = CreateProperty<StringVec>(gCore, IApplication::CommandLineName, std::move(commandLine), ""sv, true, true, nullptr);

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
