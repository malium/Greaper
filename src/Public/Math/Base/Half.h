/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_HALF_H
#define MATH_HALF_H 1

#include "../MathPrerequisites.h"

namespace greaper::math
{
	class Half
	{
		int16 m_Value = 0;

		union Cvt
		{
			int32 i;
			decltype(m_Value) t;
		};
		
		void _Set(float v)
		{
			__m128 v1 = _mm_set_ss(v);
			__m128i v2 = _mm_cvtps_ph(v1, 0);
			int32 res = _mm_cvtsi128_si32(v2);
			Cvt u{ res };
			m_Value = u.t;
		}
	public:
		constexpr Half()noexcept = default;
		template<class T, typename std::enable_if<std::is_convertible_v<T, float>, bool>::type = false>
		INLINE explicit Half(T v)noexcept
		{
			Set(v);
		}
		template<class T, typename std::enable_if<std::is_convertible_v<T, float>, bool>::type = false>
		INLINE explicit operator T ()const noexcept
		{
			return static_cast<T>(Get());
		}

		template<class T, typename std::enable_if<std::is_convertible_v<T, float>, bool>::type = false>
		INLINE void Set(T v)noexcept
		{
			Set(static_cast<float>(v));
		}
		INLINE float Get()const noexcept
		{
			Cvt u{ m_Value };
			__m128i v1 = _mm_cvtsi32_si128(u.i);
			__m128 v2 = _mm_cvtph_ps(v1);
			return _mm_cvtss_f32(v2);
		}
	};
}

#endif /* MATH_HALF_H */