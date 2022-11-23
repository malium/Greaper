/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#define MATH_USE_OPTIMIZATIONS 1

#include <intrin.h>

namespace greaper::math
{
	constexpr float PI = 3.14159265358979323846f;

	constexpr float TWO_PI = 2.f * greaper::math::PI;

	constexpr float HALF_PI = 0.5f * greaper::math::PI;

	constexpr float THREE_HALF_PI = 3.f * greaper::math::HALF_PI;

	constexpr float ONEFOURTH_PI = 0.25f * greaper::math::PI;

	constexpr float ONEOVER_PI = 1.f / greaper::math::PI;

	constexpr float ONEOVER_TWOPI = 1.f / greaper::math::TWO_PI;

	constexpr float E = 2.71828182845904523536f;

	constexpr float SQRT_TWO = 1.41421356237309504880f;

	constexpr float SQRT_THREE = 1.73205080756887729352f;

	constexpr float SQRT_1OVER2 = 0.70710678118654752440f;

	constexpr float SQRT_1OVER3 = 0.57735026918962576450f;

	constexpr float DEG2RAD = greaper::math::PI / 180.f;

	constexpr float RAD2DEG = 180 * greaper::math::ONEOVER_PI;

	constexpr float MS2SEC = 1000.f;

	constexpr float SEC2MS = 0.001f;
	/* Converts a float to an integer with truncation towards zero. */
	INLINE int32 TruncInt(float f)noexcept
	{
#if MATH_USE_OPTIMIZATIONS
		return _mm_cvtt_ss2si(_mm_set_ss(f));
		
#else
		return static_cast<int32>(std::truncf(f));
#endif
	}
	/* Converts a double to an integer with truncation towards zero. */
	INLINE int32 TruncInt(double d)noexcept
	{
#if MATH_USE_OPTIMIZATIONS
		return _mm_cvttsd_si32(_mm_set_sd(d));

#else
		return static_cast<int32>(std::trunc(d));
#endif
	}
	/* Converts a float to an integer64 with truncation towards zero. */
	INLINE int64 TruncInt64(float f)noexcept
	{
#if MATH_USE_OPTIMIZATIONS && ARCHITECTURE_X64
		return _mm_cvttss_si64(_mm_set_ss(f));

#else
		return static_cast<int64>(std::truncf(f));
#endif
	}
	/* Converts a double to an integer64 with truncation towards zero. */
	INLINE int64 TruncInt64(double d)noexcept
	{
#if MATH_USE_OPTIMIZATIONS && ARCHITECTURE_X64
		return _mm_cvttsd_si64(_mm_set_sd(d));

#else
		return static_cast<int64>(std::trunc(d));
#endif
	}
	/* Converts a float to an integer with truncation towards zero. */
	INLINE float Trunc(float f)noexcept
	{
		return static_cast<float>(TruncInt(f));
	}
	/* Converts a float to an integer with truncation towards zero. */
	INLINE double Trunc(double d)noexcept
	{
		return static_cast<double>(TruncInt(d));
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE int32 FloorInt(float f)noexcept
	{
#if MATH_USE_OPTIMIZATIONS
		return _mm_cvt_ss2si(_mm_set_ss(f + f - 0.5f)) >> 1;
#else
		return static_cast<int32>(std::floorf(f));
#endif
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE float Floor(float f)noexcept
	{
		return static_cast<float>(FloorInt(f));
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE double Floor(double d)noexcept
	{
		return floor(d);
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	INLINE int32 RoundInt(float f)noexcept
	{
#if MATH_USE_OPTIMIZATIONS
		return _mm_cvt_ss2si(_mm_set_ss(f + f + 0.5f)) >> 1;
#else
		return static_cast<int32>(std::lroundf(f));
#endif
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	INLINE float Round(float f)noexcept
	{
		return static_cast<float>(RoundInt(f));
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	INLINE double Round(double d)noexcept
	{
#if MATH_USE_OPTIMIZATIONS
		return Floor(d + 0.5);
#else
		return std::round(d);
#endif
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE int32 CeilInt(float f)noexcept
	{
#if MATH_USE_OPTIMIZATIONS
		return -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (f + f))) >> 1);
#else
		return static_cast<int32>(std::ceilf(f));
#endif
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE float CeilFloat(float f)noexcept
	{
		return static_cast<float>(CeilInt(f));
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE double CeilDouble(double d)noexcept
	{
		return std::ceil(d);
	}
	/* Returns the signed fractional part of a float */
	INLINE float Fractional(float f)noexcept
	{
		return f - Trunc(f);
	}
	/* Returns the signed fractional part of a float */
	INLINE double Fractional(double d)noexcept
	{
		return d - Trunc(d);
	}
	/* Breaks the given value into an integer and a fractional part */
	INLINE float Mod(float val, float& intPart)noexcept
	{
		return std::modff(val, &intPart);
	}
	/* Breaks the given value into an integer and a fractional part */
	INLINE double Mod(double val, double& intPart)noexcept
	{
		return std::modf(val, &intPart);
	}
	/* Returns e^val */
	INLINE float Exp(float val)noexcept
	{
		return std::expf(val);
	}
	/* Returns e^val */
	INLINE double Exp(double val)noexcept
	{
		return std::exp(val);
	}
	/* Returns ln(val) */
	INLINE float LogN(float val)noexcept
	{
		return std::logf(val);
	}
	/* Returns ln(val) */
	INLINE double LogN(double val)noexcept
	{
		return std::log(val);
	}
	/* Returns log[base](value) */
	INLINE float LogB(float base, float value)noexcept
	{
		return LogN(value) / LogN(base);
	}
	/* Returns log[base](value) */
	INLINE double LogB(double base, double value)noexcept
	{
		return LogN(value) / LogN(base);
	}
	/* Returns log2(val) */
	INLINE float Log2(float val)noexcept
	{
		static constexpr float ONEOVERLOG2 = 1.4426950408889634f;
		return LogN(val) * ONEOVERLOG2;
	}
	/* Returns log2(val) */
	INLINE double Log2(double val)noexcept
	{
		static constexpr double ONEOVERLOG2 = 1.4426950408889634;
		return LogN(val) * ONEOVERLOG2;
	}
	INLINE float Sin(float val)noexcept
	{
		return std::sinf(val);
	}
	INLINE double Sin(double val)noexcept
	{
		return std::sin(val);
	}
	INLINE float ASin(float val)noexcept
	{
		val = Clamp(val, -1.f, 1.f);
		return std::asinf(val);
	}
	INLINE double ASin(double val)noexcept
	{
		val = Clamp(val, -1.0, 1.0);
		return std::asin(val);
	}
	INLINE float Sinh(float val)noexcept
	{
		return std::sinhf(val);
	}
	INLINE double Sinh(double val)noexcept
	{
		return std::sinh(val);
	}
	INLINE float Cos(float val)noexcept
	{
		return std::cosf(val);
	}
	INLINE double Cos(double val)noexcept
	{
		return std::cos(val);
	}
	INLINE float ACos(float val)noexcept
	{
		val = Clamp(val, -1.f, 1.f);
		return std::acosf(val);
	}
	INLINE double ACos(double val)noexcept
	{
		val = Clamp(val, -1.0, 1.0);
		return std::acos(val);
	}
	INLINE float Tan(float val)noexcept
	{
		return std::tanf(val);
	}
	INLINE double Tan(double val)noexcept
	{
		return std::tan(val);
	}
	INLINE float ATan(float val)noexcept
	{
		return std::atanf(val);
	}
	INLINE double ATan(double val)noexcept
	{
		return std::atan(val);
	}
	INLINE float ATan2(float y, float x)noexcept
	{
		return std::atan2f(y, x);
	}
	INLINE double ATan2(double y, double x)noexcept
	{
		return std::atan2(y, x);
	}
	INLINE std::tuple<float, float> SinCos(float val)noexcept
	{
		return { Sin(val), Cos(val) };
	}
	INLINE std::tuple<double, double> SinCos(double val)noexcept
	{
		return { Sin(val), Cos(val) };
	}
	INLINE float Sqrt(float val)noexcept
	{
		return std::sqrtf(val);
	}
	INLINE double Sqrt(double val)noexcept
	{
		return std::sqrt(val);
	}
	INLINE float Pow(float base, float power)noexcept
	{
		return std::powf(base, power);
	}
	INLINE double Pow(double base, double power)noexcept
	{
		return std::pow(base, power);
	}
	INLINE constexpr int32 Pow(int32 base, int32 power)noexcept
	{
		int32 r = base;
		while (power > 1)
		{
			r *= base;
			--power;
		}
		return r;
	}
	INLINE float InvSqrt(float val)noexcept
	{
		return 1.f / Sqrt(val);
	}
	INLINE double InvSqrt(double val)noexcept
	{
		return 1.0 / Sqrt(val);
	}
}