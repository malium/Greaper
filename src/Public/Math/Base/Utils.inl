/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#define MATH_USE_OPTIMIZATIONS 1

#if COMPILER_MSVC
#include <nmmintrin.h>
#include <ammintrin.h>
#else
#include <x86intrin.h>
#endif
#include <cmath>


namespace greaper::math
{
	namespace Impl
	{
		template<class T>
		struct TOLERANCE_t
		{
			static_assert(std::is_floating_point_v<T>);

			static constexpr T Value = 
#ifndef MATH_TOLERANCE_VAL
				T(0.0001);
#else
				T(MATH_TOLERANCE_VAL);
#endif
		};
		template<class T>
		struct PI_t
		{
			static_assert(std::is_floating_point_v<T>);
			static constexpr T Value = T(3.1415926535897932384626433832795);
		};
		template<class T>
		struct E_t
		{
			static_assert(std::is_floating_point_v<T>);
			static constexpr T Value = T(2.7182818284590452353602874713527);
		};
		template<class T>
		struct GoldenRatio_t
		{
			static_assert(std::is_floating_point_v<T>);
			static constexpr T Value = T(1.6180339887498948482045868343656);

		};
		template<class T>
		struct SqrtTwo_t
		{
			static_assert(std::is_floating_point_v<T>);
			static constexpr T Value = T(1.4142135623730950488016887242097);
		};
		template<class T>
		struct SqrtThree_t
		{
			static_assert(std::is_floating_point_v<T>);
			static constexpr T Value = T(1.7320508075688772935274463415059);
		};
		template<class T>
		struct MathRetType { using Type = void; };
		template<> struct MathRetType<float> { using Type = float; };
		template<> struct MathRetType<double> { using Type = double; };
		template<> struct MathRetType<long double> { using Type = long double; };
		template<> struct MathRetType<int8> { using Type = float; };
		template<> struct MathRetType<int16> { using Type = float; };
		template<> struct MathRetType<int32> { using Type = float; };
		template<> struct MathRetType<int64> { using Type = double; };
		template<> struct MathRetType<uint8> { using Type = float; };
		template<> struct MathRetType<uint16> { using Type = float; };
		template<> struct MathRetType<uint32> { using Type = float; };
		template<> struct MathRetType<uint64> { using Type = double; };

		template<class T>
		using MathRetType_t = typename MathRetType<T>::Type;
	}
	
	template<class T> constexpr inline T MATH_TOLERANCE = Impl::TOLERANCE_t<T>::Value;
	template<class T> constexpr inline T PI = Impl::PI_t<T>::Value;
	template<class T> constexpr inline T TWO_PI = T(2) * PI<T>;
	template<class T> constexpr inline T HALF_PI = T(0.5) * PI<T>;
	template<class T> constexpr inline T THREE_HALF_PI = T(3) * HALF_PI<T>;
	template<class T> constexpr inline T ONEFOURTH_PI = T(0.25) * PI<T>;
	template<class T> constexpr inline T ONEOVER_PI = T(1) / PI<T>;
	template<class T> constexpr inline T ONEOVER_TWOPI = T(1) / TWO_PI<T>;
	template<class T> constexpr inline T E = Impl::E_t<T>::Value;
	template<class T> constexpr inline T GOLDEN_RATIO = Impl::GoldenRatio_t<T>::Value;
	template<class T> constexpr inline T SQRT_TWO = Impl::SqrtTwo_t<T>::Value;
	template<class T> constexpr inline T SQRT_THREE = Impl::SqrtThree_t<T>::Value;
	template<class T> constexpr inline T SQRT_ONEOVERTWO = T(1) / Impl::SqrtTwo_t<T>::Value;
	template<class T> constexpr inline T SQRT_ONEOVERTHREE = T(1) / Impl::SqrtThree_t<T>::Value;
	template<class T> constexpr inline T DEG2RAD = PI<T> / T(180);
	template<class T> constexpr inline T RAD2DEG = T(180) * ONEOVER_PI<T>;
	template<class T> constexpr inline T EPSILON = std::numeric_limits<T>::epsilon();

	template<class T> constexpr T Pow(T base, T power)noexcept;

	/* Converts a floating point value into an integer with truncation towards zero. */
	template<class T>
	NODISCARD INLINE constexpr int32 TruncInt(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
#if MATH_USE_OPTIMIZATIONS
				return _mm_cvtt_ss2si(_mm_set_ss(val));
#else
				return static_cast<int32>(truncf(val));
#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
#if MATH_USE_OPTIMIZATIONS
				return _mm_cvttsd_si32(_mm_set_sd(val));
#else
				return static_cast<int32>(trunc(val));
#endif
			}
			else // long double
			{
				return static_cast<int32>(truncl(val));
			}
		}
		else // integral
		{
			return static_cast<int32>(val);
		}
	}
	/* Converts a floating point value into an integer64 with truncation towards zero. */
	template<class T>
	NODISCARD INLINE constexpr int64 TruncInt64(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
#if MATH_USE_OPTIMIZATIONS
				return _mm_cvttss_si64(_mm_set_ss(val));
#else
				return static_cast<int64>(truncf(val));
#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
#if MATH_USE_OPTIMIZATIONS
				return _mm_cvttsd_si64(_mm_set_sd(val));
#else
				return static_cast<int64>(truc(val));
#endif
			}
			else // long double
			{
				return static_cast<int64>(truncl(val));
			}
		}
		else // integral
		{
			return static_cast<int64>(val);
		}
	}
	/* Truncates towards zero a floating point value. */
	template<class T>
	NODISCARD INLINE constexpr T Trunc(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
				return static_cast<float>(TruncInt<float>(val));
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<double>(TruncInt64<double>(val));
			}
			else // long double
			{
				return static_cast<T>(truncl(val));
			}
		}
		else // integral
		{
			return val;
		}
	}
	/* Converts a float to the nearest less or equal integer */
	template<class T>
	NODISCARD INLINE int32 FloorInt(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
				// Slower than floorf
				/*
				auto sign = _mm_set_ps1(-0.f);
				auto one = _mm_set_ps1(1.f);
				auto maxValue = _mm_set_ps1(2147483648.f);

				auto f = _mm_set_ss(val);
				auto t = _mm_cvtepi32_ps(_mm_cvtps_epi32(f));
				auto r = _mm_sub_ss(t, _mm_and_ps(_mm_cmplt_ss(f, t), one));
				auto m = _mm_cmple_ss(maxValue, _mm_andnot_ps(sign, f));

				r = _mm_or_ps(_mm_andnot_ps(m, r), _mm_and_ps(m, f));

				return static_cast<int32>(_mm_cvtss_f32(r));
				*/
				// Slower than floorf
				// return _mm_cvt_ss2si(_mm_round_ss(_mm_setzero_ps(), _mm_set_ss(val), (_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)));
				// Faster than floorf, sometimes returns wrong values
				// return _mm_cvt_ss2si(_mm_set_ss(val + val - 0.5f)) >> 1;
//#if MATH_USE_OPTIMIZATIONS
//				return _mm_cvt_ss2si(_mm_set_ss(val + val - 0.5f)) >> 1;
//#else
				return static_cast<int32>(floorf(val));
//#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<int32>(floor(val));
			}
			else // long double
			{
				return static_cast<int32>(floorl(val));
			}
		}
		else // integral
		{
			return static_cast<int32>(val);
		}
	}
	/* Converts a float to the nearest less or equal integer64 */
	template<class T>
	NODISCARD INLINE constexpr int64 FloorInt64(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
//#if MATH_USE_OPTIMIZATIONS
//				return _mm_cvtss_si64(_mm_set_ss(val + val - 0.5f)) >> 1;
//#else
				return static_cast<int64>(floorf(val));
//#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
//#if MATH_USE_OPTIMIZATIONS
//				return _mm_cvtsd_si64(_mm_set_sd(val + val - 0.5f)) >> 1;
//#else
				return static_cast<int64>(floor(val));
//#endif
			}
			else // long double
			{
				return static_cast<int64>(floorl(val));
			}
		}
		else // integral
		{
			return static_cast<int64>(val);
		}
	}
	/* Converts a float to the nearest less or equal integer */
	template<class T>
	NODISCARD INLINE constexpr T Floor(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
				return static_cast<float>(FloorInt<float>(val));
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<double>(FloorInt64<double>(val));
			}
			else // long double
			{
				return static_cast<T>(floorl(val));
			}
		}
		else // integral
		{
			return val;
		}
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	template<class T>
	NODISCARD INLINE constexpr int32 RoundInt(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
//#if MATH_USE_OPTIMIZATIONS
//				return _mm_cvt_ss2si(_mm_set_ss(val + val + 0.5f)) >> 1;
//#else
				return FloorInt<float>(val + 0.5f);
//#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return FloorInt<double>(val + 0.5);
			}
			else // long double
			{
				return static_cast<int32>(roundl(val));
			}
		}
		else // integral
		{
			return static_cast<int32>(val);
		}
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	template<class T>
	NODISCARD INLINE constexpr int64 RoundInt64(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
//#if MATH_USE_OPTIMIZATIONS
//				return _mm_cvtss_si64(_mm_set_ss(val + val + 0.5f)) >> 1;
//#else
				return FloorInt64<float>(val + 0.5f);
//#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return FloorInt64<double>(val + 0.5);
			}
			else // long double
			{
				return static_cast<int64>(roundl(val));
			}
		}
		else // integral
		{
			return static_cast<int64>(val);
		}
	}
	/* Converts a float to the nearest integer. Rounds up when the fraction is .5 */
	template<class T>
	NODISCARD INLINE constexpr T Round(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
				return static_cast<float>(RoundInt<float>(val));
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<double>(RoundInt64<double>(val));
			}
			else // long double
			{
				return static_cast<T>(roundl(val));
			}
		}
		else // integral
		{
			return val;
		}
	}
	/* Converts a float to the nearest greater or equal integer */
	template<class T>
	NODISCARD INLINE constexpr int32 CeilInt(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
//#if MATH_USE_OPTIMIZATIONS
//				return -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (val + val))) >> 1);
//#else
				return static_cast<int32>(ceilf(val));
//#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<int32>(ceil(val));
			}
			else // long double
			{
				return static_cast<int32>(ceill(val));
			}
		}
		else // integral
		{
			return static_cast<int32>(val);
		}
	}
	/* Converts a float to the nearest greater or equal integer64 */
	template<class T>
	NODISCARD INLINE constexpr int64 CeilInt64(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
//#if MATH_USE_OPTIMIZATIONS
//				return -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (val + val))) >> 1);
//#else
				return static_cast<int64>(ceilf(val));
//#endif
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<int64>(ceil(val));
			}
			else // long double
			{
				return static_cast<int64>(ceill(val));
			}
		}
		else // integral
		{
			return static_cast<int64>(val);
		}
	}
	/* Converts a float to the nearest greater or equal integer */
	template<class T>
	NODISCARD INLINE constexpr T Ceil(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
			{
				return static_cast<float>(CeilInt<float>(val));
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return static_cast<double>(CeilInt64<double>(val));
			}
			else // long double
			{
				return static_cast<T>(ceill(val));
			}
		}
		else // integral
		{
			return val;
		}
	}
	/* Returns the signed fractional part of a float */
	template<class T>
	NODISCARD INLINE constexpr T Fractional(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		return val - Trunc<T>(val);
	}
	/* Breaks the given value into an integer and a fractional part */
	template<class T>
	NODISCARD INLINE constexpr std::tuple<int32, T> Split(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		int32 i = TruncInt<T>(val);
		return { i, val - T(i) };
	}
	/* Breaks the given value into an integer and a fractional part */
	template<class T>
	NODISCARD INLINE constexpr std::tuple<int64, T> Split64(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		int64 i = TruncInt64<T>(val);
		return { i, val - T(i) };
	}
	/* Returns the modulus of two integers also on negative values */
	template<class T>
	NODISCARD INLINE constexpr T Modulus(T a, T b)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_integral_v<T>)
		{
			return ((a %= b) < T(0)) ? a + b : a;
		}
		else // is floating point
		{
			auto mod = Abs(a);
			auto bb = Abs(b);

			while (mod >= bb)
				mod -= bb;

			if (a < T(0))
				return (-mod) + b;
			return mod;
		}
	}
	/* Returns e^val */
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> Exp(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return expf(val);
			else if constexpr (std::is_same_v<T, double>)
				return exp(val);
			else // long double
				return expl(val);
		}
		else // integral
		{
			return Exp(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	/* Returns 2^val */
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> Exp2(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return exp2f(val);
			else if constexpr (std::is_same_v<T, double>)
				return exp2(val);
			else // long double
				return exp2l(val);
		}
		else // integral
		{
			return Pow(T(2), val);
		}
	}
	/* Returns ln(val) */
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> LogN(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return logf(val);
			else if constexpr (std::is_same_v<T, double>)
				return log(val);
			else // long double
				return logl(val);
		}
		else // integral
		{
			return LogN(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	/* Returns log[base](value) */
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> LogB(T base, T value)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return LogN(value) / LogN(base);
		else // integral
			return LogB(static_cast<Impl::MathRetType_t<T>>(base), static_cast<Impl::MathRetType_t<T>>(value));
	}
	/* Returns log2(val) */
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> Log2(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			static constexpr T ONEOVERLOG2 = T(1.4426950408889634073599246810019);
			return LogN(val) * ONEOVERLOG2;
		}
		else // integral
		{
			return Log2(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	/* Returns log10(val) */
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> Log10(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			static constexpr T ONEOVERLOG10 = T(0.43429448190325182765112891891661);
			return LogN(val) * ONEOVERLOG10;
		}
		else // integral
		{
			return Log10(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Sin(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return sinf(val);
			else if constexpr (std::is_same_v<T, double>)
				return sin(val);
			else // long double
				return sinl(val);
		}
		else // integral
		{
			return Sin(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ASin(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return asinf(val);
			else if constexpr (std::is_same_v<T, double>)
				return asin(val);
			else // long double
				return asinl(val);
		}
		else // integral
		{
			return ASin(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Sinh(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return sinhf(val);
			else if constexpr (std::is_same_v<T, double>)
				return sinh(val);
			else // long double
				return sinhl(val);
		}
		else // integral
		{
			return Sinh(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ASinh(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return asinhf(val);
			else if constexpr (std::is_same_v<T, double>)
				return asinh(val);
			else // long double
				return asinhl(val);
		}
		else // integral
		{
			return ASinh(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Cos(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return cosf(val);
			else if constexpr (std::is_same_v<T, double>)
				return cos(val);
			else // long double
				return cosl(val);
		}
		else // integral
		{
			return Cos(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ACos(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return acosf(val);
			else if constexpr (std::is_same_v<T, double>)
				return acos(val);
			else // long double
				return acosl(val);
		}
		else // integral
		{
			return ACos(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Cosh(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return coshf(val);
			else if constexpr (std::is_same_v<T, double>)
				return cosh(val);
			else // long double
				return coshl(val);
		}
		else // integral
		{
			return Cosh(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ACosh(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return acoshf(val);
			else if constexpr (std::is_same_v<T, double>)
				return acosh(val);
			else // long double
				return acoshl(val);
		}
		else // integral
		{
			return ACosh(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Tan(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return tanf(val);
			else if constexpr (std::is_same_v<T, double>)
				return tan(val);
			else // long double
				return tanl(val);
		}
		else // integral
		{
			return Tan(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ATan(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return atanf(val);
			else if constexpr (std::is_same_v<T, double>)
				return atan(val);
			else // long double
				return atanl(val);
		}
		else // integral
		{
			return ATan(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ATan2(T y, T x)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return atan2f(y, x);
			else if constexpr (std::is_same_v<T, double>)
				return atan2(y, x);
			else // long double
				return atan2l(y, x);
		}
		else // integral
		{
			return ATan2(static_cast<Impl::MathRetType_t<T>>(y), static_cast<Impl::MathRetType_t<T>>(x));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Tanh(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return tanhf(val);
			else if constexpr (std::is_same_v<T, double>)
				return tanh(val);
			else // long double
				return tanhl(val);
		}
		else // integral
		{
			return Tanh(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> ATanh(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return atanhf(val);
			else if constexpr (std::is_same_v<T, double>)
				return atanh(val);
			else // long double
				return atanhl(val);
		}
		else // integral
		{
			return ATanh(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE std::tuple<Impl::MathRetType_t<T>, Impl::MathRetType_t<T>> SinCos(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			T sin, cos;
			if constexpr (std::is_same_v<T, float>)
			{
				sincosf(val, &sin, &cos);
				return { sin, cos };
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				sincos(val, &sin, &cos);
				return { sin, cos };
			}
			else // long double
			{
				sincosl(val, &sin, &cos);
				return { sin, cos };
			}
		}
		else // integral
		{
			return SinCos(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Cosecant(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return T(1) / Sin(val);
		else // integral
			return Cosecant(static_cast<Impl::MathRetType_t<T>>(val));
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Secant(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return T(1) / Cos(val);
		else // integral
			return Secant(static_cast<Impl::MathRetType_t<T>>(val));
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Cotangent(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return T(1) / Tan(val);
		else // integral
			return Cotangent(static_cast<Impl::MathRetType_t<T>>(val));
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Versine(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return T(1) - Cos(val);
		else // integral
			return Versine(static_cast<Impl::MathRetType_t<T>>(val));
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Coversine(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return T(1) - Sin(val);
		else // integral
			return Coversine(static_cast<Impl::MathRetType_t<T>>(val));
	}
	template<class T>
	NODISCARD INLINE Impl::MathRetType_t<T> SmoothCosZeroToOne(const T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		return Cos(static_cast<Impl::MathRetType_t<T>>(val) * PI<Impl::MathRetType_t<T>>) * Impl::MathRetType_t<T>(-0.5) + Impl::MathRetType_t<T>(0.5);
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> Sqrt(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<T, float>)
				return sqrtf(val);
			else if constexpr (std::is_same_v<T, double>)
				return sqrt(val);
			else // long double
				return sqrtl(val);
		}
		else // integral
		{
			return Sqrt(static_cast<Impl::MathRetType_t<T>>(val));
		}
	}
	template<class T> 
	NODISCARD INLINE constexpr T Pow(T base, T power)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr(std::is_integral_v<T>)
		{
			T r = base;
			while (power > 1)
			{
				r *= base;
				--power;
			}
			return r;
		}
		else // is floating point
		{
			if constexpr (std::is_same_v<T, float>)
				return powf(base, power);
			else if constexpr (std::is_same_v<T, double>)
				return pow(base, power);
			else // long double
				return powl(base, power);
		}
	}
	template<class T> 
	NODISCARD INLINE Impl::MathRetType_t<T> InvSqrt(T val)noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		if constexpr (std::is_floating_point_v<T>)
			return T(1) / Sqrt(val);
		else // integral
			return InvSqrt(static_cast<Impl::MathRetType_t<T>>(val));
	}
 	// Currently 1 / sqrt is faster than Carmack 
	/*NODISCARD INLINE float InvSqrtFast(float val)noexcept
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
	NODISCARD INLINE double InvSqrtFast(double val)noexcept
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
	}*/
}