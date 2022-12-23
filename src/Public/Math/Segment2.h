/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_SEGMENT2_H
#define MATH_SEGMENT2_H 1

#include "MathPrerequisites.h"
#include "Vector2.h"

namespace greaper::math
{
	template<class T>
	class Segment2Real
	{
		static_assert(std::is_floating_point_v<T>, "Segment2Real can only work with float, double or long double types");
	public:
		using value_type = Vector2Real<T>;

		Vector2Real<T> Begin{};
		Vector2Real<T> End{};

		constexpr Segment2Real()noexcept = default;
		INLINE constexpr Segment2Real(Vector2Real<T> begin, Vector2Real<T> end)noexcept :Begin(begin), End(end) {  }

		INLINE void Set(Vector2Real<T> begin, Vector2Real<T> end)noexcept
		{
			Begin = begin;
			End = end;
		}

		INLINE T Length()const noexcept
		{
			return Begin.Distance(End);
		}

		INLINE constexpr bool IsNearlyEqual(const Segment2Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return Begin.IsNearlyEqual(other.Begin, tolerance) && End.IsNearlyEqual(other.End, tolerance);
		}
		INLINE constexpr bool IsEqual(const Segment2Real& other)const noexcept
		{
			return Begin.IsEqual(other.Begin) && End.IsEqual(other.End);
		}
	};
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Segment2Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Segment2Real<T>& s)const noexcept
		{
			return ComputeHash(s.Begin, s.End);
		}
	};
}

#endif /* MATH_SEGMENT2_H */