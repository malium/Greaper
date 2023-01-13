/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "GreaperDisplayDLL.h"
#include "WindowManager.h"
#include <Core/Platform.h>
#include <Core/FileStream.h>
#include <Core/Reflection/Property.h>
#define GLFW_DLL
#include <External/GLFW/glfw3.h>

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

void* _Greaper()
{
	if (gDispLibrary == nullptr)
	{
		gDispLibrary.reset(greaper::Construct<greaper::disp::GreaperDispLibrary>());
		greaper::OSPlatform::PerLibraryInit();
	}
	return &gDispLibrary;
}

static void GLFWErrorReporter(int32 errorCode, const char* errorMessage)
{
	gDispLibrary->LogError(greaper::Format("GLFW error, code:%" PRIi32 " msg: %s.", errorCode, errorMessage));
}

void greaper::disp::GreaperDispLibrary::Initialize() noexcept
{
	glfwSetErrorCallback(&GLFWErrorReporter);
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	Log(Format("Initializing GLFW %" PRIi32 ".%" PRIi32 ".%" PRIi32 "...", major, minor, rev));
	auto rtn = glfwInit();
	if (rtn != GLFW_TRUE)
	{
		const char* errorMsg;
		auto errorCode = glfwGetError(&errorMsg);
		Break("Couldn't initialize GLFW, error code:%" PRIi32 ", error msg: %s.", errorCode, errorMsg);
	}
}

void greaper::disp::GreaperDispLibrary::AddManagers()noexcept
{
	// add more managers
	gWindowManager.reset(Construct<WindowManager>());
	m_Managers.push_back((PInterface)gWindowManager);

}

void greaper::disp::GreaperDispLibrary::RemoveManagers()noexcept
{
	IGreaperLibrary::RemoveManagers();
	gWindowManager.reset();
}

void greaper::disp::GreaperDispLibrary::Deinitialize()noexcept
{
	Log("Shutting down GLFW...");
	glfwTerminate();
	gDispLibrary.reset();
}

#else

#endif // GREAPER_DISP_DLL