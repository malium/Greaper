/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR4_H
#define MATH_VECTOR4_H 1

#include "Vector3.h"

namespace greaper::math
{
	template<class T>
	class Vector4Real
	{
		static_assert(std::is_floating_point_v<T>, "Vector4Real can only work with float, double or long double types");

	public:
		static constexpr sizet ComponentCount = 4;
		using Type_t = T;

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

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC4();
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
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
		INLINE constexpr T DotProduct(const Vector4Real& other)const noexcept
		{
			return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
		}
		INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		INLINE constexpr T DistSquared(const Vector4Real& other)const noexcept
		{
			return Square(X - other.X) + Square(Y - other.Y) + Square(Z - other.Z) + Square(W - other.W);
		}
		INLINE T Distance(const Vector4Real& other)const noexcept
		{
			return Sqrt(DistSquared(other));
		}
		INLINE Vector4Real Normalized(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return Vector4Real{ X * invScale, Y * invScale, Z * invScale, W * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = (T)MATH_TOLERANCE)noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				X *= invScale;
				Y *= invScale;
				Z *= invScale;
				W *= invScale;
			}
		}
		INLINE constexpr bool IsNearlyEqual(const Vector4Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(X, other.X, tolerance) && ::IsNearlyEqual(Y, other.Y, tolerance) && ::IsNearlyEqual(Z, other.Z, tolerance) && ::IsNearlyEqual(W, other.W, tolerance);
		}
		INLINE constexpr bool IsEqual(const Vector4Real& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0), T(0) }, tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0) });
		}
		INLINE constexpr bool IsNearlyUnit(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(Length(), T(1), tolerance);
		}
		INLINE constexpr bool IsUnit()const noexcept
		{
			return Length() == T(1);
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z && X == W;
		}
		INLINE constexpr bool AreComponentsNearlyEqual(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(X, Y, tolerance) && ::IsNearlyEqual(X, Z, tolerance) && ::IsNearlyEqual(X, W, tolerance);
		}
		INLINE constexpr Vector4Real GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y), ::Abs(Z), ::Abs(W) };
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(::Max3(X, Y, Z), W);
		}
		INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max(::Max3(::Abs(X), ::Abs(Y), ::Abs(Z)), ::Abs(W));
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(::Min3(X, Y, Z), W);
		}
		INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min(::Min3(::Abs(X), ::Abs(Y), ::Abs(Z)), ::Abs(W));
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
			Z = ::Clamp(Z, minAxeVal, maxAxeVal);
			W = ::Clamp(W, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector4Real& min, const Vector4Real& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
			Z = ::Clamp(Z, min.Z, max.Z);
			W = ::Clamp(W, min.W, max.W);
		}
		INLINE constexpr Vector4Real GetSignVector()const noexcept
		{
			return Vector4Real{ ::Sign(X), ::Sign(Y), ::Sign(Z), ::Sign(W) };
		}
		INLINE String ToString()const noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				return Format("%f, %f, %f, %f", X, Y, Z, W);
			else
				return Format("%lf, %lf, %lf, %lf", X, Y, Z, W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				sscanf(str.data(), "%f, %f, %f, %f", &X, &Y, &Z, &W);
			else
				sscanf(str.data(), "%lf, %lf, %lf, %lf", &X, &Y, &Z, &W);
		}

		static const Vector4Real ZERO;
		static const Vector4Real UNIT;
	};

	template<class T> const Vector4Real<T> Vector4Real<T>::ZERO = Vector4Real<T>{};
	template<class T> const Vector4Real<T> Vector4Real<T>::UNIT = Vector4Real<T>((T)1, (T)1, (T)1, T(1));

	template<class T> INLINE constexpr Vector4Real<T> operator+(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return Vector4Real<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W }; }
	template<class T> INLINE constexpr Vector4Real<T> operator-(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return Vector4Real<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W }; }
	template<class T> INLINE Vector4Real<T>& operator+=(Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; left.W += right.W; return left; }
	template<class T> INLINE Vector4Real<T>& operator-=(Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; left.W -= right.W; return left; }

	template<class T> INLINE constexpr Vector4Real<T> operator*(const Vector4Real<T>& left, T right)noexcept { return Vector4Real<T>{ left.X * right, left.Y * right, left.Z * right, left.W * right }; }
	template<class T> INLINE constexpr Vector4Real<T> operator/(const Vector4Real<T>& left, T right)noexcept { float invRight = T(1) / right; return Vector4Real<T>{ left.X * invRight, left.Y * invRight, left.Z * invRight, left.W * invRight }; }
	template<class T> INLINE constexpr Vector4Real<T> operator*(T left, const Vector4Real<T>& right)noexcept { return Vector4Real<T>{ left * right.X, left * right.Y, left * right.Z, left * right.W }; }
	template<class T> INLINE Vector4Real<T>& operator*=(Vector4Real<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }
	template<class T> INLINE Vector4Real<T>& operator/=(Vector4Real<T>& left, T right)noexcept { float invRight = T(1) / right; left.X *= invRight; left.Y *= invRight; left.Z *= invRight; left.W *= invRight; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W; }
	template<class T> INLINE constexpr bool operator!=(const Vector4Real<T>& left, const Vector4Real<T>& right)noexcept { return !(left == right); }

	template<class T>
	class Vector4Signed
	{
		static_assert(std::is_integral_v<T> && !std::is_unsigned_v<T>, "Vector4Signed can only work with float, double or long double types");

	public:
		static constexpr sizet ComponentCount = 4;
		using Type_t = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };
		T W{ 0 };

		constexpr Vector4Signed()noexcept = default;
		INLINE constexpr Vector4Signed(T x, T y, T z, T w)noexcept :X(x), Y(y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4Signed(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]), W(arr[3]) {  }
		INLINE constexpr explicit Vector4Signed(const Vector2Signed<T>& v2, T z, T w)noexcept :X(v2.X), Y(v2.Y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4Signed(const Vector2Signed<T>& v20, const Vector2Signed<T>& v21)noexcept :X(v20.X), Y(v20.Y), Z(v21.X), W(v21.Y) {  }
		INLINE constexpr explicit Vector4Signed(const Vector3Signed<T>& v3, T w)noexcept :X(v3.X), Y(v3.Y), Z(v3.Z), W(w) {  }
		INLINE constexpr Vector4Signed operator-()const noexcept { return { -X, -Y, -Z, -W }; }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC4();
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z, W };
		}
		INLINE void Set(const Vector4Signed& other)noexcept
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
		INLINE constexpr bool IsEqual(const Vector4Signed& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0) });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z && X == W;
		}
		INLINE constexpr Vector4Signed GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y), ::Abs(Z), ::Abs(W) };
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(::Max3(X, Y, Z), W);
		}
		INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max(::Max3(::Abs(X), ::Abs(Y), ::Abs(Z)), ::Abs(W));
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(::Min3(X, Y, Z), W);
		}
		INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min(::Min3(::Abs(X), ::Abs(Y), ::Abs(Z)), ::Abs(W));
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
			Z = ::Clamp(Z, minAxeVal, maxAxeVal);
			W = ::Clamp(W, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector4Signed& min, const Vector4Signed& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
			Z = ::Clamp(Z, min.Z, max.Z);
			W = ::Clamp(W, min.W, max.W);
		}
		INLINE constexpr Vector4Signed GetSignVector()const noexcept
		{
			return Vector4Signed{ ::Sign(X), ::Sign(Y), ::Sign(Z), ::Sign(W) };
		}
		INLINE String ToString()const noexcept
		{
			return Format("%i, %i, %i, %i", X, Y, Z, W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), "%i, %i, %i, %i", &X, &Y, &Z, &W);
		}

		static const Vector4Signed ZERO;
		static const Vector4Signed UNIT;
	};

	template<class T> const Vector4Signed<T> Vector4Signed<T>::ZERO = Vector4Signed<T>{};
	template<class T> const Vector4Signed<T> Vector4Signed<T>::UNIT = Vector4Signed<T>((T)1, (T)1, (T)1, T(1));

	template<class T> INLINE constexpr Vector4Signed<T> operator+(const Vector4Signed<T>& left, const Vector4Signed<T>& right)noexcept { return Vector4Signed<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W }; }
	template<class T> INLINE constexpr Vector4Signed<T> operator-(const Vector4Signed<T>& left, const Vector4Signed<T>& right)noexcept { return Vector4Signed<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W }; }
	template<class T> INLINE Vector4Signed<T>& operator+=(Vector4Signed<T>& left, const Vector4Signed<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; left.W += right.W; return left; }
	template<class T> INLINE Vector4Signed<T>& operator-=(Vector4Signed<T>& left, const Vector4Signed<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; left.W -= right.W; return left; }

	template<class T> INLINE constexpr Vector4Signed<T> operator*(const Vector4Signed<T>& left, T right)noexcept { return Vector4Signed<T>{ left.X * right, left.Y * right, left.Z * right, left.W * right }; }
	template<class T> INLINE constexpr Vector4Signed<T> operator/(const Vector4Signed<T>& left, T right)noexcept { return Vector4Signed<T>{ left.X / right, left.Y / right, left.Z / right, left.W / right }; }
	template<class T> INLINE constexpr Vector4Signed<T> operator*(T left, const Vector4Signed<T>& right)noexcept { return Vector4Signed<T>{ left * right.X, left * right.Y, left * right.Z, left * right.W }; }
	template<class T> INLINE Vector4Signed<T>& operator*=(Vector4Signed<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }
	template<class T> INLINE Vector4Signed<T>& operator/=(Vector4Signed<T>& left, T right)noexcept { left.X /= right; left.Y /= right; left.Z /= right; left.W /= right; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector4Signed<T>& left, const Vector4Signed<T>& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W; }
	template<class T> INLINE constexpr bool operator!=(const Vector4Signed<T>& left, const Vector4Signed<T>& right)noexcept { return !(left == right); }

	template<class T>
	class Vector4Unsigned
	{
		static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Vector4Unsigned can only work with float, double or long double types");

	public:
		static constexpr sizet ComponentCount = 4;
		using Type_t = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };
		T W{ 0 };

		constexpr Vector4Unsigned()noexcept = default;
		INLINE constexpr Vector4Unsigned(T x, T y, T z, T w)noexcept :X(x), Y(y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4Unsigned(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]), W(arr[3]) {  }
		INLINE constexpr explicit Vector4Unsigned(const Vector2Unsigned<T>& v2, T z, T w)noexcept :X(v2.X), Y(v2.Y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4Unsigned(const Vector2Unsigned<T>& v20, const Vector2Unsigned<T>& v21)noexcept :X(v20.X), Y(v20.Y), Z(v21.X), W(v21.Y) {  }
		INLINE constexpr explicit Vector4Unsigned(const Vector3Unsigned<T>& v3, T w)noexcept :X(v3.X), Y(v3.Y), Z(v3.Z), W(w) {  }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC4();
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z, W };
		}
		INLINE void Set(const Vector4Unsigned& other)noexcept
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
		INLINE constexpr bool IsEqual(const Vector4Unsigned& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0) });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z && X == W;
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(::Max3(X, Y, Z), W);
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(::Min3(X, Y, Z), W);
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
			Z = ::Clamp(Z, minAxeVal, maxAxeVal);
			W = ::Clamp(W, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector4Unsigned& min, const Vector4Unsigned& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
			Z = ::Clamp(Z, min.Z, max.Z);
			W = ::Clamp(W, min.W, max.W);
		}
		INLINE String ToString()const noexcept
		{
			return Format("%u, %u, %u, %u", X, Y, Z, W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), "%u, %u, %u, %u", &X, &Y, &Z, &W);
		}

		static const Vector4Unsigned ZERO;
		static const Vector4Unsigned UNIT;
	};

	template<class T> const Vector4Unsigned<T> Vector4Unsigned<T>::ZERO = Vector4Unsigned<T>{};
	template<class T> const Vector4Unsigned<T> Vector4Unsigned<T>::UNIT = Vector4Unsigned<T>((T)1, (T)1, (T)1, T(1));

	template<class T> INLINE constexpr Vector4Unsigned<T> operator+(const Vector4Unsigned<T>& left, const Vector4Unsigned<T>& right)noexcept { return Vector4Unsigned<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W }; }
	template<class T> INLINE constexpr Vector4Unsigned<T> operator-(const Vector4Unsigned<T>& left, const Vector4Unsigned<T>& right)noexcept { return Vector4Unsigned<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W }; }
	template<class T> INLINE Vector4Unsigned<T>& operator+=(Vector4Unsigned<T>& left, const Vector4Unsigned<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; left.W += right.W; return left; }
	template<class T> INLINE Vector4Unsigned<T>& operator-=(Vector4Unsigned<T>& left, const Vector4Unsigned<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; left.W -= right.W; return left; }

	template<class T> INLINE constexpr Vector4Unsigned<T> operator*(const Vector4Unsigned<T>& left, T right)noexcept { return Vector4Unsigned<T>{ left.X* right, left.Y* right, left.Z* right, left.W* right }; }
	template<class T> INLINE constexpr Vector4Unsigned<T> operator/(const Vector4Unsigned<T>& left, T right)noexcept { return Vector4Unsigned<T>{ left.X / right, left.Y* right, left.Z* right, left.W* right }; }
	template<class T> INLINE constexpr Vector4Unsigned<T> operator*(T left, const Vector4Unsigned<T>& right)noexcept { return Vector4Unsigned<T>{ left* right.X, left* right.Y, left* right.Z, left* right.W }; }
	template<class T> INLINE Vector4Unsigned<T>& operator*=(Vector4Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }
	template<class T> INLINE Vector4Unsigned<T>& operator/=(Vector4Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector4Unsigned<T>& left, const Vector4Unsigned<T>& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W; }
	template<class T> INLINE constexpr bool operator!=(const Vector4Unsigned<T>& left, const Vector4Unsigned<T>& right)noexcept { return !(left == right); }

	class Vector4b
	{
	public:
		static constexpr sizet ComponentCount = 4;
		using Type_t = bool;

		bool X = false;
		bool Y = false;
		bool Z = false;
		bool W = false;

		constexpr Vector4b()noexcept = default;
		INLINE constexpr Vector4b(bool x, bool y, bool z, bool w)noexcept :X(x), Y(y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4b(const std::array<bool, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]), W(arr[3]) {  }
		INLINE constexpr explicit Vector4b(const Vector2b& v2, bool z, bool w)noexcept :X(v2.X), Y(v2.Y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4b(const Vector2b& v20, const Vector2b& v21)noexcept :X(v20.X), Y(v20.Y), Z(v21.X), W(v21.Y) {  }
		INLINE constexpr explicit Vector4b(const Vector3b& v3, bool w)noexcept :X(v3.X), Y(v3.Y), Z(v3.Z), W(w) {  }

		INLINE constexpr bool& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const bool& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC4();
		INLINE constexpr std::array<bool, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z, W };
		}
		INLINE void Set(const Vector4b& other)noexcept
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
		}
		INLINE void Set(bool x, bool y, bool z, bool w)noexcept
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		INLINE void SetZero()noexcept
		{
			X = false;
			Y = false;
			Z = false;
			W = false;
		}
		INLINE constexpr bool IsEqual(const Vector4b& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ false, false, false, false });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z && X == W;
		}
		INLINE String ToString()const noexcept
		{
			auto x = X ? "true"sv : "false"sv;
			auto y = Y ? "true"sv : "false"sv;
			auto z = Z ? "true"sv : "false"sv;
			auto w = W ? "true"sv : "false"sv;
			auto space = ", "sv;
			return String(x).append(space).append(y).append(space).append(z).append(space).append(w);
		}
		INLINE bool FromString(StringView str)noexcept
		{
			auto split = StringUtils::Tokenize(str, ',');
			if (split.size() != ComponentCount)
				return false;

			for (auto& r : split)
				StringUtils::ToLowerSelf(StringUtils::TrimSelf(r));

			X = split[0] == "true"sv ? true : false;
			Y = split[1] == "true"sv ? true : false;
			Z = split[2] == "true"sv ? true : false;
			W = split[3] == "true"sv ? true : false;

			return true;
		}

		static const Vector4b ZERO;
		static const Vector4b UNIT;
	};

	const Vector4b Vector4b::ZERO = Vector4b{};
	const Vector4b Vector4b::UNIT = Vector4b(true, true, true, true);

	INLINE constexpr bool operator==(const Vector4b& left, const Vector4b& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W; }
	INLINE constexpr bool operator!=(const Vector4b& left, const Vector4b& right)noexcept { return !(left == right); }

}

#endif /* MATH_VECTOR4_H */