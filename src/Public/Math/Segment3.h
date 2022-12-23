/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_SEGMENT3_H
#define MATH_SEGMENT3_H 1

#include "MathPrerequisites.h"
#include "Vector3.h"

namespace greaper::math
{
	template<class T>
	class Segment3Real
	{
		static_assert(std::is_floating_point_v<T>, "Segment3Real can only work with float, double or long double types");
	public:
		using value_type = Vector3Real<T>;

		Vector3Real<T> Begin{};
		Vector3Real<T> End{};

		constexpr Segment3Real()noexcept = default;
		INLINE constexpr Segment3Real(Vector3Real<T> begin, Vector3Real<T> end)noexcept :Begin(begin), End(end) {  }

		INLINE void Set(Vector3Real<T> begin, Vector3Real<T> end)noexcept
		{
			Begin = begin;
			End = end;
		}

		INLINE T Length()const noexcept
		{
			return Begin.Distance(End);
		}

		INLINE constexpr bool IsNearlyEqual(const Segment3Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return Begin.IsNearlyEqual(other.Begin, tolerance) && End.IsNearlyEqual(other.End, tolerance);
		}
		INLINE constexpr bool IsEqual(const Segment3Real& other)const noexcept
		{
			return Begin.IsEqual(other.Begin) && End.IsEqual(other.End);
		}
	};
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Segment3Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Segment3Real<T>& s)const noexcept
		{
			return ComputeHash(s.Begin, s.End);
		}
	};
}

#endif /* MATH_SEGMENT3_H */