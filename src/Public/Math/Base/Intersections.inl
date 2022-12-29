/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper::math::Impl
{
	/** Adapted from Freya Holm�r https://github.com/FreyaHolmer/Mathfs
	 * Directions doesn't need to be normalized
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

	template<class T>
	INLINE constexpr std::tuple<bool, T, T> Line2LineIntersection(const Vector3Real<T>& originA, const Vector3Real<T>& directionA,
		const Vector3Real<T>& originB, const Vector3Real<T>& directionB)noexcept
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