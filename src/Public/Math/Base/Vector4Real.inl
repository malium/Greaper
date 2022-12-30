/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR4REAL_H
#define MATH_VECTOR4REAL_H 1

#include "Vector3Real.inl"
#include "VecRef.h"

namespace greaper::math
{
	template<class T>
	class Vector4Real
	{
		static_assert(std::is_floating_point_v<T>, "Vector4Real can only work with float, double or long double types");

		template<class U> struct Print {  };
		template<> struct Print<float> { static constexpr auto fmt = "%f, %f, %f, %f"; };
		template<> struct Print<double> { static constexpr auto fmt = "%lf, %lf, %lf, %lf"; };
		template<> struct Print<long double> { static constexpr auto fmt = "%Lf, %Lf, %Lf, %Lf"; };

		template<class U> struct Scan {  };
		template<> struct Scan<float> { static constexpr auto fmt = "%f, %f, %f, %f"; };
		template<> struct Scan<double> { static constexpr auto fmt = "%lf, %lf, %lf, %lf"; };
		template<> struct Scan<long double> { static constexpr auto fmt = "%Lf, %Lf, %Lf, %Lf"; };

	public:
		static constexpr sizet ComponentCount = 4;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };
		T W{ 0 };

		constexpr Vector4Real()noexcept = default;
		INLINE constexpr Vector4Real(T x, T y, T z, T w)noexcept :X(x), Y(y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4Real(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]), W(arr[3]) {  }
		INLINE constexpr Vector4Real(const Vector2Real<T>& v2, T z, T w)noexcept :X(v2.X), Y(v2.Y), Z(z), W(w) {  }
		INLINE constexpr Vector4Real(const Vector2Real<T>& v20, const Vector2Real<T>& v21)noexcept :X(v20.X), Y(v20.Y), Z(v21.X), W(v21.Y) {  }
		INLINE constexpr Vector4Real(const Vector3Real<T>& v3, T w)noexcept :X(v3.X), Y(v3.Y), Z(v3.Z), W(w) {  }
		INLINE constexpr Vector4Real operator-()const noexcept { return { -X, -Y, -Z, -W }; }

		NODISCARD INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC4();
		NODISCARD INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z, W };
		}
		INLINE void Set(const Vector4Real& other)noexcept
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
		}
		INLINE void Set(T x, T y, T z, T w)noexcept
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		INLINE void SetZero()noexcept
		{
			X = T(0);
			Y = T(0);
			Z = T(0);
			W = T(0);
		}
		NODISCARD INLINE constexpr T DotProduct(const Vector4Real& other)const noexcept
		{
			return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
		}
		NODISCARD INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		NODISCARD INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		NODISCARD INLINE constexpr T DistSquared(const Vector4Real& other)const noexcept
		{
			return Square(X - other.X) + Square(Y - other.Y) + Square(Z - other.Z) + Square(W - other.W);
		}
		NODISCARD INLINE T Distance(const Vector4Real& other)const noexcept
		{
			return Sqrt(DistSquared(other));
		}
		NODISCARD INLINE Vector4Real GetNormalized(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return Vector4Real{ X * invScale, Y * invScale, Z * invScale, W * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = MATH_TOLERANCE<T>)noexcept
		{
			*this = GetNormalized(tolerance);
		}
		NODISCARD INLINE constexpr bool IsNearlyEqual(const Vector4Real& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(X, other.X, tolerance) && ::IsNearlyEqual(Y, other.Y, tolerance) && ::IsNearlyEqual(Z, other.Z, tolerance) && ::IsNearlyEqual(W, other.W, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const Vector4Real& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		NODISCARD INLINE constexpr bool IsNearlyZero(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0), T(0) }, tolerance);
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0) });
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
			return X == Y && X == Z && X == W;
		}
		NODISCARD INLINE constexpr bool AreComponentsNearlyEqual(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(X, Y, tolerance) && ::IsNearlyEqual(X, Z, tolerance) && ::IsNearlyEqual(X, W, tolerance);
		}
		NODISCARD INLINE constexpr Vector4Real GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y), ::Abs(Z), ::Abs(W) };
		}
		NODISCARD INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(::Max3(X, Y, Z), W);
		}
		NODISCARD INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max(::Max3(::Abs(X), ::Abs(Y), ::Abs(Z)), ::Abs(W));
		}
		NODISCARD INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(::Min3(X, Y, Z), W);
		}
		NODISCARD INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min(::Min3(::Abs(X), ::Abs(Y), ::Abs(Z)), ::Abs(W));
		}
		NODISCARD INLINE constexpr Vector4Real GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector4Real{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal),
				::Clamp(Z, minAxeVal, maxAxeVal),
				::Clamp(W, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector4Real GetClamped(const Vector4Real& min, const Vector4Real& max)const noexcept
		{
			return Vector4Real{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y),
				::Clamp(Z, min.Z, max.Z),
				::Clamp(W, min.W, max.W)
			};
		}
		NODISCARD INLINE constexpr Vector4Real GetSignVector()const noexcept
		{
			return Vector4Real{ ::Sign(X), ::Sign(Y), ::Sign(Z), ::Sign(W) };
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y, Z, W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y, &Z, &W);
		}

		static const Vector4Real ZERO;
		static const Vector4Real UNIT;
	};

	template<class T> const Vector4Real<T> Vector4Real<T>::ZERO = Vector4Real<T>{};
	template<class T> const Vector4Real<T> Vector4Real<T>::UNIT = Vector4Real<T>((T)1, (T)1, (T)1, T(1));

	template<class T> NODISCARD INLINE constexpr Vector4Real<T> operator+(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return Vector4Real<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W }; }
	template<class T> NODISCARD INLINE constexpr Vector4Real<T> operator-(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return Vector4Real<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W }; }
	template<class T> INLINE Vector4Real<T>& operator+=(Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; left.W += right.W; return left; }
	template<class T> INLINE Vector4Real<T>& operator-=(Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; left.W -= right.W; return left; }

	template<class T> NODISCARD INLINE constexpr Vector4Real<T> operator*(const Vector4Real<T>& left, T right)noexcept { return Vector4Real<T>{ left.X * right, left.Y * right, left.Z * right, left.W * right }; }
	template<class T> NODISCARD INLINE constexpr Vector4Real<T> operator/(const Vector4Real<T>& left, T right)noexcept { float invRight = T(1) / right; return Vector4Real<T>{ left.X * invRight, left.Y * invRight, left.Z * invRight, left.W * invRight }; }
	template<class T> NODISCARD INLINE constexpr Vector4Real<T> operator*(T left, const Vector4Real<T>& right)noexcept { return Vector4Real<T>{ left * right.X, left * right.Y, left * right.Z, left * right.W }; }
	template<class T> INLINE Vector4Real<T>& operator*=(Vector4Real<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }
	template<class T> INLINE Vector4Real<T>& operator/=(Vector4Real<T>& left, T right)noexcept { float invRight = T(1) / right; left.X *= invRight; left.Y *= invRight; left.Z *= invRight; left.W *= invRight; return left; }

	template<class T> NODISCARD INLINE constexpr bool operator==(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T> NODISCARD INLINE constexpr bool operator!=(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return !(left == right); }
}

#define INSTANTIATE_VEC4R_UTILS(type)\
template<> NODISCARD INLINE constexpr greaper::math::Vector4Real<type> Abs<greaper::math::Vector4Real<type>>(const greaper::math::Vector4Real<type> a)noexcept{\
	return a.GetAbs();\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector4Real<type> Clamp<greaper::math::Vector4Real<type>>(const greaper::math::Vector4Real<type> a, const greaper::math::Vector4Real<type> min, const greaper::math::Vector4Real<type> max)noexcept{\
	return a.GetClamped(min, max);\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector4Real<type> ClampZeroToOne<greaper::math::Vector4Real<type>>(const greaper::math::Vector4Real<type> a)noexcept{\
	return a.GetClampledAxes(type(0), type(1));\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector4Real<type> ClampNegOneToOne<greaper::math::Vector4Real<type>>(const greaper::math::Vector4Real<type> a)noexcept{\
	return a.GetClampledAxes(type(-1), type(1));\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector4Real<type> Sign<greaper::math::Vector4Real<type>>(const greaper::math::Vector4Real<type> a)noexcept{\
	return a.GetSignVector();\
}

INSTANTIATE_VEC4R_UTILS(float);
INSTANTIATE_VEC4R_UTILS(double);
INSTANTIATE_VEC4R_UTILS(long double);

#undef INSTANTIATE_VEC4R_UTILS

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector4Real<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector4Real<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR4REAL_H */