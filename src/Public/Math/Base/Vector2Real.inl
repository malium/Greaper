/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR2REAL_H
#define MATH_VECTOR2REAL_H 1

#include "../MathPrerequisites.h"
#include <Core/StringUtils.h>
#include <array>

namespace greaper::math
{
	template<class T>
	class Vector2Real
	{
		static_assert(std::is_floating_point_v<T>, "Vector2Real can only work with float, double or long double types");

		template<class U> struct Print {  };
		template<> struct Print<float> { static constexpr auto fmt = "%f, %f"; };
		template<> struct Print<double> { static constexpr auto fmt = "%lf, %lf"; };
		template<> struct Print<long double> { static constexpr auto fmt = "%Lf, %Lf"; };

		template<class U> struct Scan {  };
		template<> struct Scan<float> { static constexpr auto fmt = "%f, %f"; };
		template<> struct Scan<double> { static constexpr auto fmt = "%lf, %lf"; };
		template<> struct Scan<long double> { static constexpr auto fmt = "%Lf, %Lf"; };

	public:
		static constexpr sizet ComponentCount = 2;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };

		constexpr Vector2Real()noexcept = default;
		INLINE constexpr Vector2Real(T x, T y)noexcept :X(x), Y(y) {  }
		INLINE constexpr explicit Vector2Real(const std::array<T, ComponentCount>& arr) :X(arr[0]), Y(arr[1]) {  }
		INLINE constexpr Vector2Real operator-()const noexcept { return { -X, -Y }; }

		NODISCARD INLINE T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y };
		}
		INLINE void Set(const Vector2Real& other)noexcept
		{
			X = other.X;
			Y = other.Y;
		}
		INLINE void Set(T x, T y)noexcept
		{
			X = x;
			Y = y;
		}
		INLINE void SetZero()noexcept
		{
			X = T(0);
			Y = T(0);
		}
		NODISCARD INLINE constexpr T DotProduct(const Vector2Real& other)const noexcept
		{
			return X * other.X + Y * other.Y;
		}
		NODISCARD INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		NODISCARD INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		NODISCARD INLINE constexpr T DistSquared(const Vector2Real& other)const noexcept
		{
			return Square(X - other.X) + Square(Y - other.Y);
		}
		NODISCARD INLINE T Distance(const Vector2Real& other)const noexcept
		{
			return Sqrt(DistSquared(other));
		}
		NODISCARD INLINE constexpr T CrossProduct(const Vector2Real& other)const noexcept
		{
			return X * other.Y - Y * other.X;
		}
		NODISCARD INLINE Vector2Real GetNormalized(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return Vector2Real{ X * invScale, Y * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = MATH_TOLERANCE<T>)noexcept
		{
			*this = GetNormalized(tolerance);
		}
		NODISCARD INLINE constexpr bool IsNearlyEqual(const Vector2Real& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(X, other.X, tolerance) && ::IsNearlyEqual(Y, other.Y, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const Vector2Real& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		NODISCARD INLINE constexpr bool IsNearlyZero(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0) }, tolerance);
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0) });
		}
		NODISCARD INLINE constexpr bool IsNearlyUnit(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(Length(), T(1), tolerance);
		}
		NODISCARD INLINE constexpr bool IsUnit()const noexcept
		{
			return Length() == T(1);
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y;
		}
		NODISCARD INLINE constexpr bool AreComponentsNearlyEqual(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(X, Y, tolerance);
		}
		NODISCARD INLINE constexpr Vector2Real GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y) };
		}
		NODISCARD INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(X, Y);
		}
		NODISCARD INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max(::Abs(X), ::Abs(Y));
		}
		NODISCARD INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(X, Y);
		}
		NODISCARD INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min(::Abs(X), ::Abs(Y));
		}
		NODISCARD INLINE constexpr Vector2Real GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector2Real{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector2Real GetClamped(const Vector2Real& min, const Vector2Real& max)const noexcept
		{
			return Vector2Real{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y)
			};
		}
		NODISCARD INLINE constexpr Vector2Real GetSignVector()const noexcept
		{
			return Vector2Real{ ::Sign(X), ::Sign(Y) };
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y);
		}

		static const Vector2Real ZERO;
		static const Vector2Real UNIT;
	};

	template<class T> const Vector2Real<T> Vector2Real<T>::ZERO = Vector2Real<T>{};
	template<class T> const Vector2Real<T> Vector2Real<T>::UNIT = Vector2Real<T>((T)1, (T)1);

	template<class T> NODISCARD INLINE constexpr Vector2Real<T> operator+(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return Vector2Real<T>{ left.X + right.X, left.Y + right.Y }; }
	template<class T> NODISCARD INLINE constexpr Vector2Real<T> operator-(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return Vector2Real<T>{ left.X - right.X, left.Y - right.Y }; }
	template<class T> INLINE Vector2Real<T>& operator+=(Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { left.X += right.X; left.Y += right.Y; return *left; }
	template<class T> INLINE Vector2Real<T>& operator-=(Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; return *left; }

	template<class T> NODISCARD INLINE constexpr Vector2Real<T> operator*(const Vector2Real<T>& left, T right)noexcept { return Vector2Real<T>{ left.X* right, left.Y* right }; }
	template<class T> NODISCARD INLINE constexpr Vector2Real<T> operator/(const Vector2Real<T>& left, T right)noexcept { float invRight = T(1) / right; return Vector2Real<T>{ left.X* invRight, left.Y* invRight }; }
	template<class T> NODISCARD INLINE constexpr Vector2Real<T> operator*(T left, const Vector2Real<T>& right)noexcept { return Vector2Real<T>{ left* right.X, left* right.Y }; }
	template<class T> INLINE Vector2Real<T>& operator*=(Vector2Real<T>& left, T right)noexcept { left.X *= right; left.Y *= right; return left; }
	template<class T> INLINE Vector2Real<T>& operator/=(Vector2Real<T>& left, T right)noexcept { float invRight = T(1) / right; left.X *= invRight; left.Y *= invRight; return left; }

	template<class T> NODISCARD INLINE constexpr bool operator==(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T> NODISCARD INLINE constexpr bool operator!=(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return !(left == right); }
}

#define INSTANTIATE_VEC2R_UTILS(type)\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Real<type> Abs<greaper::math::Vector2Real<type>>(const greaper::math::Vector2Real<type> a)noexcept{\
	return a.GetAbs();\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Real<type> Clamp<greaper::math::Vector2Real<type>>(const greaper::math::Vector2Real<type> a, const greaper::math::Vector2Real<type> min, const greaper::math::Vector2Real<type> max)noexcept{\
	return a.GetClamped(min, max);\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Real<type> ClampZeroToOne<greaper::math::Vector2Real<type>>(const greaper::math::Vector2Real<type> a)noexcept{\
	return a.GetClampledAxes(type(0), type(1));\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Real<type> ClampNegOneToOne<greaper::math::Vector2Real<type>>(const greaper::math::Vector2Real<type> a)noexcept{\
	return a.GetClampledAxes(type(-1), type(1));\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Real<type> Sign<greaper::math::Vector2Real<type>>(const greaper::math::Vector2Real<type> a)noexcept{\
	return a.GetSignVector();\
}\

INSTANTIATE_VEC2R_UTILS(float);
INSTANTIATE_VEC2R_UTILS(double);
INSTANTIATE_VEC2R_UTILS(long double);

#undef INSTANTIATE_VEC2R_UTILS

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector2Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2Real<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};
}

#endif /* MATH_VECTOR2REAL_H */