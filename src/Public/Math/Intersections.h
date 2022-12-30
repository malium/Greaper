/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_INTERSECTIONS_H
#define MATH_INTERSECTIONS_H 1

#include "MathPrerequisites.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Segment2.h"
#include "Segment3.h"
#include "Line2.h"
#include "Line3.h"
#include <Core/Result.h>

namespace greaper::math
{
	namespace Impl
	{
		/** Adapted from Freya Holm�r https://github.com/FreyaHolmer/Mathfs
		 * Directions doesn't need to be normalized
		*/
		template<class T>
		INLINE constexpr TReturn<Vector2Real<T>> Line2LineIntersection(const Vector2Real<T>& originA, const Vector2Real<T>& directionA,
			const Vector2Real<T>& originB, const Vector2Real<T>& directionB)noexcept
		{
			T d = directionA.CrossProduct(directionB);
			if (Abs(d) < T(0.00001))
				return Return::CreateFailure<Vector2Real<T>>();

			Vector2Real A2B = originB - originA;
			T tA = A2B.CrossProduct(directionB) / d;
			T tB = A2B.CrossProduct(directionA) / d;
			return Return::CreateSuccess(directionA * tA + directionB * tB);
		}
		
		/**
		 * Adapted from Paul Bourke http://paulbourke.net/geometry/pointlineplane/
		 */
		template<class T>
		INLINE constexpr TReturn<Vector3Real<T>> Line2LineIntersection(const Vector3Real<T>& originA, const Vector3Real<T>& directionA,
			const Vector3Real<T>& originB, const Vector3Real<T>& directionB)noexcept
		{
			Vector3Real<T> p1 = originA;
			Vector3Real<T> p2 = originA + directionA;
			Vector3Real<T> p3 = originB;
			Vector3Real<T> p4 = originB + directionB;
			Vector3Real<T> p13 = p1 - p3;
			Vector3Real<T> p43 = p4 - p3;
			Vector3Real<T> p21 = p2 - p1;

			T d1343 = p13.DotProduct(p43);
			T d4321 = p43.DotProduct(p21);
			T d1321 = p13.DotProduct(p21);
			T d4343 = p43.DotProduct(p43);
			T d2121 = p21.DotProduct(p21);

			T denominator = d2121 * d4343 - d1321 * d4343;
			if(::IsNearlyEqual(denominator, T(0), MATH_TOLERANCE<T>))
				return Return::CreateFailure<Vector3Real<T>>();
			
			T numerator = d1343 * d4321 - d1321 * d4343;
			T mua = numerator / denominator;
			T mub = (d1343 + d4321 * mua) / d4343;

			Vector3Real<T> resultSegmentPoint1 = p1 + (mua * p21);
			Vector3Real<T> resultSegmentPoint2 = p3 + (mub * p43);
			return Return::CreateSuccess(resultSegmentPoint1);
		}
		constexpr auto s0 = Segment3f(Vector3f(5,5,4), Vector3f(10,10,6));
		constexpr auto s1 = Segment3f(Vector3f(5,5,5), Vector3f(10,10,3));
		constexpr auto t = Line2LineIntersection(s0.Begin, s0.GetDirectionWithMagnitude(), s1.Begin, s1.GetDirectionWithMagnitude());
	}

	template<class T>
	NODISCARD INLINE constexpr bool IsInside(const Segment2Real<T>& segment, const Vector2Real<T>& point)noexcept
	{
		Vector2Real<T> ba = segment.End - segment.Begin;
		Vector2Real<T> ca = point - segment.Begin;
		T cross = ba.CrossProduct(ca);
		if(!::IsNearlyEqual(cross, T(0), MATH_TOLERANCE<T>))
			return false;
		
		T dot = ba.DotProduct(ca);
		if(dot < T(0))
			return false;
		
		T sqrtLengthBA = ba.LengthSquared();
		return dot <= sqrtLengthBA;
	}
	
	template<class T>
	NODISCARD INLINE constexpr bool IsInside(const Segment3Real<T>& segment, const Vector3Real<T>& point)noexcept
	{
		
	}
}

#endif /* MATH_INTERSECTIONS_H */