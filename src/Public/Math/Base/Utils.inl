/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#define MATH_USE_OPTIMIZATIONS 1

#if PLT_WINDOWS
#include <nmmintrin.h>
#include <ammintrin.h>
#elif PLT_LINUX
#include <x86intrin.h>
#endif
#include <cmath>


namespace greaper::math
{
	namespace Impl
	{
		template<class T>
		struct PI_t
		{
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
			static constexpr T Value = T(3.14159265358979323846);
		};
		template<class T>
		struct E_t
		{
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
			static constexpr T Value = T(2.71828182845904523536);
		};
		template<class T>
		struct SqrtTwo_t
		{
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
			static constexpr T Value = T(1.41421356237309504880);
		};
		template<class T>
		struct SqrtThree_t
		{
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
			static constexpr T Value = T(1.73205080756887729352);
		};
		template<class T>
		struct SqrtOneOverTwo_t
		{
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
			static constexpr T Value = T(0.70710678118654752440);
		};
		template<class T>
		struct SqrtOneOverThree_t
		{
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
			static constexpr T Value = T(0.57735026918962576450);
		};
	}
	
	template<class T> constexpr inline T PI = Impl::PI_t<T>::Value;
	template<class T> constexpr inline T TWO_PI = T(2) * PI<T>;
	template<class T> constexpr inline T HALF_PI = T(0.5) * PI<T>;
	template<class T> constexpr inline T THREE_HALF_PI = T(3) * HALF_PI<T>;
	template<class T> constexpr inline T ONEFOURTH_PI = T(0.25) * PI<T>;
	template<class T> constexpr inline T ONEOVER_PI = T(1) / PI<T>;
	template<class T> constexpr inline T ONEOVER_TWOPI = T(1) / TWO_PI<T>;
	template<class T> constexpr inline T E = Impl::E_t<T>::Value;
	template<class T> constexpr inline T SQRT_TWO = Impl::SqrtTwo_t<T>::Value;
	template<class T> constexpr inline T SQRT_THREE = Impl::SqrtThree_t<T>::Value;
	template<class T> constexpr inline T SQRT_ONEOVERTWO = Impl::SqrtOneOverTwo_t<T>::Value;
	template<class T> constexpr inline T SQRT_ONEOVERTHREE = Impl::SqrtOneOverThree_t<T>::Value;
	template<class T> constexpr inline T DEG2RAD = PI<T> / T(180);
	template<class T> constexpr inline T RAD2DEG = T(180) * ONEOVER_PI<T>;

	/* Converts a float to an integer with truncation towards zero. */
	INLINE int32 TruncInt(float f)noexcept
	{
		// Optimizations makes it x6.89 times faster than trunkf
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
	/* Converts a double to an integer with truncation towards zero. */
	INLINE double Trunc(double d)noexcept
	{
		return static_cast<double>(TruncInt(d));
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE int32 FloorInt(float f)noexcept
	{
		// Slower than floorf
		/*
		auto sign = _mm_set_ps1(-0.f);
		auto one = _mm_set_ps1(1.f);
		auto maxValue = _mm_set_ps1(2147483648.f);

		auto f = _mm_set_ss(f);
		auto t = _mm_cvtepi32_ps(_mm_cvtps_epi32(f));
		auto r = _mm_sub_ss(t, _mm_and_ps(_mm_cmplt_ss(f, t), one));
		auto m = _mm_cmple_ss(maxValue, _mm_andnot_ps(sign, f));

		r = _mm_or_ps(_mm_andnot_ps(m, r), _mm_and_ps(m, f));

		return static_cast<int32>(_mm_cvtss_f32(r));
		*/
		// Slower than floorf
		// return _mm_cvt_ss2si(_mm_round_ss(_mm_setzero_ps(), _mm_set_ss(f), (_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)));
		// Faster than floorf, sometimes returns wrong values
		// return _mm_cvt_ss2si(_mm_set_ss(f + f - 0.5f)) >> 1;
//#if MATH_USE_OPTIMIZATIONS
//		return _mm_cvt_ss2si(_mm_set_ss(f + f - 0.5f)) >> 1;
//#else
		return static_cast<int32>(std::floor(f));
//#endif
	}
	/* Converts a double to the nearest less or equal integer */
	INLINE int32 FloorInt(double f)noexcept
	{
//#if MATH_USE_OPTIMIZATIONS
//		return _mm_cvtsd_si32(_mm_set_sd(f + f - 0.5f)) >> 1;
//#else
		return static_cast<int32>(std::floor(f));
//#endif
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE int64 FloorInt64(float f)noexcept
	{
//#if MATH_USE_OPTIMIZATIONS
//		return _mm_cvtss_si64(_mm_set_ss(f + f - 0.5f)) >> 1;
//#else
		return static_cast<int64>(std::floor(f));
//#endif
	}
	/* Converts a double to the nearest less or equal integer */
	INLINE int64 FloorInt64(double f)noexcept
	{
//#if MATH_USE_OPTIMIZATIONS
//		return _mm_cvtsd_si64(_mm_set_sd(f + f - 0.5f)) >> 1;
//#else
		return static_cast<int64>(std::floor(f));
//#endif
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE float Floor(float f)noexcept
	{
		return static_cast<float>(FloorInt(f));
	}
	/* Converts a float to the nearest less or equal integer */
	INLINE double Floor(double d)noexcept
	{
		return static_cast<double>(FloorInt(d));
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	INLINE int32 RoundInt(float f)noexcept
	{
//#if MATH_USE_OPTIMIZATIONS
//		return _mm_cvt_ss2si(_mm_set_ss(f + f + 0.5f)) >> 1;
//#else
		return FloorInt(f + 0.5f);
//#endif
	}
	/* Converts a double to the nearest integer. Rounds up when the fraction is .5 */
	INLINE int32 RoundInt(double d)noexcept
	{
		return FloorInt(d + 0.5);
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	INLINE int64 RoundInt64(float f)noexcept
	{
		return FloorInt64(f + 0.5f);
	}
	/* Converts a double to the nearest integer. Rounds up when the fraction is .5 */
	INLINE int64 RoundInt64(double d)noexcept
	{
		return FloorInt64(d + 0.5);
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	INLINE float Round(float f)noexcept
	{
		return static_cast<float>(RoundInt(f));
	}
	/* Converts a double to the nearest integer. Rounds up when the fraction is .5 */
	INLINE double Round(double d)noexcept
	{
		return static_cast<double>(RoundInt(d));
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE int32 CeilInt(float f)noexcept
	{
//#if MATH_USE_OPTIMIZATIONS
//		return -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (f + f))) >> 1);
//#else
		return static_cast<int32>(std::ceil(f));
//#endif
	}
	/* Converts a double to the nearest greater or equal integer */
	INLINE int32 CeilInt(double d)noexcept
	{
		return static_cast<int32>(std::ceil(d));
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE int64 CeilInt64(float f)noexcept
	{
		return static_cast<int64>(std::ceil(f));
	}
	/* Converts a double to the nearest greater or equal integer */
	INLINE int64 CeilInt64(double d)noexcept
	{
		return static_cast<int64>(std::ceil(d));
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE float Ceil(float f)noexcept
	{
		return static_cast<float>(CeilInt(f));
	}
	/* Converts a float to the nearest greater or equal integer */
	INLINE double Ceil(double d)noexcept
	{
		return static_cast<double>(CeilInt(d));
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
	INLINE std::tuple<int32, float> Split(float val)noexcept
	{
		int32 i = TruncInt(val);
		return { i, val - float(i) };
	}
	/* Breaks the given value into an integer and a fractional part */
	INLINE std::tuple<int64, float> Split64(float val)noexcept
	{
		int64 i = TruncInt64(val);
		return { i, val - float(i) };
	}
	/* Breaks the given value into an integer and a fractional part */
	INLINE std::tuple<int32, double> Split(double val)noexcept
	{
		int32 i = TruncInt(val);
		return { i, val - double(i) };
	}
	/* Breaks the given value into an integer and a fractional part */
	INLINE std::tuple<int64, double> Split64(double val)noexcept
	{
		int64 i = TruncInt64(val);
		return { i, val - double(i) };
	}
	/* Returns the modulus of two integers also on negative values */
	INLINE constexpr int32 Modulus(int32 a, int32 b)noexcept
	{
		return ((a %= b) < 0) ? a + b : a;
	}
	/* Returns the modulus of two integers also on negative values */
	INLINE constexpr int64 Modulus(int64 a, int64 b)noexcept
	{
		return ((a %= b) < 0ull) ? a + b : a;
	}
	/* Returns the modulus of two integers also on negative values */
	INLINE constexpr float Modulus(float a, float b)noexcept
	{
		auto mod = Abs(a);
		auto bb = Abs(b);

		while (mod >= bb)
			mod -= bb;

		if (a < 0.f)
			return (-mod) + b;
		return mod;
	}
	/* Returns the modulus of two integers also on negative values */
	INLINE constexpr double Modulus(double a, double b)noexcept
	{
		auto mod = Abs(a);
		auto bb = Abs(b);

		while (mod >= bb)
			mod -= bb;

		if (a < 0.0)
			return (-mod) + b;
		return mod;
	}
	/* Returns e^val */
	INLINE float Exp(float val)noexcept
	{
		return std::exp(val);
	}
	/* Returns e^val */
	INLINE double Exp(double val)noexcept
	{
		return std::exp(val);
	}
	/* Returns ln(val) */
	INLINE float LogN(float val)noexcept
	{
		return std::log(val);
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
#if !GREAPER_DEBUG
		return std::log2f(val);
#else
		// Same speed as log2f in Release build but 2.5x faster in Debug build
		static constexpr float ONEOVERLOG2 = 1.4426950408889634f;
		return LogN(val) * ONEOVERLOG2;
#endif
	}
	/* Returns log2(val) */
	INLINE double Log2(double val)noexcept
	{
#if !GREAPER_DEBUG
		return std::log2(val);
#else
		// Same speed as log2f in Release build but 2.5x faster in Debug build
		static constexpr double ONEOVERLOG2 = 1.4426950408889634;
		return LogN(val) * ONEOVERLOG2;
#endif
	}
	INLINE float Sin(float val)noexcept
	{
		return std::sin(val);
	}
	INLINE double Sin(double val)noexcept
	{
		return std::sin(val);
	}
	INLINE float ASin(float val)noexcept
	{
		val = Clamp(val, -1.f, 1.f);
		return std::asin(val);
	}
	INLINE double ASin(double val)noexcept
	{
		val = Clamp(val, -1.0, 1.0);
		return std::asin(val);
	}
	INLINE float Sinh(float val)noexcept
	{
		return std::sinh(val);
	}
	INLINE double Sinh(double val)noexcept
	{
		return std::sinh(val);
	}
	INLINE float Cos(float val)noexcept
	{
		return std::cos(val);
	}
	INLINE double Cos(double val)noexcept
	{
		return std::cos(val);
	}
	INLINE float ACos(float val)noexcept
	{
		val = Clamp(val, -1.f, 1.f);
		return std::acos(val);
	}
	INLINE double ACos(double val)noexcept
	{
		val = Clamp(val, -1.0, 1.0);
		return std::acos(val);
	}
	INLINE float Cosh(float val)noexcept
	{
		return std::cosh(val);
	}
	INLINE double Cosh(double val)noexcept
	{
		return std::cosh(val);
	}
	INLINE float Tan(float val)noexcept
	{
		return std::tan(val);
	}
	INLINE double Tan(double val)noexcept
	{
		return std::tan(val);
	}
	INLINE float ATan(float val)noexcept
	{
		return std::atan(val);
	}
	INLINE double ATan(double val)noexcept
	{
		return std::atan(val);
	}
	INLINE float ATan2(float y, float x)noexcept
	{
		return std::atan2(y, x);
	}
	INLINE double ATan2(double y, double x)noexcept
	{
		return std::atan2(y, x);
	}
	INLINE float Tanh(float val)noexcept
	{
		return std::tanh(val);
	}
	INLINE double Tanh(double val)noexcept
	{
		return std::tanh(val);
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
		return std::sqrt(val);
	}
	INLINE double Sqrt(double val)noexcept
	{
		return std::sqrt(val);
	}
	INLINE float Pow(float base, float power)noexcept
	{
		return std::pow(base, power);
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
	INLINE constexpr int64 Pow(int64 base, int64 power)noexcept
	{
		int64 r = base;
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
#if 0 // Currently 1 / sqrt is faster than Carmack 
	INLINE float InvSqrtFast(float val)noexcept
	{
		float y = val;
		float y2 = val * 0.5f;
		int32 i = *reinterpret_cast<int32*>(&y);
		i = 0x5f3759df - (i >> 1);
		y = *reinterpret_cast<float*>(&i);
		y = y * (1.5f - (y2 * y * y));

#if 1
		y = y * (1.5f - (y2 * y * y));
#endif
		return y;
	}
	INLINE double InvSqrtFast(double val)noexcept
	{
		double y = val;
		double y2 = val * 0.5;
		int64 i = *reinterpret_cast<int64*>(&y);
		i = 0x5fe6eb50c7b537a9 - (i >> 1ull);
		y = *reinterpret_cast<double*>(&i);
		y = y * (1.5 - (y2 * y * y));

#if 1
		y = y * (1.5 - (y2 * y * y));
#endif
		return y;
	}
#endif
}