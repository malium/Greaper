/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H 1

#include "Vector2.h"
#include "Base/VecRef.h"

namespace greaper::math
{
	template<class T>
	class Vector3Real
	{
		static_assert(std::is_floating_point_v<T>, "Vector3Real can only work with float, double or long double types");

	public:
		static constexpr sizet ComponentCount = 3;
		using Type_t = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr Vector3Real()noexcept = default;
		INLINE constexpr Vector3Real(T x, T y, T z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3Real(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr Vector3Real(const Vector2Real<T>& v2, T z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }
		INLINE constexpr Vector3Real operator-()const noexcept { return { -X, -Y, -Z }; }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC3();
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
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
		INLINE constexpr T DotProduct(const Vector3Real& other)const noexcept
		{
			return X * other.X + Y * other.Y + Z * other.Z;
		}
		INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		INLINE constexpr T DistSquared(const Vector3Real& other)const noexcept
		{
			return Square(X - other.X) + Square(Y - other.Y) + Square(Z - other.Z);;
		}
		INLINE T Distance(const Vector3Real& other)const noexcept
		{
			return Sqrt(DistSquared(other));
		}
		INLINE constexpr Vector3Real CrossProduct(const Vector3Real& other)const noexcept
		{
			return { Y * other.Z - Z * other.Y, X * other.Z - Z * other.X, X * other.Y - Y * other.X };
		}
		INLINE Vector3Real Normalized(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return Vector3Real{ X * invScale, Y * invScale, Z * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				X *= invScale;
				Y *= invScale;
				Z *= invScale;
			}
		}
		INLINE constexpr bool IsNearlyEqual(const Vector3Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(X, other.X, tolerance) && ::IsNearlyEqual(Y, other.Y, tolerance) && ::IsNearlyEqual(Z, other.Z, tolerance);
		}
		INLINE constexpr bool IsEqual(const Vector3Real& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0) }, tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0) });
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
			return X == Y && X == Z;
		}
		INLINE constexpr bool AreComponentsNearlyEqual(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(X, Y, tolerance) && ::IsNearlyEqual(X, Z, tolerance);
		}
		INLINE constexpr Vector3Real GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y), ::Abs(Z) };
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max3(X, Y, Z);
		}
		INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max3(::Abs(X), ::Abs(Y), ::Abs(Z));
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min3(X, Y, Z);
		}
		INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min3(::Abs(X), ::Abs(Y), ::Abs(Z));
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
			Z = ::Clamp(Z, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector3Real& min, const Vector3Real& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
			Z = ::Clamp(Z, min.Z, max.Z);
		}
		INLINE constexpr Vector3Real GetSignVector()const noexcept
		{
			return Vector3Real{ ::Sign(X), ::Sign(Y), ::Sign(Z) };
		}
		INLINE String ToString()const noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				return Format("%f, %f, %f", X, Y, Z);
			else
				return Format("%lf, %lf, %lf", X, Y, Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				sscanf(str.data(), "%f, %f, %f", &X, &Y, &Z);
			else
				sscanf(str.data(), "%lf, %lf, %lf", &X, &Y, &Z);
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

	template<class T> const Vector3Real<T> Vector3Real<T>::ZERO = Vector3Real<T>{};
	template<class T> const Vector3Real<T> Vector3Real<T>::UNIT = Vector3Real<T>((T)1, (T)1, (T)1);

	template<class T> const Vector3Real<T> Vector3Real<T>::UP = Vector3Real<T>((T)0, (T)1, (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::DOWN = Vector3Real<T>((T)0, (T)-1, (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::LEFT = Vector3Real<T>((T)-1, (T)0, (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::RIGHT = Vector3Real<T>((T)1, (T)0, (T)0);
	template<class T> const Vector3Real<T> Vector3Real<T>::FRONT = Vector3Real<T>((T)0, (T)0, (T)1);
	template<class T> const Vector3Real<T> Vector3Real<T>::BACK = Vector3Real<T>((T)0, (T)0, (T)-1);

	template<class T> INLINE constexpr Vector3Real<T> operator+(const Vector3Real<T>& left, const Vector3Real<T>& right)noexcept { return Vector3Real<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
	template<class T> INLINE constexpr Vector3Real<T> operator-(const Vector3Real<T>& left, const Vector3Real<T>& right)noexcept { return Vector3Real<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
	template<class T> INLINE Vector3Real<T>& operator+=(Vector3Real<T>& left, const Vector3Real<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
	template<class T> INLINE Vector3Real<T>& operator-=(Vector3Real<T>& left, const Vector3Real<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

	template<class T> INLINE constexpr Vector3Real<T> operator*(const Vector3Real<T>& left, T right)noexcept { return Vector3Real<T>{ left.X * right, left.Y * right, left.Z * right }; }
	template<class T> INLINE constexpr Vector3Real<T> operator/(const Vector3Real<T>& left, T right)noexcept { float invRight = T(1) / right; return Vector3Real<T>{ left.X * invRight, left.Y * invRight, left.Z * invRight }; }
	template<class T> INLINE constexpr Vector3Real<T> operator*(T left, const Vector3Real<T>& right)noexcept { return Vector3Real<T>{ left * right.X, left * right.Y, left * right.Z }; }
	template<class T> INLINE Vector3Real<T>& operator*=(Vector3Real<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; return left; }
	template<class T> INLINE Vector3Real<T>& operator/=(Vector3Real<T>& left, T right)noexcept { float invRight = T(1) / right; left.X *= invRight; left.Y *= invRight; left.Z *= invRight; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector3Real<T>& left, const Vector3Real<T>& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z; }
	template<class T> INLINE constexpr bool operator!=(const Vector3Real<T>& left, const Vector3Real<T>& right)noexcept { return !(left == right); }

	template<class T>
	class Vector3Signed
	{
		static_assert(std::is_integral_v<T> && !std::is_unsigned_v<T>, "Vector3Signed can only work with signed intXX types");

	public:
		static constexpr sizet ComponentCount = 3;
		using Type_t = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr Vector3Signed()noexcept = default;
		INLINE constexpr Vector3Signed(T x, T y, T z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3Signed(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr explicit Vector3Signed(const Vector2Signed<T>& v2, T z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }
		INLINE constexpr Vector3Signed operator-()const noexcept { return { -X, -Y, -Z }; }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC3();
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z };
		}
		INLINE void Set(const Vector3Signed& other)noexcept
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
		INLINE constexpr bool IsEqual(const Vector3Signed& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0) });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z;
		}
		INLINE constexpr Vector3Signed GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y), ::Abs(Z) };
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max3(X, Y, Z);
		}
		INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max3(::Abs(X), ::Abs(Y), ::Abs(Z));
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min3(X, Y, Z);
		}
		INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min3(::Abs(X), ::Abs(Y), ::Abs(Z));
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
			Z = ::Clamp(Z, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector3Signed& min, const Vector3Signed& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
			Z = ::Clamp(Z, min.Z, max.Z);
		}
		INLINE constexpr Vector3Signed GetSignVector()const noexcept
		{
			return Vector3Signed{ ::Sign(X), ::Sign(Y), ::Sign(Z) };
		}
		INLINE String ToString()const noexcept
		{
			return Format("%i, %i, %i", X, Y, Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), "%i, %i, %i", &X, &Y, &Z);
		}

		static const Vector3Signed ZERO;
		static const Vector3Signed UNIT;
		static const Vector3Signed UP;
		static const Vector3Signed DOWN;
		static const Vector3Signed LEFT;
		static const Vector3Signed RIGHT;
		static const Vector3Signed FRONT;
		static const Vector3Signed BACK;
	};

	template<class T> const Vector3Signed<T> Vector3Signed<T>::ZERO = Vector3Signed<T>{};
	template<class T> const Vector3Signed<T> Vector3Signed<T>::UNIT = Vector3Signed<T>((T)1, (T)1, (T)1);

	template<class T> const Vector3Signed<T> Vector3Signed<T>::UP = Vector3Signed<T>((T)0, (T)1, (T)0);
	template<class T> const Vector3Signed<T> Vector3Signed<T>::DOWN = Vector3Signed<T>((T)0, (T)-1, (T)0);
	template<class T> const Vector3Signed<T> Vector3Signed<T>::LEFT = Vector3Signed<T>((T)-1, (T)0, (T)0);
	template<class T> const Vector3Signed<T> Vector3Signed<T>::RIGHT = Vector3Signed<T>((T)1, (T)0, (T)0);
	template<class T> const Vector3Signed<T> Vector3Signed<T>::FRONT = Vector3Signed<T>((T)0, (T)0, (T)1);
	template<class T> const Vector3Signed<T> Vector3Signed<T>::BACK = Vector3Signed<T>((T)0, (T)0, (T)-1);

	template<class T> INLINE constexpr Vector3Signed<T> operator+(const Vector3Signed<T>& left, const Vector3Signed<T>& right)noexcept { return Vector3Signed<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
	template<class T> INLINE constexpr Vector3Signed<T> operator-(const Vector3Signed<T>& left, const Vector3Signed<T>& right)noexcept { return Vector3Signed<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
	template<class T> INLINE Vector3Signed<T>& operator+=(Vector3Signed<T>& left, const Vector3Signed<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
	template<class T> INLINE Vector3Signed<T>& operator-=(Vector3Signed<T>& left, const Vector3Signed<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

	template<class T> INLINE constexpr Vector3Signed<T> operator*(const Vector3Signed<T>& left, T right)noexcept { return Vector3Signed<T>{ left.X* right, left.Y* right, left.Z* right }; }
	template<class T> INLINE constexpr Vector3Signed<T> operator/(const Vector3Signed<T>& left, T right)noexcept { return Vector3Signed<T>{ left.X / right, left.Y / right, left.Z / right }; }
	template<class T> INLINE constexpr Vector3Signed<T> operator*(T left, const Vector3Signed<T>& right)noexcept { return Vector3Signed<T>{ left* right.X, left* right.Y, left* right.Z }; }
	template<class T> INLINE Vector3Signed<T>& operator*=(Vector3Signed<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; return left; }
	template<class T> INLINE Vector3Signed<T>& operator/=(Vector3Signed<T>& left, T right)noexcept { left.X /= right; left.Y /= right; left.Z /= right; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector3Signed<T>& left, const Vector3Signed<T>& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z; }
	template<class T> INLINE constexpr bool operator!=(const Vector3Signed<T>& left, const Vector3Signed<T>& right)noexcept { return !(left == right); }

	template<class T>
	class Vector3Unsigned
	{
		static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Vector3Unsigned can only work with unsigned intXX types");

	public:
		static constexpr sizet ComponentCount = 3;
		using Type_t = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr Vector3Unsigned()noexcept = default;
		INLINE constexpr Vector3Unsigned(T x, T y, T z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3Unsigned(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr explicit Vector3Unsigned(const Vector2Unsigned<T>& v2, T z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC3();
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z };
		}
		INLINE void Set(const Vector3Unsigned& other)noexcept
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
		INLINE constexpr bool IsEqual(const Vector3Unsigned& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0) });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z;
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max3(X, Y, Z);
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min3(X, Y, Z);
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
			Z = ::Clamp(Z, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector3Unsigned& min, const Vector3Unsigned& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
			Z = ::Clamp(Z, min.Z, max.Z);
		}
		INLINE String ToString()const noexcept
		{
			return Format("%u, %u, %u", X, Y, Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), "%u, %u, %u", &X, &Y, &Z);
		}

		static const Vector3Unsigned ZERO;
		static const Vector3Unsigned UNIT;
		static const Vector3Unsigned UP;
		static const Vector3Unsigned RIGHT;
		static const Vector3Unsigned FRONT;
	};

	template<class T> const Vector3Unsigned<T> Vector3Unsigned<T>::ZERO = Vector3Unsigned<T>{};
	template<class T> const Vector3Unsigned<T> Vector3Unsigned<T>::UNIT = Vector3Unsigned<T>((T)1, (T)1, (T)1);

	template<class T> const Vector3Unsigned<T> Vector3Unsigned<T>::UP = Vector3Unsigned<T>((T)0, (T)1, (T)0);
	template<class T> const Vector3Unsigned<T> Vector3Unsigned<T>::RIGHT = Vector3Unsigned<T>((T)1, (T)0, (T)0);
	template<class T> const Vector3Unsigned<T> Vector3Unsigned<T>::FRONT = Vector3Unsigned<T>((T)0, (T)0, (T)1);

	template<class T> INLINE constexpr Vector3Unsigned<T> operator+(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return Vector3Unsigned<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
	template<class T> INLINE constexpr Vector3Unsigned<T> operator-(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return Vector3Unsigned<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
	template<class T> INLINE Vector3Unsigned<T>& operator+=(Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
	template<class T> INLINE Vector3Unsigned<T>& operator-=(Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

	template<class T> INLINE constexpr Vector3Unsigned<T> operator*(const Vector3Unsigned<T>& left, T right)noexcept { return Vector3Unsigned<T>{ left.X* right, left.Y* right, left.Z* right }; }
	template<class T> INLINE constexpr Vector3Unsigned<T> operator/(const Vector3Unsigned<T>& left, T right)noexcept { return Vector3Unsigned<T>{ left.X / right, left.Y / right, left.Z / right }; }
	template<class T> INLINE constexpr Vector3Unsigned<T> operator*(T left, const Vector3Unsigned<T>& right)noexcept { return Vector3Unsigned<T>{ left* right.X, left* right.Y, left* right.Z }; }
	template<class T> INLINE Vector3Unsigned<T>& operator*=(Vector3Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; return left; }
	template<class T> INLINE Vector3Unsigned<T>& operator/=(Vector3Unsigned<T>& left, T right)noexcept { left.X /= right; left.Y /= right; left.Z /= right; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z; }
	template<class T> INLINE constexpr bool operator!=(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return !(left == right); }

	class Vector3b
	{
	public:
		static constexpr sizet ComponentCount = 3;
		using Type_t = bool;
		
		bool X = false;
		bool Y = false;
		bool Z = false;

		constexpr Vector3b()noexcept = default;
		INLINE constexpr Vector3b(bool x, bool y, bool z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3b(const std::array<bool, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr explicit Vector3b(const Vector2b& v2, bool z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }

		INLINE constexpr bool& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const bool& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %lld was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC3();
		INLINE constexpr std::array<bool, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z };
		}
		INLINE void Set(const Vector3b& other)noexcept
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		INLINE void Set(bool x, bool y, bool z)noexcept
		{
			X = x;
			Y = y;
			Z = z;
		}
		INLINE void SetZero()noexcept
		{
			X = false;
			Y = false;
			Z = false;
		}
		INLINE constexpr bool IsEqual(const Vector3b& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ false, false, false });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z;
		}
		INLINE String ToString()const noexcept
		{
			auto x = X ? "true"sv : "false"sv;
			auto y = Y ? "true"sv : "false"sv;
			auto z = Z ? "true"sv : "false"sv;
			auto space = ", "sv;
			return String(x).append(space).append(y).append(space).append(z);
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
			
			return true;
		}
		
		static const Vector3b ZERO;
		static const Vector3b UNIT;
	};

	const Vector3b Vector3b::ZERO = Vector3b{ };
	const Vector3b Vector3b::UNIT = Vector3b(true, true, true);

	INLINE constexpr bool operator==(const Vector3b& left, const Vector3b& right)noexcept { return left.X == right.X && left.Y == right.Y && left.Z == right.Z; }
	INLINE constexpr bool operator!=(const Vector3b& left, const Vector3b& right)noexcept { return !(left == right); }
}

#endif /* MATH_VECTOR3_H */