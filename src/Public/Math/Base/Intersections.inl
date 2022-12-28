/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../MathPrerequisites.h"
#include "../Vector2.h"

namespace greaper::math::Impl
{
	/** Adapted from Freya Holmér https://github.com/FreyaHolmer/Mathfs
	*/
	template<class T>
	INLINE constexpr std::tuple<bool, T, T> Line2LineIntersection(const Vector2Real<T>& originA, const Vector2Real<T>& directionA,
		const Vector2Real<T>& originB, const Vector2Real<T>& directionB)noexcept
	{
		T d = directionA.CrossProduct(directionB);
		if (Abs(d) < T(0.00001))
		{
			return { false, T(0), T(0) };
		}

		Vector2Real A2B = originB - originA;
		T pointA = A2B.CrossProduct(directionB) / d;
		T pointB = A2B.CrossProduct(directionA) / d;
		return { true, pointA, pointB };
	}
}