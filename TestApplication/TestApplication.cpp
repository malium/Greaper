/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include <Core/CorePrerequisites.h>
#include <Core/IGreaperLibrary.h>
#include <Core/IApplication.h>
#include <Core/Reflection/ReflectedPlainContainer.h>
#include <Core/Property.h>
#include <Core/Base/LogWriterFile.h>
#include <Core/IThreadManager.h>
#include <Core/Platform.h>
#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <random>
#include <iostream>

#if PLT_WINDOWS
#define PLT_NAME "Windows"
#else
#define PLT_NAME "Linux"
#endif
#if ARCHITECTURE_X64
#define ARCH_NAME "64"
#else
#define ARCH_NAME "32"
#endif

#if PLT_WINDOWS
#define CORE_LIBRARY_NAME "Core" PLT_NAME ARCH_NAME GREAPER_LIBSUFFIX GREAPER_DLLEXT
#elif PLT_LINUX
#define CORE_LIBRARY_NAME "./Core" PLT_NAME ARCH_NAME GREAPER_LIBSUFFIX GREAPER_DLLEXT
#endif
constexpr greaper::StringView CORE_LIB_NAME = { CORE_LIBRARY_NAME };
constexpr greaper::StringView LibFnName = "_Greaper"sv;
constexpr static bool AsyncLog = true;
greaper::PLibrary gCoreLib;
greaper::PGreaperLib gCore;
greaper::PApplication gApplication;
greaper::PLogManager gLogManager;
greaper::PThreadManager gThreadManger;

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
INT WINAPI WinMain(HINSTANCE hInstance, UNUSED HINSTANCE hPrevInstance, LPSTR lpCmdLine, UNUSED int nShowCmd)
{
	int argc = 0;

	auto argv = greaper::UPtr<LPSTR>(CommandLineToArgvA(lpCmdLine, &argc), &FreeArgvA);
	if (argv == nullptr)
		return EXIT_FAILURE;

	return MainCode(hInstance, argc, argv.Get());
}
#endif 
#else /* !PLT_WINDOWS */
int main(int argc, char* argv[])
{
	void* hInstance = nullptr;
	return MainCode(hInstance, argc, argv);
}
#endif

static void SetProperties(void* hInstance, int32 argc, achar** argv)
{
	using namespace greaper;

	auto propAppInstanceRes = CreateProperty<ptruint>((WGreaperLib)gCore, IApplication::AppInstanceName, (ptruint)hInstance, ""sv, true, true, nullptr);
	if (propAppInstanceRes.HasFailed())
		gCore->LogError(propAppInstanceRes.GetFailMessage());

	StringVec commandLine;
	commandLine.resize(argc);
	for (int32 i = 0; i < argc; ++i)
		commandLine[i].assign(argv[i]);
	auto propCmdLineRes = CreateProperty<StringVec>((WGreaperLib)gCore, IApplication::CommandLineName, std::move(commandLine), ""sv, true, true, nullptr);
	if (propCmdLineRes.HasFailed())
		gCore->LogError(propCmdLineRes.GetFailMessage());

	auto propAppNameRes = CreateProperty<String>((WGreaperLib)gCore, IApplication::ApplicationNameName, String{"TestApplication"sv}, ""sv, true, true, nullptr);
	if (propAppNameRes.HasFailed())
		gCore->LogError(propAppNameRes.GetFailMessage());

	auto propAppVersionRes = CreateProperty<uint32>((WGreaperLib)gCore, IApplication::ApplicationVersionName, APPLICATION_VERSION, ""sv, true, true, nullptr);
	if (propAppVersionRes.HasFailed())
		gCore->LogError(propAppVersionRes.GetFailMessage());

	auto propLogAsyncRes = CreateProperty<bool>((WGreaperLib)gCore, ILogManager::AsyncLogName, AsyncLog, ""sv, false, true, nullptr);
	if (propLogAsyncRes.HasFailed())
		gCore->LogError(propLogAsyncRes.GetFailMessage());
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
		gLogManager->AddLogWriter(SPtr<ILogWriter>(Construct<LogWriterFile>()));
	}

	mgrRes = gApplication->GetInterface(IThreadManager::InterfaceUUID, gCore->GetLibraryUuid());
	if (mgrRes.IsOk())
	{
		gThreadManger = (PThreadManager)mgrRes.GetValue();
	}

	gApplication->ActivateInterface((const PInterface&)gThreadManger);
	gApplication->ActivateInterface((const PInterface&)gLogManager);
}

static void GreaperCoreLibInit(void* hInstance, int32 argc, achar** argv)
{
	using namespace greaper;

	auto libFNRes = gCoreLib->GetFunctionT<void*>(LibFnName);
	TRYEXP(libFNRes.IsOk(), CORE_LIBRARY_NAME " does not have the _Greaper function.");

	// init Greaper
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

	gThreadManger.reset();
	gLogManager.reset();
	gApplication.reset();
	gCore->DeinitLibrary();
	gCore.reset();
}

template<class TResult>
static void ReportTest(greaper::StringView testName, sizet sampleCount, const greaper::Vector<TResult>& resultNormal, const greaper::Vector<TResult>& resultOptim,
	greaper::Clock_t::duration durationNormalNs, greaper::Clock_t::duration durationOptimNs)
{
	using namespace greaper;
	using namespace math;

	const achar* outputTxt = nullptr;
	if constexpr (std::is_same_v<TResult, int32>)
		outputTxt = "%s: Sample %lld was not verified normal:%d optim:%d.\n";
	else if (std::is_same_v<TResult, int64>)
		outputTxt = "%s: Sample %lld was not verified normal:%lld optim:%lld.\n";

	for (sizet i = 0; i < sampleCount; ++i)
	{
		auto normal = resultNormal[i];
		auto optim = resultOptim[i];
		if (normal != optim)
		{
			DEBUG_OUTPUT(Format(outputTxt, testName.data(), i, normal, optim).c_str());
		}
	}

	if (durationNormalNs.count() < durationOptimNs.count())
	{
		DEBUG_OUTPUT(Format("%s:\tNormal was %f times faster, NORM:%fns OPTM:%fns.\n", testName.data(), (float)durationOptimNs.count() / (float)durationNormalNs.count(), (float)durationNormalNs.count() / (float)sampleCount, (float)durationOptimNs.count() / (float)sampleCount).c_str());
	}
	else if (durationNormalNs.count() > durationOptimNs.count())
	{
		DEBUG_OUTPUT(Format("%s:\tOptim was %f times faster, NORM:%fns OPTM:%fns.\n", testName.data(), (float)durationNormalNs.count() / (float)durationOptimNs.count(), (float)durationNormalNs.count() / (float)sampleCount, (float)durationOptimNs.count() / (float)sampleCount).c_str());
	}
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> TruncIntFTest(sizet sampleCount, const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal, greaper::Vector<int32>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = static_cast<int32>(std::truncf(samples[i]));
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultOptim[i] = _mm_cvtt_ss2si(_mm_set_ss(samples[i]));
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> FloorIntFTest(sizet sampleCount, const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal, greaper::Vector<int32>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = static_cast<int32>(std::floorf(samples[i]));
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		resultOptim[i] = _mm_cvt_ss2si(_mm_round_ss(_mm_setzero_ps(), _mm_set_ss(samples[i]), (_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)));
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> RoundIntFTest(sizet sampleCount, const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal, greaper::Vector<int32>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = static_cast<int32>(std::lroundf(samples[i]));
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		resultOptim[i] = static_cast<int32>(std::floorf(samples[i] + 0.5f));
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> CeilIntFTest(sizet sampleCount, const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal, greaper::Vector<int32>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = static_cast<int32>(std::ceilf(samples[i]));
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		//resultOptim[i] = -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (samples[i] + samples[i]))) >> 1);
		resultOptim[i] = _mm_cvt_ss2si(_mm_round_ss(_mm_setzero_ps(), _mm_set_ss(samples[i]), (_MM_FROUND_CEIL | _MM_FROUND_NO_EXC)));
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static void TestFunction()
{
	using namespace greaper;
	using namespace math;

	constexpr sizet sampleCount = 1'000'000;

	Vector<float> samplesF, samplesF64;
	samplesF.resize(sampleCount, 0.f);
	samplesF64.resize(sampleCount, 0.f);
	Vector<double> samplesD, samplesD64;
	samplesD.resize(sampleCount, 0.0);
	samplesD64.resize(sampleCount, 0.0);
	Vector<int32> resultNormal, resultOptim;
	resultNormal.resize(sampleCount, 0);
	resultOptim.resize(sampleCount, 0);
	Vector<int64> resultNormal64, resultOptim64;
	resultNormal64.resize(sampleCount, 0);
	resultOptim64.resize(sampleCount, 0);

	std::mt19937_64 generator{ (unsigned long)Clock_t::now().time_since_epoch().count()};

	std::uniform_real_distribution<float> distributionF((float)INT_MIN, (float)INT_MAX);
	std::uniform_real_distribution<double> distributionD((double)INT_MIN, (double)INT_MAX);
	std::uniform_real_distribution<float> distributionF64((float)INT64_MIN, (float)INT64_MAX);
	std::uniform_real_distribution<double> distributionD64((double)INT64_MIN, (double)INT64_MAX);

	for (sizet i = 0; i < sampleCount; ++i)
	{
		samplesF[i] = distributionF(generator);
		samplesD[i] = distributionD(generator);
		samplesF64[i] = distributionF64(generator);
		samplesD64[i] = distributionD64(generator);
	}

	Clock_t::duration durNorm, durOpt;
	std::tie(durNorm, durOpt) = TruncIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("TruncIntF"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);
	
	std::tie(durNorm, durOpt) = FloorIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("FloorIntFTest"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);

	std::tie(durNorm, durOpt) = RoundIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("RoundIntFTest"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);

	std::tie(durNorm, durOpt) = CeilIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("CeilIntFTest"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);
}

int MainCode(void* hInstance, int argc, char** argv)
{
	using namespace greaper;

	OSPlatform::PerThreadInit();

	try
	{
		gCoreLib.reset(Construct<Library>(CORE_LIB_NAME));
		TRYEXP(gCoreLib->IsOpen(), "Couldn't open " CORE_LIBRARY_NAME);

		GreaperCoreLibInit(hInstance, argc, argv);

		TestFunction();

		GreaperCoreLibClose();

		gCoreLib->Close();
		gCoreLib.reset();
	}
	catch (const std::exception& e)
	{
		TRIGGER_BREAKPOINT();
		DEBUG_OUTPUT(e.what());
	}

	return EXIT_SUCCESS;
}
