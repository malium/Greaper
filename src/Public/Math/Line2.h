/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_LINE2_H
#define MATH_LINE2_H 1

#include "MathPrerequisites.h"
#include "Vector2.h"

namespace greaper::math
{
	template<class T>
	class Line2Real
	{
		static_assert(std::is_floating_point_v<T>, "Line2Real can only work with float, double or long double types");
	public:
		using value_type = Vector2Real<T>;

		Vector2Real<T> Origin{};
		Vector2Real<T> Direction{ T(0), T(1) };

		constexpr Line2Real()noexcept = default;
		INLINE constexpr Line2Real(Vector2Real<T> origin, Vector2Real<T> direction)noexcept :Origin(origin), Direction(direction) {  }

		INLINE void Set(Vector2Real<T> origin, Vector2Real<T> direction)noexcept
		{
			Origin = origin;
			Direction = direction;
		}
		
		INLINE constexpr bool IsNearlyEqual(const Line2Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return Origin.IsNearlyEqual(other.Origin, tolerance) && Direction.IsNearlyEqual(other.Direction, tolerance);
		}
		INLINE constexpr bool IsEqual(const Line2Real& other)const noexcept
		{
			return Origin.IsEqual(other.Origin) && Direction.IsEqual(other.Direction);
		}
	};
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Line2Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Line2Real<T>& s)const noexcept
		{
			return ComputeHash(s.Origin, s.Direction);
		}
	};
}

#endif /* MATH_LINE2_H */