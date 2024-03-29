/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "../GreaperCore/Public/CorePrerequisites.h"
#include "../GreaperCore/Public/Library.h"
#include "../GreaperCore/Public/IGreaperLibrary.h"
#include "../GreaperCore/Public/IApplication.h"
#include "../GreaperCore/Public/Reflection/ContainerType.h"
#include "../GreaperCore/Public/Property.h"
#include "../GreaperCore/Public/Base/LogWriterFile.h"
#include "../GreaperCore/Public/Base/LogWriterStdout.h"
#include "../GreaperCore/Public/IThreadManager.h"
#include "../GreaperCore/Public/ICommandManager.h"
#include "../GreaperCore/Public/Platform.h"
#include "../GreaperMath/Public/Vector4.h"
#include "../GreaperMath/Public/Matrix4.h"
#include "../GreaperMath/Public/Quaternion.h"
#include "../GreaperMath/Public/Reflection/Vector3.h"
#include "../GreaperMath/Public/Reflection/Quaternion.h"
#include "../GreaperCore/Public/MemoryStream.h"
#include "../GreaperCore/Public/Reflection/Property.h"
//#include "../GreaperGAL/Public/IWindowManager.h"
//#if PLT_WINDOWS
//#include "../GreaperGAL/Public/Win/WinWindow.h"
//#elif PLT_LINUX
//#include "../GreaperGAL/Public/Lnx/LnxWindow.h"
//#endif
#include "../GreaperCore/Public/SlimTaskScheduler.h"
#include <random>
#include <iostream>

#if PLT_WINDOWS
#define PLT_NAME "Win"
#else
#define PLT_NAME "Lnx"
#endif
// DROP SUPPORT FOR 32bit
#if ARCHITECTURE_X64
#define ARCH_NAME /*"64"*/
#else
#define ARCH_NAME /*"32"*/
#endif

#if RelWithDebInfo

#endif

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

template<class TResult>
static void ReportTest(greaper::StringView testName, sizet sampleCount, const greaper::Vector<TResult>& resultNormal, const greaper::Vector<TResult>& resultOptim,
	greaper::Clock_t::duration durationNormalNs, greaper::Clock_t::duration durationOptimNs)
{
	using namespace greaper;
	using namespace math;

	const achar* outputTxt = nullptr;
	if constexpr (std::is_same_v<TResult, int32>)
		outputTxt = "%s: Sample %" PRIuPTR " was not verified normal:%" PRId32 " optim:%" PRId32 ".\n";
	else if constexpr (std::is_same_v<TResult, int64>)
		outputTxt = "%s: Sample %" PRIuPTR " was not verified normal:%" PRId64 " optim:%" PRId64 ".\n";
	else if constexpr (std::is_same_v<TResult, float>)
		outputTxt = "%s: Sample %" PRIuPTR " was not verified normal:%f optim:%f.\n";
	else if constexpr (std::is_same_v<TResult, double>)
		outputTxt = "%s: Sample %" PRIuPTR " was not verified normal:%lf optim:%lf.\n";

	if constexpr (std::is_integral_v<TResult>)
	{
		for (sizet i = 0; i < sampleCount; ++i)
		{
			auto normal = resultNormal[i];
			auto optim = resultOptim[i];
			if (normal != optim)
			{
				std::cout << Format(outputTxt, testName.data(), i, normal, optim);
			}
		}
	}
	else if constexpr (std::is_floating_point_v<TResult>)
	{
		for (sizet i = 0; i < sampleCount; ++i)
		{
			auto normal = resultNormal[i];
			auto optim = resultOptim[i];
			if (!IsNearlyEqual(normal, optim))
			{
				std::cout << Format(outputTxt, testName.data(), i, normal, optim);
			}
		}
	}

	auto normAvg = float((double)durationNormalNs.count() / (double)sampleCount);
	auto optiAvg = float((double)durationOptimNs.count() / (double)sampleCount);

	if (durationNormalNs.count() < durationOptimNs.count())
	{
		auto times = float((double)durationOptimNs.count() / (double)durationNormalNs.count());
		std::cout << Format("%s:\tNormal was %.3f times faster, NORM:%.3fns OPTM:%.3fns.\n", testName.data(), times, normAvg, optiAvg);
	}
	else if (durationNormalNs.count() > durationOptimNs.count())
	{
		auto times = float((double)durationNormalNs.count() / (double)durationOptimNs.count());
		std::cout << Format("%s:\tOptim was %.3f times faster, NORM:%.3fns OPTM:%.3fns.\n", testName.data(), times, normAvg, optiAvg);
	}
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> TruncIntFTest(sizet sampleCount,
	const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal,
	greaper::Vector<int32>& resultOptim)
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

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> FloorIntFTest(sizet sampleCount,
	const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal,
	greaper::Vector<int32>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = static_cast<int32>(std::floor(samples[i]));
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

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> RoundIntFTest(sizet sampleCount,
	const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal,
	greaper::Vector<int32>& resultOptim)
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
		resultOptim[i] = static_cast<int32>(std::floor(samples[i] + 0.5f));
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> CeilIntFTest(sizet sampleCount,
	const greaper::Vector<float>& samples, greaper::Vector<int32>& resultNormal,
	greaper::Vector<int32>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = static_cast<int32>(std::ceil(samples[i]));
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

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> Log2DTest(sizet sampleCount,
	const greaper::Vector<double>& samples, greaper::Vector<double>& resultNormal,
	greaper::Vector<double>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	static constexpr double ONEOVERLOG2 = 1.4426950408889634;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = std::log2(samples[i]);
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		resultOptim[i] = std::log(samples[i]) * ONEOVERLOG2;
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> InvSqrtFTest(sizet sampleCount,
	const greaper::Vector<float>& samples, greaper::Vector<float>& resultNormal,
	greaper::Vector<float>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = InvSqrt(samples[i]);
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		resultOptim[i] = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(samples[i])));
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> LenghtV4FTest(sizet sampleCount,
	const greaper::Vector<greaper::math::Vector4f>& samplesV4, const greaper::VectorAligned<__m128>& samplesSSE,
	greaper::Vector<float>& resultNormal, greaper::Vector<float>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
		resultNormal[i] = samplesV4[i].LengthSquared();
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		resultOptim[i] = SSE::LengthSquared(samplesSSE[i]);
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> NormV4FTest(sizet sampleCount,
	const greaper::Vector<greaper::math::Vector4f>& samplesV4, const greaper::VectorAligned<__m128>& samplesSSE,
	greaper::Vector<float>& resultNormal, greaper::Vector<float>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		auto norm = samplesV4[i].GetNormalized();
		resultNormal[i] = norm.Length();
	}
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		auto norm = SSE::Normalize(samplesSSE[i]);
		resultOptim[i] = SSE::Length(norm);
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static std::tuple<greaper::Clock_t::duration, greaper::Clock_t::duration> DistV4FTest(sizet sampleCount,
	const greaper::Vector<greaper::math::Vector4f>& samplesV4, const greaper::VectorAligned<__m128>& samplesSSE,
	greaper::Vector<float>& resultNormal, greaper::Vector<float>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; )
	{
		const auto& x = samplesV4[i++];
		const auto& y = samplesV4[i++];
		auto dist = x.Distance(y);
		resultNormal[i - 2] = dist;
		resultNormal[i - 1] = dist;
	}
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; )
	{
		auto dist = SSE::Distance(samplesSSE[i++], samplesSSE[i++]);
		resultOptim[i - 2] = dist;
		resultOptim[i - 1] = dist;
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

	Vector<float> samplesPF, samplesPF64;
	samplesPF.resize(sampleCount, 0.f);
	samplesPF64.resize(sampleCount, 0.f);
	Vector<double> samplesPD, samplesPD64;
	samplesPD.resize(sampleCount, 0.0);
	samplesPD64.resize(sampleCount, 0.0);

	Vector<Vector4f> samplesV4;
	samplesV4.resize(sampleCount, Vector4f{});
	VectorAligned<__m128> samplesSSE;
	samplesSSE.resize(sampleCount, __m128{});

	Vector<int32> resultNormal, resultOptim;
	resultNormal.resize(sampleCount, 0);
	resultOptim.resize(sampleCount, 0);
	Vector<int64> resultNormal64, resultOptim64;
	resultNormal64.resize(sampleCount, 0);
	resultOptim64.resize(sampleCount, 0);
	Vector<float> resultNormalF, resultOptimF;
	resultNormalF.resize(sampleCount, 0);
	resultOptimF.resize(sampleCount, 0);
	Vector<double> resultNormalD, resultOptimD;
	resultNormalD.resize(sampleCount, 0);
	resultOptimD.resize(sampleCount, 0);

	std::random_device generator{};

	constexpr auto int32Min = std::numeric_limits<int32>::min();
	constexpr auto int32Max = std::numeric_limits<int32>::max();
	constexpr auto int64Min = std::numeric_limits<int64>::min();
	constexpr auto int64Max = std::numeric_limits<int64>::max();

	std::uniform_real_distribution<float> distributionF((float)int32Min, (float)int32Max);
	std::uniform_real_distribution<double> distributionD((double)int32Min, (double)int32Max);
	std::uniform_real_distribution<float> distributionF64((float)int64Min, (float)int64Max);
	std::uniform_real_distribution<double> distributionD64((double)int64Min, (double)int64Max);

	std::uniform_real_distribution<float> distributionPF(0.f, (float)int32Max);
	std::uniform_real_distribution<double> distributionPD(0.0, (double)int32Max);
	std::uniform_real_distribution<float> distributionPF64(0.f, (float)int64Max);
	std::uniform_real_distribution<double> distributionPD64(0.0, (double)int64Max);

	std::uniform_real_distribution<float> distributionV4(-10.f, 10.f);

	for (sizet i = 0; i < sampleCount; ++i)
	{
		samplesF[i] = distributionF(generator);
		samplesD[i] = distributionD(generator);
		samplesF64[i] = distributionF64(generator);
		samplesD64[i] = distributionD64(generator);

		samplesPF[i] = distributionPF(generator);
		samplesPD[i] = distributionPD(generator);
		samplesPF64[i] = distributionPF64(generator);
		samplesPD64[i] = distributionPD64(generator);

		auto f0 = distributionV4(generator);
		auto f1 = distributionV4(generator);
		auto f2 = distributionV4(generator);
		auto f3 = distributionV4(generator);

		samplesV4[i].Set(f0, f1, f2, f3);
		samplesSSE[i] = _mm_set_ps(f0, f1, f2, f3);
	}

	Clock_t::duration durNorm, durOpt;
	std::tie(durNorm, durOpt) = TruncIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("TruncIntF"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);
	
	std::tie(durNorm, durOpt) = FloorIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("FloorIntFTest"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);

	std::tie(durNorm, durOpt) = RoundIntFTest(sampleCount, samplesF, resultNormal, resultOptim);
	ReportTest("RoundIntFTest"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);

	std::tie(durNorm, durOpt) = CeilIntFTest(sampleCount, samplesPF, resultNormal, resultOptim);
	ReportTest("CeilIntFTest"sv, sampleCount, resultNormal, resultOptim, durNorm, durOpt);

	std::tie(durNorm, durOpt) = Log2DTest(sampleCount, samplesPD, resultNormalD, resultOptimD);
	ReportTest("Log2DTest"sv, sampleCount, resultNormalD, resultOptimD, durNorm, durOpt);

	std::tie(durNorm, durOpt) = InvSqrtFTest(sampleCount, samplesPF, resultNormalF, resultOptimF);
	ReportTest("InvSqrtFTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = LenghtV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("LenghtV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = NormV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("NormV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = DistV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("DistV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	using prec = double;
	auto odeg = Vector3Real<prec>(90, -60, 15);
	auto orad = odeg * DEG2RAD<prec>;

	auto q0 = QuaternionReal<prec>::FromEuler(orad.X, 0, 0);
	auto q1 = QuaternionReal<prec>::FromEuler(0, orad.Y, 0);
	auto q2 = QuaternionReal<prec>::FromEuler(0, 0, orad.Z);
	auto qf0 = q0 * q1 * q2;
	auto qf1 = q0 * q2 * q1;
	auto qf2 = q1 * q0 * q2;
	auto qf3 = q1 * q2 * q0;
	auto qf4 = q2 * q0 * q1;
	auto qf5 = q2 * q1 * q0;

	auto erad0 = qf0.ToEulerAngles();
	auto erad1 = qf1.ToEulerAngles();
	auto erad2 = qf2.ToEulerAngles();
	auto erad3 = qf3.ToEulerAngles();
	auto erad4 = qf4.ToEulerAngles();
	auto erad5 = qf5.ToEulerAngles();

	auto edeg0 = erad0 * RAD2DEG<prec>;
	auto edeg1 = erad1 * RAD2DEG<prec>;
	auto edeg2 = erad2 * RAD2DEG<prec>;
	auto edeg3 = erad3 * RAD2DEG<prec>;
	auto edeg4 = erad4 * RAD2DEG<prec>;
	auto edeg5 = erad5 * RAD2DEG<prec>;

	auto quatArray = Vector<std::pair<QuaternionReal<prec>, Vector3Real<prec>>>{ {qf0,edeg0}, {qf1, edeg1}, {qf2, edeg2}, {qf3, edeg3}, {qf4, edeg4}, {qf5, edeg5} };
	decltype(quatArray) testArray, testArray2{};

	using typeInfo = typename refl::TypeInfo_t<decltype(quatArray)>::Type;

	auto json = typeInfo::CreateJSON(quatArray, "quatMap"sv);
	auto text = SPtr<char>(cJSON_Print(json.get()));
	std::cout << text.get() << std::endl;
	auto parsed = SPtr<cJSON>(cJSON_Parse(text.get()), cJSON_Delete);
	auto parseRes = typeInfo::FromJSON(testArray, parsed.get(), "quatMap"sv);

	if (parseRes.HasFailed())
		std::cout << parseRes.GetFailMessage() << std::endl;

	MemoryStream ms{ (uint64)typeInfo::StaticSize + (uint64)typeInfo::GetDynamicSize(quatArray) };
	
	auto res = typeInfo::ToStream(quatArray, ms);
	if (res.HasFailed())
		std::cout << res.GetFailMessage() << std::endl;

	ms.Seek(0);

	res = typeInfo::FromStream(testArray2, ms);
	if (res.HasFailed())
		std::cout << res.GetFailMessage() << std::endl;

	auto origIt = quatArray.begin();
	auto test1It = testArray.begin();
	auto test2It = testArray2.begin();

	for (decltype(quatArray)::size_type i = 0; i < quatArray.size(); ++i)
	{
		const auto& orig = *origIt;
		const auto& test1 = *test1It;
		const auto& test2 = *test2It;

		using origTypeInfo = refl::TypeInfo_t<decltype(orig)>::Type;
		using test1TypeInfo = refl::TypeInfo_t<decltype(test1)>::Type;
		using test2TypeInfo = refl::TypeInfo_t<decltype(test2)>::Type;

		if (orig != test1)
			std::cout << Format("Badly JSON stream quaternion, expected: '%s' obtained: '%s'.", origTypeInfo::ToString(orig).c_str(), test1TypeInfo::ToString(test1).c_str()) << std::endl;
		if (orig != test2)
			std::cout << Format("Badly Memory stream quaternion, expected: '%s' obtained: '%s'.", origTypeInfo::ToString(orig).c_str(), test2TypeInfo::ToString(test2).c_str()) << std::endl;

		++origIt; ++test1It; ++test2It;
	}

	auto props = gCore->GetProperties();
	sizet propIdx = 0;
	achar buffer[128];
	for(const auto& prop : props)
	{
		snprintf(buffer, ArraySize(buffer), "Elem_%" PRIuPTR, propIdx++);
		using propTypeInfo = refl::TypeInfo<IProperty>::Type;
		auto propJSON = propTypeInfo::CreateJSON(*prop, StringView{buffer});
		auto propText = SPtr<char>(cJSON_Print(propJSON.get()));
		std::cout << propText.get() << std::endl;
	}
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

extern void WaylandTest();

int MainCode(void* hInstance, int argc, char** argv)
{
	using namespace greaper;
	const bool RunTests = true;
	const bool RunWindow = false;
	const bool RunWaylandTest = PLT_LINUX;

	if (RunWaylandTest)
	{
		WaylandTest();
		return EXIT_SUCCESS;
	}


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
}
