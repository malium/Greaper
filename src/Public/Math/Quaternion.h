/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H 1

#include "MathPrerequisites.h"

namespace greaper::math
{
	template<class T>
	class QuaternionReal
	{
		static_assert(std::is_floating_point_v<T>, "QuaternionReal can only work with float, double or long double types");

	public:
		static constexpr sizet ComponentCount = 4;
		using Type_t = T;

		// Scalar part
		T W{ 0 };

		// Imaginary part
		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr QuaternionReal()noexcept = default;
		constexpr QuaternionReal(T w, T x, T y, T z)noexcept :W(w), X(x), Y(y), Z(z) {  }
		constexpr explicit QuaternionReal(const std::array<T, ComponentCount>& arr)noexcept :W(arr[0]), X(arr[1]), Y(arr[2]), Z(arr[3]) {  }
		INLINE constexpr QuaternionReal operator-()const noexcept { return { -W, -X, -Y, -Z }; }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Quaternion, but the index %lld was out of range.", index);
			return (&W)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Quaternion, but the index %lld was out of range.", index);
			return (&W)[index];
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { W, X, Y, Z };
		}
		INLINE void Set(const QuaternionReal& other)noexcept
		{
			W = other.W;
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		INLINE void Set(T w, T x, T y, T z)noexcept
		{
			W = w;
			X = x;
			Y = y;
			Z = z;
		}
		INLINE void SetZero()noexcept
		{
			W = T(0);
			X = T(0);
			Y = T(0);
			Z = T(0);
		}
		INLINE constexpr QuaternionReal Conjugated()const noexcept
		{
			return { W, -X, -Y, -Z };
		}
		INLINE void Conjugate()noexcept
		{
			*this = Conjugated();
		}
		INLINE constexpr T LengthSquared()const noexcept
		{
			return W * W + X * X + Y * Y + Z * Z;
		}
		INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		INLINE QuaternionReal Normalized(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return QuaternionReal{ W * invScale, X * invScale, Y * invScale, Z * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = (T)MATH_TOLERANCE)noexcept
		{
			*this = Normalized(tolerance);
		}

		INLINE constexpr bool IsNearlyEqual(const QuaternionReal& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(W, other.W, tolerance)
				&& ::IsNearlyEqual(X, other.X, tolerance)
				&& ::IsNearlyEqual(Y, other.Y, tolerance)
				&& ::IsNearlyEqual(Z, other.Z, tolerance);
		}
		INLINE constexpr bool IsEqual(const QuaternionReal& other)const noexcept
		{
			return W == other.W
				&& X == other.X
				&& Y == other.Y
				&& Z == other.Z;
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(W, T(0), tolerance)
				&& ::IsNearlyEqual(X, T(0), tolerance)
				&& ::IsNearlyEqual(Y, T(0), tolerance)
				&& ::IsNearlyEqual(Z, T(0), tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return W == T(0)
				&& X == T(0)
				&& Y == T(0)
				&& Z == T(0);
		}
		INLINE constexpr bool IsNearlyUnit(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(Length(), T(1), tolerance);
		}
		INLINE constexpr bool IsUnit()const noexcept
		{
			return Length() == T(1);
		}
		INLINE constexpr bool IsReal(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return !::IsNearlyEqual(W, T(0), tolerance)
				&& ::IsNearlyEqual(X, T(0), tolerance)
				&& ::IsNearlyEqual(Y, T(0), tolerance)
				&& ::IsNearlyEqual(Z, T(0), tolerance);
		}
		INLINE constexpr bool IsImaginary(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return !IsImaginary(tolerance);
		}

		static const QuaternionReal ZERO;
		static const QuaternionReal UNIT;
	};

	template<class T> const QuaternionReal<T> QuaternionReal<T>::ZERO{};
	template<class T> const QuaternionReal<T> QuaternionReal<T>::UNIT{ T(1), T(0), T(0), T(0) };

	template<class T> INLINE constexpr QuaternionReal<T> operator+(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return QuaternionReal<T>{ left.W + right.W, left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
	template<class T> INLINE constexpr QuaternionReal<T> operator-(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return QuaternionReal<T>{ left.W - right.W, left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
	template<class T> INLINE QuaternionReal<T>& operator+=(QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { left.W += right.W; left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
	template<class T> INLINE QuaternionReal<T>& operator-=(QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { left.W -= right.W; left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

	template<class T> INLINE constexpr QuaternionReal<T> operator*(const QuaternionReal<T>& left, T right)noexcept { return QuaternionReal<T>{ left.W * right, left.X * right, left.Y * right, left.Z * right }; }
	template<class T> INLINE constexpr QuaternionReal<T> operator/(const QuaternionReal<T>& left, T right)noexcept { float invRight = T(1) / right; return QuaternionReal<T>{ left.W * invRight, left.X * invRight, left.Y * invRight, left.Z * invRight }; }
	template<class T> INLINE constexpr QuaternionReal<T> operator*(T left, const QuaternionReal<T>& right)noexcept { return QuaternionReal<T>{ left * right.W, left * right.X, left * right.Y, left * right.Z }; }
	template<class T> INLINE QuaternionReal<T>& operator*=(QuaternionReal<T>& left, T right)noexcept { left.W *= right; left.X *= right; left.Y *= right; left.Z *= right; return left; }
	template<class T> INLINE QuaternionReal<T>& operator/=(QuaternionReal<T>& left, T right)noexcept { float invRight = T(1) / right; left.W *= invRight; left.X *= invRight; left.Y *= invRight; left.Z *= invRight; return left; }
	template<class T> INLINE constexpr QuaternionReal<T> operator*(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept
	{
		return {
			left.W * right.W - left.X * right.X - left.Y * right.Y - left.Z * right.Z,
			left.W * right.X - left.X * right.W - left.Y * right.Z - left.Z * right.Y,
			left.W * right.Y - left.X * right.Z - left.Y * right.W - left.Z * right.X,
			left.W * right.Z - left.X * right.Y - left.Y * right.X - left.Z * right.W
		};
	}
	template<class T> INLINE QuaternionReal<T>& operator*=(QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { left = (left * right); return left; }

	template<class T> INLINE constexpr bool operator==(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return left.W == right.W && left.X == right.X && left.Y == right.Y && left.Z == right.Z; }
	template<class T> INLINE constexpr bool operator!=(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return !(left == right); }
}

#endif /* MATH_QUATERNION_H */