/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_LINE3_H
#define MATH_LINE3_H 1

#include "MathPrerequisites.h"
#include "Vector3.h"

namespace greaper::math
{
	template<class T>
	class Line3Real
	{
		static_assert(std::is_floating_point_v<T>, "Line3Real can only work with float, double or long double types");
	public:
		using value_type = Vector3Real<T>;

		Vector3Real<T> Origin{};
		Vector3Real<T> Direction{ T(0), T(1), T(0) };

		constexpr Line3Real()noexcept = default;
		INLINE constexpr Line3Real(Vector3Real<T> origin, Vector3Real<T> direction)noexcept :Origin(origin), Direction(direction) {  }

		INLINE void Set(Vector3Real<T> origin, Vector3Real<T> direction)noexcept
		{
			Origin = origin;
			Direction = direction;
		}

		NODISCARD INLINE constexpr bool IsNearlyEqual(const Line3Real& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return Origin.IsNearlyEqual(other.Origin, tolerance) && Direction.IsNearlyEqual(other.Direction, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const Line3Real& other)const noexcept
		{
			return Origin.IsEqual(other.Origin) && Direction.IsEqual(other.Direction);
		}
	};

	template<class T>
	NODISCARD INLINE constexpr bool operator==(const Line3Real<T>& left, const Line3Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T>
	NODISCARD INLINE constexpr bool operator!=(const Line3Real<T>& left, const Line3Real<T>& right)noexcept { return !(left == right); }
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Line3Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Line3Real<T>& s)const noexcept
		{
			return ComputeHash(s.Origin, s.Direction);
		}
	};
}

#endif /* MATH_LINE3_H */