/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR3REAL_H
#define MATH_VECTOR3REAL_H 1

#include "Vector2Real.inl"
#include "VecRef.h"

namespace greaper::math
{
	template<class T>
	class Vector3Real
	{
		static_assert(std::is_floating_point_v<T>, "Vector3Real can only work with float, double or long double types");

		template<class U> struct Print {  };
		template<> struct Print<float> { static constexpr auto fmt = "%f, %f, %f"; };
		template<> struct Print<double> { static constexpr auto fmt = "%lf, %lf, %lf"; };
		template<> struct Print<long double> { static constexpr auto fmt = "%Lf, %Lf, %Lf"; };

		template<class U> struct Scan {  };
		template<> struct Scan<float> { static constexpr auto fmt = "%f, %f, %f"; };
		template<> struct Scan<double> { static constexpr auto fmt = "%lf, %lf, %lf"; };
		template<> struct Scan<long double> { static constexpr auto fmt = "%Lf, %Lf, %Lf"; };

	public:
		static constexpr sizet ComponentCount = 3;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr Vector3Real()noexcept = default;
		INLINE constexpr Vector3Real(T x, T y, T z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3Real(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr Vector3Real(const Vector2Real<T>& v2, T z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }
		INLINE constexpr Vector3Real operator-()const noexcept { return { -X, -Y, -Z }; }

		NODISCARD INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC3();
		NODISCARD INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z };
		}
		INLINE void Set(const Vector3Real& other)noexcept
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		INLINE void Set(T x, T y, T z)noexcept
		{
			X = x;
			Y = y;
			Z = z;
		}
		INLINE void SetZero()noexcept
		{
			X = T(0);
			Y = T(0);
			Z = T(0);
		}
		NODISCARD INLINE constexpr T DotProduct(const Vector3Real& other)const noexcept
		{
			return X * other.X + Y * other.Y + Z * other.Z;
		}
		NODISCARD INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		NODISCARD INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		NODISCARD INLINE constexpr T DistSquared(const Vector3Real& other)const noexcept
		{
			return Square(X - other.X) + Square(Y - other.Y) + Square(Z - other.Z);;
		}
		NODISCARD INLINE T Distance(const Vector3Real& other)const noexcept
		{
			return Sqrt(DistSquared(other));
		}
		NODISCARD INLINE constexpr Vector3Real CrossProduct(const Vector3Real& other)const noexcept
		{
			return { Y * other.Z - Z * other.Y, X * other.Z - Z * other.X, X * other.Y - Y * other.X };
		}
		NODISCARD INLINE Vector3Real GetNormalized(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return Vector3Real{ X * invScale, Y * invScale, Z * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = MATH_TOLERANCE<T>)noexcept
		{
			*this = GetNormalized(tolerance);
		}
		NODISCARD INLINE constexpr bool IsNearlyEqual(const Vector3Real& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(X, other.X, tolerance) && ::IsNearlyEqual(Y, other.Y, tolerance) && ::IsNearlyEqual(Z, other.Z, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const Vector3Real& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		NODISCARD INLINE constexpr bool IsNearlyZero(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0) }, tolerance);
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0) });
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
			return X == Y && X == Z;
		}
		NODISCARD INLINE constexpr bool AreComponentsNearlyEqual(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(X, Y, tolerance) && ::IsNearlyEqual(X, Z, tolerance);
		}
		NODISCARD INLINE constexpr Vector3Real GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y), ::Abs(Z) };
		}
		NODISCARD INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max3(X, Y, Z);
		}
		NODISCARD INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max3(::Abs(X), ::Abs(Y), ::Abs(Z));
		}
		NODISCARD INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min3(X, Y, Z);
		}
		NODISCARD INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min3(::Abs(X), ::Abs(Y), ::Abs(Z));
		}
		NODISCARD INLINE constexpr Vector3Real GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector3Real{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal),
				::Clamp(Z, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector3Real GetClamped(const Vector3Real& min, const Vector3Real& max)const noexcept
		{
			return Vector3Real{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y),
				::Clamp(Z, min.Z, max.Z)
			};
		}
		NODISCARD INLINE constexpr Vector3Real GetSignVector()const noexcept
		{
			return Vector3Real{ ::Sign(X), ::Sign(Y), ::Sign(Z) };
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y, Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y, &Z);
		}

		static const Vector3Real ZERO;
		static const Vector3Real UNIT;
		static const Vector3Real UP;
		static const Vector3Real DOWN;
		static const Vector3Real LEFT;
		static const Vector3Real RIGHT;
		static const Vector3Real FRONT;
		static const Vector3Real BACK;
	};

	template<class T> const Vector3Real<T> Vector3Real<T>::ZERO 	= Vector3Real<T>{};
	template<class T> const Vector3Real<T> Vector3Real<T>::UNIT 	= Vector3Real<T>((T)1,  (T)1,  (T)1);
	template<class T> const Vector3Real<T> Vector3Real<T>::UP 		= Vector3Real<T>((T)0,  (T)1,  (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::DOWN 	= Vector3Real<T>((T)0,  (T)-1, (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::LEFT 	= Vector3Real<T>((T)-1, (T)0,  (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::RIGHT 	= Vector3Real<T>((T)1,  (T)0,  (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::FRONT 	= Vector3Real<T>((T)0,  (T)0,  (T)1);
	template<class T> const Vector3Real<T> Vector3Real<T>::BACK 	= Vector3Real<T>((T)0,  (T)0,  (T)-1);
}

template<class T> NODISCARD INLINE constexpr greaper::math::Vector3Real<T> operator+(const greaper::math::Vector3Real<T>& left, const greaper::math::Vector3Real<T>& right)noexcept { return greaper::math::Vector3Real<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector3Real<T> operator-(const greaper::math::Vector3Real<T>& left, const greaper::math::Vector3Real<T>& right)noexcept { return greaper::math::Vector3Real<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
template<class T> INLINE greaper::math::Vector3Real<T>& operator+=(greaper::math::Vector3Real<T>& left, const greaper::math::Vector3Real<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
template<class T> INLINE greaper::math::Vector3Real<T>& operator-=(greaper::math::Vector3Real<T>& left, const greaper::math::Vector3Real<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

template<class T> NODISCARD INLINE constexpr greaper::math::Vector3Real<T> operator*(const greaper::math::Vector3Real<T>& left, T right)noexcept { return greaper::math::Vector3Real<T>{ left.X * right, left.Y * right, left.Z * right }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector3Real<T> operator/(const greaper::math::Vector3Real<T>& left, T right)noexcept { float invRight = T(1) / right; return greaper::math::Vector3Real<T>{ left.X * invRight, left.Y * invRight, left.Z * invRight }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector3Real<T> operator*(T left, const greaper::math::Vector3Real<T>& right)noexcept { return greaper::math::Vector3Real<T>{ left * right.X, left * right.Y, left * right.Z }; }
template<class T> INLINE greaper::math::Vector3Real<T>& operator*=(greaper::math::Vector3Real<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; return left; }
template<class T> INLINE greaper::math::Vector3Real<T>& operator/=(greaper::math::Vector3Real<T>& left, T right)noexcept { float invRight = T(1) / right; left.X *= invRight; left.Y *= invRight; left.Z *= invRight; return left; }

template<class T> NODISCARD INLINE constexpr bool operator==(const greaper::math::Vector3Real<T>& left, const greaper::math::Vector3Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
template<class T> NODISCARD INLINE constexpr bool operator!=(const greaper::math::Vector3Real<T>& left, const greaper::math::Vector3Real<T>& right)noexcept { return !(left == right); }

template<class T>
NODISCARD INLINE constexpr greaper::math::Vector3Real<T> Abs<greaper::math::Vector3Real<T>>(const greaper::math::Vector3Real<T> a)noexcept
{
	return a.GetAbs();
}
template<class T>
NODISCARD INLINE constexpr greaper::math::Vector3Real<T> Clamp<greaper::math::Vector3Real<T>>(const greaper::math::Vector3Real<T> a, const greaper::math::Vector3Real<T> min, const greaper::math::Vector3Real<T> max)noexcept
{
	return a.GetClamped(min, max);
}
template<class T>
NODISCARD INLINE constexpr greaper::math::Vector3Real<T> ClampZeroToOne<greaper::math::Vector3Real<T>>(const greaper::math::Vector3Real<T> a)noexcept
{
	return a.GetClampledAxes(T(0), T(1));
}
template<class T>
NODISCARD INLINE constexpr greaper::math::Vector3Real<T> ClampNegOneToOne<greaper::math::Vector3Real<T>>(const greaper::math::Vector3Real<T> a)noexcept
{
	return a.GetClampledAxes(T(-1), T(1));
}
template<class T>
NODISCARD INLINE constexpr greaper::math::Vector3Real<T> Sign<greaper::math::Vector3Real<T>>(const greaper::math::Vector3Real<T> a)noexcept
{
	return a.GetSignVector();
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector3Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector3Real<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR3REAL_H */