/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#if PLT_WINDOWS
#include <nmmintrin.h>
#include <ammintrin.h>
#elif PLT_LINUX
#include <x86intrin.h>
#endif

namespace greaper::math::SSE
{
	/* Arithmetic */
	INLINE __m128 Add(__m128 left, __m128 right)noexcept
	{
		return _mm_add_ps(left, right);
	}
	INLINE __m128d Add(__m128d left, __m128d right)noexcept
	{
		return _mm_add_pd(left, right);
	}
	INLINE __m128i Add8(__m128i left, __m128i right)noexcept
	{
		return _mm_add_epi8(left, right);
	}
	INLINE __m128i Add16(__m128i left, __m128i right)noexcept
	{
		return _mm_add_epi16(left, right);
	}
	INLINE __m128i Add32(__m128i left, __m128i right)noexcept
	{
		return _mm_add_epi32(left, right);
	}
	INLINE __m128i Add64(__m128i left, __m128i right)noexcept
	{
		return _mm_add_epi64(left, right);
	}
	INLINE __m128 Sub(__m128 left, __m128 right)noexcept
	{
		return _mm_sub_ps(left, right);
	}
	INLINE __m128d Sub(__m128d left, __m128d right)noexcept
	{
		return _mm_sub_pd(left, right);
	}
	INLINE __m128i Sub8(__m128i left, __m128i right)noexcept
	{
		return _mm_sub_epi8(left, right);
	}
	INLINE __m128i Sub16(__m128i left, __m128i right)noexcept
	{
		return _mm_sub_epi16(left, right);
	}
	INLINE __m128i Sub32(__m128i left, __m128i right)noexcept
	{
		return _mm_sub_epi32(left, right);
	}
	INLINE __m128i Sub64(__m128i left, __m128i right)noexcept
	{
		return _mm_sub_epi64(left, right);
	}
	INLINE __m128 Mul(__m128 left, __m128 right)noexcept
	{
		return _mm_mul_ps(left, right);
	}
	INLINE __m128d Mul(__m128d left, __m128d right)noexcept
	{
		return _mm_mul_pd(left, right);
	}
	INLINE __m128i Mul32(__m128i left, __m128i right)noexcept
	{
		return _mm_mul_epi32(left, right);
	}
	INLINE __m128 Mul(float left, __m128 right)noexcept
	{
		return _mm_mul_ps(_mm_set_ps1(left), right);
	}
	INLINE __m128d Mul(double left, __m128d right)noexcept
	{
		return _mm_mul_pd(_mm_set_pd1(left), right);
	}
	INLINE __m128i Mul32(int32 left, __m128i right)noexcept
	{
		return _mm_mul_epi32(_mm_set1_epi32(left), right);
	}
	INLINE __m128 Mul(__m128 left, float right)noexcept
	{
		return _mm_mul_ps(left, _mm_set_ps1(right));
	}
	INLINE __m128d Mul(__m128d left, double right)noexcept
	{
		return _mm_mul_pd(left, _mm_set_pd1(right));
	}
	INLINE __m128i Mul32(__m128i left, int32 right)noexcept
	{
		return _mm_mul_epi32(left, _mm_set1_epi32(right));
	}
	INLINE __m128 Div(__m128 left, __m128 right)noexcept
	{
		return _mm_div_ps(left, right);
	}
	INLINE __m128d Div(__m128d left, __m128d right)noexcept
	{
		return _mm_div_pd(left, right);
	}
	INLINE __m128 Div(float left, __m128 right)noexcept
	{
		return _mm_div_ps(_mm_set_ps1(left), right);
	}
	INLINE __m128d Div(double left, __m128d right)noexcept
	{
		return _mm_div_pd(_mm_set_pd1(left), right);
	}
	INLINE __m128 Div(__m128 left, float right)noexcept
	{
		return Mul(left, 1.f / right);
	}
	INLINE __m128d Div(__m128d left, double right)noexcept
	{
		return Mul(left, 1.0 / right);
	}

	/* Comparision */
	INLINE bool Equal(__m128 left, __m128 right)noexcept
	{
		/*
		auto cmp = _mm_castps_si128(_mm_cmpeq_ps(left, right));
		auto res = _mm_movemask_epi8(cmp);
		*/
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpeq_ps(left, right))) == 0xFFFF;
	}
	INLINE bool NearlyEqual(__m128 left, __m128 right, float tolerance = MATH_TOLERANCE)noexcept
	{
		/*
		auto mask = _mm_set_ps1(-0.f);
		auto t = _mm_set_ps1(tolerance);

		auto f = _mm_sub_ps(left, right);
		auto abs = _mm_andnot_ps(mask, f);
		auto cmp = _mm_castps_si128(_mm_cmple_ps(abs, t));
		auto res = _mm_movemask_epi8(cmp);
		return res == 0xFFFF;
		*/
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmple_ps(_mm_andnot_ps(_mm_set_ps1(-0.f), _mm_sub_ps(left, right)), _mm_set_ps1(tolerance)))) == 0xFFFF;
	}

	/* Vector functions */
	INLINE float DotProduct(__m128 a, __m128 b)noexcept
	{
#define DOTPRODUCT_VER 2
#if DOTPRODUCT_VER == 0 // Same speed as Vector4 implementation
		auto m = Mul(a, b);
		alignas(16) float values[4];
		_mm_store_ps(values, m);
		return values[0] + values[1] + values[2] + values[3];
#elif DOTPRODUCT_VER == 1 // Slower than the other implementations
		return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xFF));
#elif DOTPRODUCT_VER == 2 // Faster than other implementations
		auto m0 = Mul(a, b);
		auto m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(2, 3, 0, 1));
		m0 = _mm_add_ps(m0, m1);
		m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(0, 1, 2, 3));
		m0 = _mm_add_ps(m0, m1);

		return _mm_cvtss_f32(m0);
#elif DOTPRODUCT_VER == 3 // A bit slower than Vector4 implementation
		auto m0 = Mul(a, b);
		m0 = _mm_hadd_ps(m0, m0);
		m0 = _mm_hadd_ps(m0, m0);
		return _mm_cvtss_f32(m0);
#endif
#undef DOTPRODUCT_VER
	}
	INLINE float LengthSquared(__m128 v)noexcept
	{
		return DotProduct(v, v);
	}
	INLINE float Length(__m128 v)noexcept
	{
		return Sqrt(LengthSquared(v));
	}
	INLINE float DistanceSquared(__m128 a, __m128 b)noexcept
	{
		auto t = Sub(a, b);
		auto m = Mul(t, t);
		alignas(16) float values[4];
		_mm_store_ps(values, m);
		return values[0] + values[1] + values[2] + values[3];
	}
	INLINE float Distance(__m128 a, __m128 b)noexcept
	{
		return Sqrt(DistanceSquared(a, b));
	}
	INLINE __m128 Normalize(__m128 v, float tolerance = MATH_TOLERANCE)noexcept
	{
		auto lenSqrt = LengthSquared(v);
		if (lenSqrt > Square(tolerance))
		{
			auto invScale = InvSqrt(lenSqrt);
			return Mul(v, _mm_set_ps1(invScale));
		}
		return v;
	}
}