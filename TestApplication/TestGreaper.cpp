
#include <iostream>
#include "../GreaperCore/Public/Uuid.hpp"

#if 0
#if PLT_WINDOWS
#define CORE_LIBRARY_NAME "Core" PLT_NAME ARCH_NAME GREAPER_LIBSUFFIX GREAPER_DLLEXT
#define GAL_LIBRARY_NAME "GAL" PLT_NAME ARCH_NAME GREAPER_LIBSUFFIX GREAPER_DLLEXT
#elif PLT_LINUX
#define CORE_LIBRARY_NAME "./Core" PLT_NAME ARCH_NAME GREAPER_LIBSUFFIX GREAPER_DLLEXT
#define GAL_LIBRARY_NAME "./GAL" PLT_NAME ARCH_NAME GREAPER_LIBSUFFIX GREAPER_DLLEXT
#endif
constexpr greaper::StringView CORE_LIB_NAME = { CORE_LIBRARY_NAME };
constexpr greaper::StringView GAL_LIB_NAME = { GAL_LIBRARY_NAME };
constexpr greaper::StringView LibFnName = "_Greaper"sv;
constexpr static bool AsyncLog = true;
greaper::PLibrary gCoreLib, gGALLib;
greaper::PGreaperLib gCore, gGAL;
greaper::PApplication gApplication;
greaper::PLogManager gLogManager;
greaper::PThreadManager gThreadManager;
greaper::PCommandManager gCommandManager;
//greaper::gal::PWindowManager gWindowManager;

#define APPLICATION_VERSION VERSION_SETTER(1, 0, 0, 0)

#define TRYEXP(exp, msg) { if(!exp) { DEBUG_OUTPUT(msg); TRIGGER_BREAKPOINT(); exit(EXIT_FAILURE); } }


static void SetProperties(void* hInstance, int32 argc, achar** argv)
{
	using namespace greaper;

	auto propAppInstanceRes = CreateProperty<ptruint>((WGreaperLib)gCore, IApplication::AppInstanceName, (ptruint)hInstance, ""sv, true, true, {});
	if (propAppInstanceRes.HasFailed())
		gCore->LogError(propAppInstanceRes.GetFailMessage());

	StringVec commandLine;
	commandLine.resize(argc);
	for (int32 i = 0; i < argc; ++i)
		commandLine[i].assign(argv[i]);
	auto propCmdLineRes = CreateProperty<StringVec>((WGreaperLib)gCore, IApplication::CommandLineName, std::move(commandLine), ""sv, true, true, {});
	if (propCmdLineRes.HasFailed())
		gCore->LogError(propCmdLineRes.GetFailMessage());

	auto propAppNameRes = CreateProperty<String>((WGreaperLib)gCore, IApplication::ApplicationNameName, String{ "TestApplication"sv }, ""sv, true, true, {});
	if (propAppNameRes.HasFailed())
		gCore->LogError(propAppNameRes.GetFailMessage());

	auto propAppVersionRes = CreateProperty<uint32>((WGreaperLib)gCore, IApplication::ApplicationVersionName, APPLICATION_VERSION, ""sv, true, true, {});
	if (propAppVersionRes.HasFailed())
		gCore->LogError(propAppVersionRes.GetFailMessage());

	/*auto propLogAsyncRes = CreateProperty<bool>((WGreaperLib)gCore, ILogManager::AsyncLogName, AsyncLog, ""sv, false, true, nullptr);
	if (propLogAsyncRes.HasFailed())
		gCore->LogError(propLogAsyncRes.GetFailMessage());*/
}

static void ActivateManagers()
{
	using namespace greaper;
	auto app = gCore->GetApplication();
	TRYEXP(!app.expired(), CORE_LIBRARY_NAME " does not have an Application.");
	gApplication = app.lock();

	auto mgrRes = gApplication->GetInterface(ILogManager::InterfaceUUID, gCore->GetLibraryUuid());
	if (mgrRes.IsOk())
	{
		gLogManager = (PLogManager)mgrRes.GetValue();
		gLogManager->AddLogWriter(SPtr<ILogWriter>(ConstructShared<LogWriterFile>()));
		gLogManager->AddLogWriter(SPtr<ILogWriter>(ConstructShared<LogWriterStdout>()));
	}

	mgrRes = gApplication->GetInterface(IThreadManager::InterfaceUUID, gCore->GetLibraryUuid());
	if (mgrRes.IsOk())
	{
		gThreadManager = (PThreadManager)mgrRes.GetValue();
	}
	mgrRes = gApplication->GetInterface(ICommandManager::InterfaceUUID, gCore->GetLibraryUuid());
	if (mgrRes.IsOk())
	{
		gCommandManager = (PCommandManager)mgrRes.GetValue();
	}

	gApplication->ActivateInterface((const PInterface&)gThreadManager);
	gApplication->ActivateInterface((const PInterface&)gLogManager);
	gApplication->ActivateInterface((const PInterface&)gCommandManager);
}

static void GreaperCoreLibInit(void* hInstance, int32 argc, achar** argv)
{
	using namespace greaper;

	gCoreLib = ConstructShared<Library>(CORE_LIB_NAME);
	TRYEXP(gCoreLib->IsOpen(), "Couldn't open " CORE_LIBRARY_NAME);

	auto libFNRes = gCoreLib->GetFunctionT<void*>(LibFnName);
	TRYEXP(libFNRes.IsOk(), CORE_LIBRARY_NAME " does not have the _Greaper function.");

	// init GreaperCore
	auto* corePtr = static_cast<PGreaperLib*>(libFNRes.GetValue()());
	TRYEXP(corePtr, CORE_LIBRARY_NAME " does not return a IGreaperLibrary.");
	gCore = *corePtr;

	SetProperties(hInstance, argc, argv);

	gCore->InitLibrary(gCoreLib, PApplication());

	ActivateManagers();

	auto appNameProp = gApplication->GetApplicationName().lock();
	auto appVersionProp = gApplication->GetApplicationVersion().lock();
	const auto [major, minor, patch, rev] = GetGreaperVersionValues(appVersionProp->GetValueCopy());
	String appName = appNameProp->GetValueCopy();
	gCore->Log(Format("Successfully started %s! Version:%d.%d.%d.%d", appName.c_str(), major, minor,
		patch, rev));
}

static void GreaperCoreLibClose()
{
	using namespace greaper;

	String appName = gApplication->GetApplicationName().lock()->GetValueCopy();
	gCore->Log(Format("Closing %s...", appName.c_str()));

	gCommandManager.reset();
	gThreadManager.reset();
	gLogManager.reset();
	gApplication.reset();
	gCore->DeinitLibrary();
	gCore.reset();
}


static void GreaperGALLibInit()
{
	using namespace greaper;

	gGALLib = ConstructShared<Library>(GAL_LIB_NAME);
	TRYEXP(gGALLib->IsOpen(), "Couldn't open " GAL_LIBRARY_NAME);

	auto galRes = gApplication->RegisterGreaperLibrary(gGALLib);
	TRYEXP(galRes.IsOk(), "Something went wrong registering " GAL_LIBRARY_NAME);
	gGAL = galRes.GetValue();

	//auto wndMgrRes = gApplication->GetInterface(gal::IWindowManager::InterfaceUUID, gGAL->GetLibraryUuid());
	//TRYEXP(wndMgrRes.IsOk(), "Something went wrong obtaining interface WindowManager.");
	//gWindowManager = wndMgrRes.GetValue();
	//auto activateRes = gApplication->ActivateInterface((const PInterface&)gWindowManager);
	//TRYEXP(activateRes.IsOk(), "Something went wrong activating interface WindowManager.");
}

static void GreaperGALLibClose()
{
	using namespace greaper;

	auto res = gApplication->UnregisterGreaperLibrary(gGAL);
	if (res.HasFailed())
		gLogManager->Log(LogLevel_t::ERROR, res.GetFailMessage(), GAL_LIB_NAME);
	//gWindowManager.reset();
	gGAL.reset();
	gGALLib.reset();
}

static void WindowedRunFunction()
{
	using namespace greaper;

	// Create a test window
//#if PLT_WINDOWS
//	gal::WinWindowDesc windowDesc{};
//	using WindowType = gal::WinWindow;
//#elif PLT_LINUX
//	gal::LnxWindowDesc windowDesc{};
//	using WindowType = gal::LnxWindow;
//#endif
//
//	windowDesc.Title = "Greaper Test Window"sv;
//	windowDesc.Size = math::Vector2i(800, 600);
//	windowDesc.State = WindowState_t::Normal;
//	
//	gal::PWindow window;
//	{
//		auto windowRes = gWindowManager->CreateWindow(windowDesc);
//
//		if (windowRes.HasFailed())
//		{
//			gLogManager->Log(LogLevel_t::ERROR, windowRes.GetFailMessage(), "TestApplication"sv);
//			return;
//		}
//		window = windowRes.GetValue();
//	}
//
//
//	// Keep running until the test window is closed
//	auto prevTime = Clock_t::now();
//	achar buffer[64];
//	uint64 frameCount = 0;
//	static constexpr sizet accumFrames = 60;
//	static constexpr double invTimeCount = 1.0 / accumFrames;
//	double accumTime = 0.0;
//	//double accumTimes[60];
//	//ClearMemory(accumTimes);
//	//auto hWnd = ((SPtr<gal::WinWindow>)window)->GetOSHandle();
//	bool closeWindow = false;
//	while (!closeWindow)
//	{
//		/*auto ret = */window->GetTaskScheduler()->AddTask([&window, &closeWindow]() { 
//			window->PollEvents();
//			window->SwapWindow();
//			auto res = window->ShouldClose();
//			if (res.IsOk())
//				closeWindow = res.GetValue();
//		});
//		//if (ret.IsOk())
//		//{
//		//	window->GetTaskScheduler()->WaitUntilTaskFinished(ret.GetValue());
//			//auto& future = ret.GetValue();
//			//if (future.valid())
//			//	future.wait();
//		//}
//		
//		window->GetTaskScheduler()->WaitUntilAllTasksFinished();
//
//		auto curTime = Clock_t::now();
//		auto diff = curTime - prevTime;
//		
//		accumTime += diff.count();
//
//		if ((frameCount % accumFrames) == 0)
//		{
//			double avgTime = accumTime * 1e-6 * invTimeCount;
//			double avgUpdate = 1.0 / (avgTime * 1e-3);
//			int sz = snprintf(buffer, ArraySize(buffer), "Greaper Test Window %fms %.2f", avgTime, avgUpdate);
//			window->ChangeWindowTitle(StringView(buffer, sz+1));
//			accumTime = 0.0;
//			//if(frameCount % 10000 == 0)
//			//	printf_s("Times: %fms %.2f\n", avgTime, avgUpdate);
//		}
//
//		prevTime = curTime;
//		++frameCount;
//	}
//
//	//while(!shouldClose)
//	//{
//	//	gWindowManager->PollEvents();
//	//	gWindowManager->AccessWindows([&shouldClose](CSpan<gal::PWindow> windows) { shouldClose = windows.GetSizeFn() > 0; });
//	//}
}
#endif 
void GreaperTest()
{
#if 0
	OSPlatform::PerThreadInit();
	
	try
	{
		GreaperCoreLibInit(hInstance, argc, argv);
		//GreaperGALLibInit();

		if (RunTests)
		{
			TestFunction();
			std::cout << "Test finished" << std::endl;
			char c;
			std::cin >> c;
		}

		if(RunWindow)
		{
			WindowedRunFunction();
		}

		//GreaperGALLibClose();
		GreaperCoreLibClose();

		gCoreLib->Close();
		gCoreLib.reset();
	}
	catch (const std::exception& e)
	{
		DEBUG_OUTPUT("-------------");
		DEBUG_OUTPUT(e.what());
		DEBUG_OUTPUT("-------------");
		TRIGGER_BREAKPOINT();
	}

	return EXIT_SUCCESS;
#endif

	using namespace greaper;

	auto uuid = Uuid::GenerateRandom();

	using refl_uuid = refl::TypeInfo<decltype(uuid)>::Type;

	auto json_ret = refl_uuid::CreateJSON(uuid, "TestUuid"sv);

	if (!json_ret.has_value())
	{
		std::cout << "ERROR: " << json_ret.error() << std::endl;
		return;
	}
	auto json = json_ret.value();
	std::cout << cJSON_Print(json.get()) << std::endl;

	Uuid test{};
	auto json_ret2 = refl_uuid::FromJSON(test, json.get(), "TestUuid"sv);
	if (!json_ret2.has_value())
	{
		std::cout << "ERROR: " << json_ret2.error() << std::endl;
		return;
	}
	if (uuid != test)
	{
		std::cout << std::format("Uuid converted to JSON and back, expected '{}', obtained '{}'", uuid.ToString(),
			test.ToString()) << std::endl;
		return;
	}

	
	
	



	std::cout << "Test under construction" << std::endl;
}