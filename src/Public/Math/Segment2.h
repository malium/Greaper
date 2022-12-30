/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_SEGMENT2_H
#define MATH_SEGMENT2_H 1

#include "MathPrerequisites.h"
#include "Base/Vector2Real.inl"

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
		INLINE void Set(const Segment2Real& other)noexcept
		{
			Begin = other.Begin;
			End = other.End;
		}

		NODISCARD INLINE T Length()const noexcept
		{
			return Begin.Distance(End);
		}
		NODISCARD INLINE constexpr Vector2Real<T> GetDirectionWithMagnitude()const noexcept
		{
			return (End - Begin);
		}
		NODISCARD INLINE Vector2Real<T> GetDirection()const noexcept
		{
			return GetDirectionWithMagnitude().GetNormalized();
		}
		NODISCARD INLINE constexpr Vector2Real<T> PointAt(T segmentPCT)const noexcept
		{
			return Lerp(Begin, End, segmentPCT);
		}
		NODISCARD INLINE constexpr Vector2Real<T> PointAtUnclamped(T segmentPCT)const noexcept
		{
			return LerpUnclamped(Begin, End, segmentPCT);
		}
		INLINE constexpr TReturn<Vector2Real<T>> Intersects(const Segment2Real<T>& other)const noexcept
		{
			std::tuple<bool, T, T> res = Impl::Line2LineIntersection(Begin, GetDirectionWithMagnitude(), other.Begin, other.GetDirectionWithMagnitude());
			if (std::get<0>(res))
			{
				T tA = std::get<1>(res);
				T tB = std::get<2>(res);
				Vector2Real<T> point = PointAtUnclamped(tA);
				if(IsPointInside(point) && other.IsPointInside(point))
					return Return::CreateSuccess(point);
			}
			return Return::CreateFailure<Vector2Real<T>>();
		}

		NODISCARD INLINE constexpr bool IsNearlyEqual(const Segment2Real& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return Begin.IsNearlyEqual(other.Begin, tolerance) && End.IsNearlyEqual(other.End, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const Segment2Real& other)const noexcept
		{
			return Begin.IsEqual(other.Begin) && End.IsEqual(other.End);
		}
	};

	template<class T>
	NODISCARD INLINE constexpr bool operator==(const Segment2Real<T>& left, const Segment2Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T>
	NODISCARD INLINE constexpr bool operator!=(const Segment2Real<T>& left, const Segment2Real<T>& right)noexcept { return !(left == right); }
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