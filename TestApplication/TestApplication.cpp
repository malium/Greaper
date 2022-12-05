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
	else if constexpr (std::is_same_v<TResult, int64>)
		outputTxt = "%s: Sample %lld was not verified normal:%lld optim:%lld.\n";
	else if constexpr (std::is_same_v<TResult, float>)
		outputTxt = "%s: Sample %lld was not verified normal:%f optim:%f.\n";
	else if constexpr (std::is_same_v<TResult, double>)
		outputTxt = "%s: Sample %lld was not verified normal:%lf optim:%lf.\n";

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
		resultOptim[i] = static_cast<int32>(std::floorf(samples[i] + 0.5f));
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
	const greaper::Vector<greaper::math::Vector4f>& samplesV4, const greaper::VectorAligned<__m128, 16>& samplesSSE,
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
	const greaper::Vector<greaper::math::Vector4f>& samplesV4, const greaper::VectorAligned<__m128, 16>& samplesSSE,
	greaper::Vector<float>& resultNormal, greaper::Vector<float>& resultOptim)
{
	using namespace greaper;
	using namespace math;

	Timepoint_t beginNormal = Clock_t::now();
	for (sizet i = 0; i < sampleCount; ++i)
	{
		auto norm = samplesV4[i].Normalized();
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
	const greaper::Vector<greaper::math::Vector4f>& samplesV4, const greaper::VectorAligned<__m128, 16>& samplesSSE,
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
		resultNormal[i - 1] = dist;
		resultNormal[i] = dist;
	}
	Timepoint_t endNormal = Clock_t::now();

	auto durationNormalNs = endNormal - beginNormal;

	Timepoint_t beginOptim = Clock_t::now();
	for (sizet i = 0; i < sampleCount; )
	{
		auto dist = SSE::Distance(samplesSSE[i++], samplesSSE[i++]);
		resultOptim[i - 1] = dist;
		resultOptim[i] = dist;
	}
	Timepoint_t endOptim = Clock_t::now();

	auto durationOptimNs = endOptim - beginOptim;

	return { durationNormalNs, durationOptimNs };
}

static void TestFunction()
{
	using namespace greaper;
	using namespace math;

	constexpr sizet sampleCount = 10'000'000;

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
	VectorAligned<__m128, 16> samplesSSE;
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

	//std::mt19937_64 generator{ (unsigned long)Clock_t::now().time_since_epoch().count()};
	std::random_device generator{};

	std::uniform_real_distribution<float> distributionF((float)INT_MIN, (float)INT_MAX);
	std::uniform_real_distribution<double> distributionD((double)INT_MIN, (double)INT_MAX);
	std::uniform_real_distribution<float> distributionF64((float)INT64_MIN, (float)INT64_MAX);
	std::uniform_real_distribution<double> distributionD64((double)INT64_MIN, (double)INT64_MAX);

	std::uniform_real_distribution<float> distributionPF(0.f, (float)INT_MAX);
	std::uniform_real_distribution<double> distributionPD(0.0, (double)INT_MAX);
	std::uniform_real_distribution<float> distributionPF64(0.f, (float)INT64_MAX);
	std::uniform_real_distribution<double> distributionPD64(0.0, (double)INT64_MAX);

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


	auto a = _mm_set_ps(1.1f, 1.2f, 1.3f, 1.4f);
	auto b = _mm_set_ps(1.1001f, 1.20f, 1.30f, 1.40f);
	auto equal = SSE::Equal(a, b);
	auto nearly = SSE::NearlyEqual(a, b);

	std::tie(durNorm, durOpt) = LenghtV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("LenghtV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = NormV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("NormV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = DistV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("DistV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);
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
		std::cout << "Test finished" << std::endl;
		char c;
		std::cin >> c;

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
