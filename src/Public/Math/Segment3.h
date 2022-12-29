/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_SEGMENT3_H
#define MATH_SEGMENT3_H 1

#include "MathPrerequisites.h"
#include "Vector3.h"
#include <Core/Result.h>
#include "Base/Intersections.inl"

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
		INLINE void Set(const Segment3Real& other)noexcept
		{
			Begin = other.Begin;
			End = other.End;
		}

		NODISCARD INLINE T Length()const noexcept
		{
			return Begin.Distance(End);
		}
		NODISCARD INLINE constexpr Vector3Real<T> GetDirectionWithMagnitude()const noexcept
		{
			return (End - Begin);
		}
		NODISCARD INLINE Vector3Real<T> GetDirection()const noexcept
		{
			return GetDirectionWithMagnitude().GetNormalized();
		}
		NODISCARD INLINE constexpr Vector3Real<T> PointAt(T segmentPCT)const noexcept
		{
			return Lerp(Begin, End, segmentPCT);
		}
		NODISCARD INLINE constexpr Vector3Real<T> PointAtUnclamped(T segmentPCT)const noexcept
		{
			return LerpUnclamped(Begin, End, segmentPCT);
		}
		NODISCARD INLINE constexpr bool IsPointInside(const Vector3Real<T>& point)const noexcept
		{
			Vector3Real<T> ba = End - Begin;
			Vector3Real<T> ca = point - Begin;
			T cross = ba.CrossProduct(ca);
			if(!::IsNearlyEqual(cross, T(0), MATH_TOLERANCE<T>))
				return false;
			
			T dot = ba.DotProduct(ca);
			if(dot < T(0))
				return false;
			
			T sqrtLengthBA = ba.LengthSquared();
			return dot <= sqrtLengthBA;
		}
		INLINE constexpr TReturn<Vector3Real<T>> Intersects(const Segment3Real<T>& other)const noexcept
		{
			std::tuple<bool, T, T> res = Impl::Line2LineIntersection(Begin, GetDirectionWithMagnitude(), other.Begin, other.GetDirectionWithMagnitude());
			if (std::get<0>(res))
			{
				T tA = std::get<1>(res);
				T tB = std::get<2>(res);
				Vector3Real<T> point = PointAtUnclamped(tA);
				if(IsPointInside(point) && other.IsPointInside(point))
					return Return::CreateSuccess(point);
			}
			return Return::CreateFailure<Vector3Real<T>>();
		}

		NODISCARD INLINE constexpr bool IsNearlyEqual(const Segment3Real& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return Begin.IsNearlyEqual(other.Begin, tolerance) && End.IsNearlyEqual(other.End, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const Segment3Real& other)const noexcept
		{
			return Begin.IsEqual(other.Begin) && End.IsEqual(other.End);
		}
	};
	
	template<class T>
	NODISCARD INLINE constexpr bool operator==(const Segment3Real<T>& left, const Segment3Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T>
	NODISCARD INLINE constexpr bool operator!=(const Segment3Real<T>& left, const Segment3Real<T>& right)noexcept { return !(left == right); }
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