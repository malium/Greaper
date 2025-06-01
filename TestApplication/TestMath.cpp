/***********************************************************************************************************************
 *                                   Copyright 2025 Marcos Sánchez Torrent (@malium)                                   *
 *                                               All Rights Reserved                                                   *
 **********************************************************************************************************************/

#include "../GreaperMath/Public/Quaternion.hpp"
#include "../GreaperMath/Public/Vector4.hpp"
#include "../GreaperCore/Public/MemoryStream.hpp"
#include <iostream>
#include <format>
#include <random>

#define USE_SSE 0

template<class TResult>
static void ReportTest(greaper::StringView testName, sizet sampleCount, const greaper::Vector<TResult>& resultNormal,
	const greaper::Vector<TResult>& resultOptim, greaper::Clock_t::duration durationNormalNs,
	greaper::Clock_t::duration durationOptimNs)
{
	using namespace greaper;
	using namespace math;

	if constexpr (std::is_integral_v<TResult>)
	{
		for (sizet i = 0; i < sampleCount; ++i)
		{
			auto normal = resultNormal[i];
			auto optim = resultOptim[i];
			if (normal != optim)
			{
				std::cout << std::format("{}: Sample {} was not verified normal:{} optim:{}.\n",
					testName, i, normal, optim);
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
				std::cout << std::format("{}: Sample {} was not verified normal:{} optim:{}.\n",
					testName, i, normal, optim);
			}
		}
	}

	auto normAvg = float((double)durationNormalNs.count() / (double)sampleCount);
	auto optiAvg = float((double)durationOptimNs.count() / (double)sampleCount);

	if (durationNormalNs.count() < durationOptimNs.count())
	{
		auto times = float((double)durationOptimNs.count() / (double)durationNormalNs.count());
		std::cout << std::format("{}:\tNormal was {:.3f} times faster, NORM:{:.3f}ns OPTM:{:.3f}ns.\n",
			testName, times, normAvg, optiAvg);
	}
	else if (durationNormalNs.count() > durationOptimNs.count())
	{
		auto times = float((double)durationNormalNs.count() / (double)durationOptimNs.count());
		std::cout << std::format("{}:\tOptim was {:.3f} times faster, NORM:{:.3f}ns OPTM:{:.3f}ns.\n",
			testName, times, normAvg, optiAvg);
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
		auto zero = _mm_setzero_ps();
		auto val = _mm_set_ss(samples[i]);
		resultOptim[i] = _mm_cvt_ss2si(_mm_round_ss(zero, val, (_MM_FROUND_CEIL | _MM_FROUND_NO_EXC)));
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

#if USE_SSE
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
#endif 

void MathTest()
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

#if USE_SSE
	Vector<Vector4f> samplesV4;
	samplesV4.resize(sampleCount, Vector4f{});
	Vector<__m128> samplesSSE;
	samplesSSE.resize(sampleCount, __m128{});
#endif

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

#if USE_SSE
		auto f0 = distributionV4(generator);
		auto f1 = distributionV4(generator);
		auto f2 = distributionV4(generator);
		auto f3 = distributionV4(generator);

		samplesV4[i].Set(f0, f1, f2, f3);
		samplesSSE[i] = _mm_set_ps(f0, f1, f2, f3);
#endif
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

#if USE_SSE
	std::tie(durNorm, durOpt) = LenghtV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("LenghtV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = NormV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("NormV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);

	std::tie(durNorm, durOpt) = DistV4FTest(sampleCount, samplesV4, samplesSSE, resultNormalF, resultOptimF);
	ReportTest("DistV4FTest"sv, sampleCount, resultNormalF, resultOptimF, durNorm, durOpt);
#endif

	using prec = double;
	constexpr auto odeg = Vector3Real<prec>(90, -60, 15);
	constexpr auto orad = odeg * DEG2RAD<prec>;

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

	auto quatArray = Vector<std::pair<QuaternionReal<prec>, Vector3Real<prec>>>{ 
		{qf0,edeg0}, {qf1, edeg1}, {qf2, edeg2}, {qf3, edeg3}, {qf4, edeg4}, {qf5, edeg5}
	};
	decltype(quatArray) testArray, testArray2{};

	using typeInfo = typename refl::TypeInfo_t<decltype(quatArray)>::Type;

	auto json_ret = typeInfo::CreateJSON(quatArray, "quatMap"sv);
	if (!json_ret.has_value())
	{
		std::cout << json_ret.error() << std::endl;
		return ;
	}
	auto text = std::shared_ptr<char>(cJSON_Print(json_ret.value().get()), cJSON_free);

	std::cout << text.get() << std::endl;
	auto parsed = std::shared_ptr<cJSON>(cJSON_Parse(text.get()), cJSON_Delete);
	auto ret_parse = typeInfo::FromJSON(testArray, parsed.get(), "quatMap"sv);

	if (!ret_parse.has_value())
	{
		std::cout << ret_parse.error() << std::endl;
		return ;
	}

	auto ret_quatArrayDynSize = typeInfo::GetDynamicSize(quatArray);
	if (!ret_quatArrayDynSize.has_value())
	{
		std::cout << ret_quatArrayDynSize.error() << std::endl;
		return ;
	}
	MemoryStream ms{ (uint64)typeInfo::StaticSize + (uint64)ret_quatArrayDynSize.value() };
	
	auto res = typeInfo::ToStream(quatArray, ms);
	if (!res.has_value())
	{
		std::cout << res.error() << std::endl;
		return ;
	}

	ms.Seek(0);

	auto res2 = typeInfo::FromStream(testArray2, ms);
	if (!res2.has_value())
	{
		std::cout << res2.error() << std::endl;
		return ;
	}

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
		{
			auto ret_strorig = origTypeInfo::ToString(orig);
			if(!ret_strorig.has_value())
			{
				std::cout << "ERROR: Could not obtain the string representation of MathTest origTypeInfo::ToString(orig), " << ret_strorig.error() << std::endl;
				return;
			}
			auto ret_strtest1 = test1TypeInfo::ToString(test1);
			if(!ret_strtest1.has_value())
			{
				std::cout << "ERROR: Could not obtain the string representation of MathTest test1TypeInfo::ToString(test1), " << ret_strtest1.error() << std::endl;
				return;
			}
			std::cout << std::format("Badly JSON stream quaternion, expected: '{}' obtained: '{}'.", ret_strorig.value(), ret_strtest1.value()) << std::endl;
		}
		if (orig != test2)
		{
			auto ret_strorig = origTypeInfo::ToString(orig);
			if(!ret_strorig.has_value())
			{
				std::cout << "ERROR: Could not obtain the string representation of MathTest origTypeInfo::ToString(orig), " << ret_strorig.error() << std::endl;
				return;
			}
			auto ret_strtest2 = test2TypeInfo::ToString(test2);
			if(!ret_strtest2.has_value())
			{
				std::cout << "ERROR: Could not obtain the string representation of MathTest test2TypeInfo::ToString(test2), " << ret_strtest2.error() << std::endl;
				return;
			}
			std::cout << std::format("Badly Memory stream quaternion, expected: '{}' obtained: '{}'.", ret_strorig.value(), ret_strtest2.value()) << std::endl;
		}

		++origIt; ++test1It; ++test2It;
	}

	greaper::math::Vector2b vec {1,0};
	using vec_refl = greaper::refl::TypeInfo_t<decltype(vec)>;
	auto ret_json = vec_refl::Type::CreateJSON(vec, vec_refl::Name);
	if (!ret_json.has_value())
	{
		std::cout << "ERROR" << ret_json.error() << std::endl;
		return;
	}
	auto json = ret_json.value();
	auto text2 = std::shared_ptr<char>(cJSON_Print(json.get()), cJSON_free);

	std::cout << text2.get() << std::endl;
}