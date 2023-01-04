/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#if COMPILER_MSVC
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

namespace greaper::math::SSE
{
	using Vector4f = __m128;
	using Vector4i = __m128i;
	using Vector2d = __m128d;

	/* Basic functions */
	INLINE Vector4f CreateV4f()noexcept
	{
		return _mm_setzero_ps();
	}
	INLINE Vector4f CreateV4f(float a)noexcept
	{
		return _mm_set_ps1(a);
	}
	INLINE Vector4f CreateV4f(float x, float y, float z, float w)noexcept
	{
		return _mm_set_ps(x, y, z, w);
	}
	INLINE Vector2d CreateV2d()noexcept
	{
		return _mm_setzero_pd();
	}
	INLINE Vector2d CreateV2d(double a)noexcept
	{
		return _mm_set_pd1(a);
	}
	INLINE Vector2d CreateV2d(double x, double y)noexcept
	{
		return _mm_set_pd(x, y);
	}
	INLINE Vector4i CreateV4i()noexcept
	{
		return _mm_setzero_si128();
	}
	INLINE Vector4i CreateV4i(int32 a)noexcept
	{
		return _mm_set1_epi32(a);
	}
	INLINE Vector4i CreateV4i(int32 x, int32 y, int32 z, int32 w)noexcept
	{
		return _mm_set_epi32(x, y, z, w);
	}
	INLINE std::tuple<float, float, float, float> Extract(Vector4f v)noexcept
	{
		alignas(16) float values[4];
		_mm_store_ps(values, v);
		return { values[0], values[1], values[2], values[3] };
	}
	INLINE std::tuple<double, double> Extract(Vector2d v)noexcept
	{
		alignas(16) double values[2];
		_mm_store_pd(values, v);
		return { values[0], values[1] };
	}
	INLINE std::tuple<int32, int32, int32, int32> Extract(Vector4i v)noexcept
	{
		std::tuple<int32, int32, int32, int32> values;
		
		std::get<0>(values) = _mm_extract_epi32(v, 0);
		std::get<1>(values) = _mm_extract_epi32(v, 1);
		std::get<2>(values) = _mm_extract_epi32(v, 2);
		std::get<3>(values) = _mm_extract_epi32(v, 3);
		return values;
	}
	/* Arithmetic */
	INLINE Vector4f Add(Vector4f left, Vector4f right)noexcept
	{
		return _mm_add_ps(left, right);
	}
	INLINE Vector2d Add(Vector2d left, Vector2d right)noexcept
	{
		return _mm_add_pd(left, right);
	}
	INLINE Vector4i Add(Vector4i left, Vector4i right)noexcept
	{
		return _mm_add_epi32(left, right);
	}
	INLINE Vector4f Sub(Vector4f left, Vector4f right)noexcept
	{
		return _mm_sub_ps(left, right);
	}
	INLINE Vector2d Sub(Vector2d left, Vector2d right)noexcept
	{
		return _mm_sub_pd(left, right);
	}
	INLINE Vector4i Sub(Vector4i left, Vector4i right)noexcept
	{
		return _mm_sub_epi32(left, right);
	}
	INLINE Vector4f Mul(Vector4f left, Vector4f right)noexcept
	{
		return _mm_mul_ps(left, right);
	}
	INLINE Vector2d Mul(Vector2d left, Vector2d right)noexcept
	{
		return _mm_mul_pd(left, right);
	}
	INLINE Vector4i Mul(Vector4i left, Vector4i right)noexcept
	{
		return _mm_mul_epi32(left, right);
	}
	INLINE Vector4f Mul(float left, Vector4f right)noexcept
	{
		return _mm_mul_ps(_mm_set_ps1(left), right);
	}
	INLINE Vector2d Mul(double left, Vector2d right)noexcept
	{
		return _mm_mul_pd(_mm_set_pd1(left), right);
	}
	INLINE Vector4i Mul(int32 left, Vector4i right)noexcept
	{
		return _mm_mul_epi32(_mm_set1_epi32(left), right);
	}
	INLINE Vector4f Mul(Vector4f left, float right)noexcept
	{
		return _mm_mul_ps(left, _mm_set_ps1(right));
	}
	INLINE Vector2d Mul(Vector2d left, double right)noexcept
	{
		return _mm_mul_pd(left, _mm_set_pd1(right));
	}
	INLINE Vector4i Mul(Vector4i left, int32 right)noexcept
	{
		return _mm_mul_epi32(left, _mm_set1_epi32(right));
	}
	INLINE Vector4f Div(Vector4f left, Vector4f right)noexcept
	{
		return _mm_div_ps(left, right);
	}
	INLINE Vector2d Div(Vector2d left, Vector2d right)noexcept
	{
		return _mm_div_pd(left, right);
	}
	INLINE Vector4i Div(Vector4i left, Vector4i right)noexcept
	{
		auto l = Extract(left);
		auto r = Extract(right);

		auto i0 = std::get<0>(l) / std::get<0>(r);
		auto i1 = std::get<1>(l) / std::get<1>(r);
		auto i2 = std::get<2>(l) / std::get<2>(r);
		auto i3 = std::get<3>(l) / std::get<3>(r);
		return CreateV4i(i0, i1, i2, i3);
	}
	INLINE Vector4f Div(float left, Vector4f right)noexcept
	{
		return _mm_div_ps(_mm_set_ps1(left), right);
	}
	INLINE Vector2d Div(double left, Vector2d right)noexcept
	{
		return _mm_div_pd(_mm_set_pd1(left), right);
	}
	INLINE Vector4i Div(int32 left, Vector4i right)noexcept
	{
		auto r = Extract(right);

		auto i0 = left / std::get<0>(r);
		auto i1 = left / std::get<1>(r);
		auto i2 = left / std::get<2>(r);
		auto i3 = left / std::get<3>(r);
		return CreateV4i(i0, i1, i2, i3);
	}
	INLINE Vector4f Div(Vector4f left, float right)noexcept
	{
		return Mul(left, 1.f / right);
	}
	INLINE Vector2d Div(Vector2d left, double right)noexcept
	{
		return Mul(left, 1.0 / right);
	}
	INLINE Vector4i Div(Vector4i left, int32 right)noexcept
	{
		auto l = Extract(left);

		auto i0 = std::get<0>(l) / right;
		auto i1 = std::get<1>(l) / right;
		auto i2 = std::get<2>(l) / right;
		auto i3 = std::get<3>(l) / right;
		return CreateV4i(i0, i1, i2, i3);
	}

	/* Comparision */
	INLINE bool Equal(Vector4f left, Vector4f right)noexcept
	{
		/*
		auto cmp = _mm_castps_si128(_mm_cmpeq_ps(left, right));
		auto res = _mm_movemask_epi8(cmp);
		*/
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpeq_ps(left, right))) == 0xFFFF;
	}
	INLINE bool NearlyEqual(Vector4f left, Vector4f right, float tolerance = MATH_TOLERANCE<float>)noexcept
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
	INLINE float DotProduct(Vector4f a, Vector4f b)noexcept
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
		m0 = Add(m0, m1);
		m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(0, 1, 2, 3));
		m0 = Add(m0, m1);

		return _mm_cvtss_f32(m0);
#elif DOTPRODUCT_VER == 3 // A bit slower than Vector4 implementation
		auto m0 = Mul(a, b);
		m0 = _mm_hadd_ps(m0, m0);
		m0 = _mm_hadd_ps(m0, m0);
		return _mm_cvtss_f32(m0);
#endif
#undef DOTPRODUCT_VER
	}
	INLINE float LengthSquared(Vector4f v)noexcept
	{
		return DotProduct(v, v);
	}
	INLINE float Length(Vector4f v)noexcept
	{
		return Sqrt(LengthSquared(v));
	}
	INLINE float DistanceSquared(Vector4f a, Vector4f b)noexcept
	{
		auto t = Sub(a, b);
		auto m0 = Mul(t, t);
#define DISTANCE_VER 1
#if DISTANCE_VER == 0 // Faster than Vector4 implementation
		alignas(16) float values[4];
		_mm_store_ps(values, m0);
		return values[0] + values[1] + values[2] + values[3];
#elif DISTANCE_VER == 1 // A bit faster than VER 0 & 2
		auto m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(2, 3, 0, 1));
		m0 = Add(m0, m1);
		m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(0, 1, 2, 3));
		m0 = Add(m0, m1);

		return _mm_cvtss_f32(m0);
#elif DISTANCE_VER == 2 // Same speed as VER 0
		m0 = _mm_hadd_ps(m0, m0);
		m0 = _mm_hadd_ps(m0, m0);
		return _mm_cvtss_f32(m0);
#endif
#undef DISTANCE_VER
	}
	INLINE float Distance(Vector4f a, Vector4f b)noexcept
	{
		return Sqrt(DistanceSquared(a, b));
	}
	INLINE Vector4f Normalize(Vector4f v, float tolerance = MATH_TOLERANCE<float>)noexcept
	{
		auto lenSqrt = LengthSquared(v);
		if (lenSqrt > Square(tolerance))
		{
			auto invScale = InvSqrt(lenSqrt);
			return Mul(v, _mm_set_ps1(invScale));
		}
		return v;
	}
	INLINE Vector4f Clamp(Vector4f val, float min, float max)noexcept
	{
		/*
		auto m0 = _mm_set_ps1(min);
		auto m1 = _mm_set_ps1(max);
		auto t0 = _mm_max_ps(val, m0);
		auto t1 = _mm_min_ps(t0, m1);
		return t1;
		*/
		return _mm_min_ps(_mm_max_ps(val, _mm_set_ps1(min)), _mm_set_ps1(max));
	}
	INLINE Vector4f Clamp(Vector4f val, Vector4f min, Vector4f max)noexcept
	{
		/*
		auto t0 = _mm_max_ps(val, min);
		auto t1 = _mm_min_ps(t0, max);
		return t1;
		*/
		return _mm_min_ps(_mm_max_ps(val, min), max);
	}
	INLINE Vector4f Sign(Vector4f v)noexcept
	{
		return _mm_and_ps(v, _mm_set_ps1(-0.f));
	}
}