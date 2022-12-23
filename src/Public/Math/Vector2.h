/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR2_H
#define MATH_VECTOR2_H 1

#include "MathPrerequisites.h"
#include "../Core/StringUtils.h"
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
		template<> struct Print<long double> { static constexpr auto fmt = "%lf, %lf"; };

		template<class U> struct Scan {  };
		template<> struct Scan<float> { static constexpr auto fmt = "%f, %f"; };
		template<> struct Scan<double> { static constexpr auto fmt = "%lf, %lf"; };
		template<> struct Scan<long double> { static constexpr auto fmt = "%lf, %lf"; };

	public:
		static constexpr sizet ComponentCount = 2;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };

		constexpr Vector2Real()noexcept = default;
		INLINE constexpr Vector2Real(T x, T y)noexcept :X(x), Y(y) {  }
		INLINE constexpr explicit Vector2Real(const std::array<T, ComponentCount>& arr) :X(arr[0]), Y(arr[1]) {  }
		INLINE constexpr Vector2Real operator-()const noexcept { return { -X, -Y }; }

		INLINE T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
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
		INLINE constexpr T DotProduct(const Vector2Real& other)const noexcept
		{
			return X * other.X + Y * other.Y;
		}
		INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		INLINE constexpr T DistSquared(const Vector2Real& other)const noexcept
		{
			return Square(X - other.X) + Square(Y - other.Y);
		}
		INLINE T Distance(const Vector2Real& other)const noexcept
		{
			return Sqrt(DistSquared(other));
		}
		INLINE constexpr T CrossProduct(const Vector2Real& other)const noexcept
		{
			return X * other.Y - Y * other.X;
		}
		INLINE Vector2Real Normalized(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return Vector2Real{ X * invScale, Y * invScale };
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
			}
		}
		INLINE constexpr bool IsNearlyEqual(const Vector2Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(X, other.X, tolerance) && ::IsNearlyEqual(Y, other.Y, tolerance);
		}
		INLINE constexpr bool IsEqual(const Vector2Real& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0) }, tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0) });
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
			return X == Y;
		}
		INLINE constexpr bool AreComponentsNearlyEqual(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(X, Y, tolerance);
		}
		INLINE constexpr Vector2Real GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y) };
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(X, Y);
		}
		INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max(::Abs(X), ::Abs(Y));
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(X, Y);
		}
		INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min(::Abs(X), ::Abs(Y));
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = ::Clamp(X, minAxeVal, maxAxeVal);
			Y = ::Clamp(Y, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector2Real& min, const Vector2Real& max)noexcept
		{
			X = ::Clamp(X, min.X, max.X);
			Y = ::Clamp(Y, min.Y, max.Y);
		}
		INLINE constexpr Vector2Real GetSignVector()const noexcept
		{
			return Vector2Real{ ::Sign(X), ::Sign(Y) };
		}
		INLINE String ToString()const noexcept
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

	template<class T> INLINE constexpr Vector2Real<T> operator+(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return Vector2Real<T>{ left.X + right.X, left.Y + right.Y }; }
	template<class T> INLINE constexpr Vector2Real<T> operator-(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return Vector2Real<T>{ left.X - right.X, left.Y - right.Y }; }
	template<class T> INLINE Vector2Real<T>& operator+=(Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { left.X += right.X; left.Y += right.Y; return *left; }
	template<class T> INLINE Vector2Real<T>& operator-=(Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; return *left; }
	
	template<class T> INLINE constexpr Vector2Real<T> operator*(const Vector2Real<T>& left, T right)noexcept { return Vector2Real<T>{ left.X * right, left.Y * right }; }
	template<class T> INLINE constexpr Vector2Real<T> operator/(const Vector2Real<T>& left, T right)noexcept { float invRight = T(1) / right; return Vector2Real<T>{ left.X * invRight, left.Y * invRight }; }
	template<class T> INLINE constexpr Vector2Real<T> operator*(T left, const Vector2Real<T>& right)noexcept { return Vector2Real<T>{ left * right.X, left * right.Y }; }
	template<class T> INLINE Vector2Real<T>& operator*=(Vector2Real<T>& left, T right)noexcept { left.X *= right; left.Y *= right; return left; }
	template<class T> INLINE Vector2Real<T>& operator/=(Vector2Real<T>& left, T right)noexcept { float invRight = T(1) / right; left.X *= invRight; left.Y *= invRight; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T> INLINE constexpr bool operator!=(const Vector2Real<T>& left, const Vector2Real<T>& right)noexcept { return !(left == right); }

	template<class T>
	class Vector2Signed
	{
		static_assert(std::is_integral_v<T> && !std::is_unsigned_v<T>, "Vector2Signed can only work with signed intXX types");

		template<class U> struct Print {  };
		template<> struct Print<int8> { static constexpr auto fmt = "%" PRIi8 ", %" PRIi8; };
		template<> struct Print<int16> { static constexpr auto fmt = "%" PRIi16 ", %" PRIi16; };
		template<> struct Print<int32> { static constexpr auto fmt = "%" PRIi32 ", %" PRIi32; };
		template<> struct Print<int64> { static constexpr auto fmt = "%" PRIi64 ", %" PRIi64; };

		template<class U> struct Scan {  };
		template<> struct Scan<int8> { static constexpr auto fmt = "%" SCNi8 ", %" SCNi8; };
		template<> struct Scan<int16> { static constexpr auto fmt = "%" SCNi16 ", %" SCNi16; };
		template<> struct Scan<int32> { static constexpr auto fmt = "%" SCNi32 ", %" SCNi32; };
		template<> struct Scan<int64> { static constexpr auto fmt = "%" SCNi64 ", %" SCNi64; };

	public:
		static constexpr sizet ComponentCount = 2;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };

		constexpr Vector2Signed()noexcept = default;
		INLINE constexpr Vector2Signed(T x, T y)noexcept :X(x), Y(y) {  }
		INLINE constexpr explicit Vector2Signed(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]) {  }
		INLINE constexpr Vector2Signed operator-()const noexcept { return { -X, -Y }; }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y };
		}
		INLINE void Set(const Vector2Signed& other)noexcept
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
		INLINE constexpr bool IsEqual(const Vector2Signed& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0) });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y;
		}
		INLINE constexpr Vector2Signed GetAbs()const noexcept
		{
			return { ::Abs(X), ::Abs(Y) };
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(X, Y);
		}
		INLINE constexpr T GetAbsMaxComponent()const noexcept
		{
			return ::Max(::Abs(X), ::Abs(Y));
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(X, Y);
		}
		INLINE constexpr T GetAbsMinComponent()const noexcept
		{
			return ::Min(::Abs(X), ::Abs(Y));
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = Clamp(X, minAxeVal, maxAxeVal);
			Y = Clamp(Y, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector2Signed& min, const Vector2Signed& max)noexcept
		{
			X = Clamp(X, min.X, max.X);
			Y = Clamp(Y, min.Y, max.Y);
		}
		INLINE constexpr Vector2Signed GetSignVector()const noexcept
		{
			return { ::Sign(X), ::Sign(Y) };
		}
		INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y);
		}

		static const Vector2Signed ZERO;
		static const Vector2Signed UNIT;
	};

	template<class T> const Vector2Signed<T> Vector2Signed<T>::ZERO = Vector2Signed<T>{};
	template<class T> const Vector2Signed<T> Vector2Signed<T>::UNIT = Vector2Signed<T>((T)1, (T)1);

	template<class T> INLINE constexpr Vector2Signed<T> operator+(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return Vector2Signed<T>{ left.X + right.X, left.Y + right.Y }; }
	template<class T> INLINE constexpr Vector2Signed<T> operator-(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return Vector2Signed<T>{ left.X - right.X, left.Y - right.Y }; }
	template<class T> INLINE Vector2Signed<T>& operator+=(Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { left.X += right.X; left.Y += right.Y; return left; }
	template<class T> INLINE Vector2Signed<T>& operator-=(Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; return left; }

	template<class T> INLINE constexpr Vector2Signed<T> operator*(const Vector2Signed<T>& left, T right)noexcept { return Vector2Signed<T>{ left.X* right, left.Y* right }; }
	template<class T> INLINE constexpr Vector2Signed<T> operator/(const Vector2Signed<T>& left, T right)noexcept { return Vector2Signed<T>{ left.X / right, left.Y / right }; }
	template<class T> INLINE constexpr Vector2Signed<T> operator*(T left, const Vector2Signed<T>& right)noexcept { return Vector2Signed<T>{ left* right.X, left* right.Y }; }
	template<class T> INLINE Vector2Signed<T>& operator*=(Vector2Signed<T>& left, T right)noexcept { left.X *= right; left.Y *= right; return left; }
	template<class T> INLINE Vector2Signed<T>& operator/=(Vector2Signed<T>& left, T right)noexcept { left.X /= right; left.Y /= right; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return left.X == right.X && left.Y == right.Y; }
	template<class T> INLINE constexpr bool operator!=(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return !(left == right); }

	template<class T>
	class Vector2Unsigned
	{
		static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Vector2Unsigned can only work with unsigned intXX types");

		template<class U> struct Print {  };
		template<> struct Print<uint8> { static constexpr auto fmt = "%" PRIu8 ", %" PRIu8; };
		template<> struct Print<uint16> { static constexpr auto fmt = "%" PRIu16 ", %" PRIu16; };
		template<> struct Print<uint32> { static constexpr auto fmt = "%" PRIu32 ", %" PRIu32; };
		template<> struct Print<uint64> { static constexpr auto fmt = "%" PRIu64 ", %" PRIu64; };

		template<class U> struct Scan {  };
		template<> struct Scan<uint8> { static constexpr auto fmt = "%" SCNu8 ", %" SCNu8; };
		template<> struct Scan<uint16> { static constexpr auto fmt = "%" SCNu16 ", %" SCNu16; };
		template<> struct Scan<uint32> { static constexpr auto fmt = "%" SCNu32 ", %" SCNu32; };
		template<> struct Scan<uint64> { static constexpr auto fmt = "%" SCNu64 ", %" SCNu64; };

	public:
		static constexpr sizet ComponentCount = 2;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };

		constexpr Vector2Unsigned()noexcept = default;
		INLINE constexpr Vector2Unsigned(T x, T y)noexcept :X(x), Y(y) {  }
		INLINE constexpr explicit Vector2Unsigned(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]) {  }

		INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { X, Y };
		}
		INLINE void Set(const Vector2Unsigned& other)noexcept
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
		INLINE constexpr bool IsEqual(const Vector2Unsigned& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0) });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y;
		}
		INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(X, Y);
		}
		INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(X, Y);
		}
		INLINE void ClampAxes(T minAxeVal, T maxAxeVal)noexcept
		{
			X = Clamp(X, minAxeVal, maxAxeVal);
			Y = Clamp(Y, minAxeVal, maxAxeVal);
		}
		INLINE void Clamp(const Vector2Unsigned& min, const Vector2Unsigned& max)noexcept
		{
			X = Clamp(X, min.X, max.X);
			Y = Clamp(Y, min.Y, max.Y);
		}
		INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y);
		}

		static const Vector2Unsigned ZERO;
		static const Vector2Unsigned UNIT;
	};

	template<class T> const Vector2Unsigned<T> Vector2Unsigned<T>::ZERO = Vector2Unsigned<T>{};
	template<class T> const Vector2Unsigned<T> Vector2Unsigned<T>::UNIT = Vector2Unsigned<T>((T)1, (T)1);

	template<class T> INLINE constexpr Vector2Unsigned<T> operator+(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return Vector2Unsigned<T>{ left.X + right.X, left.Y + right.Y }; }
	template<class T> INLINE constexpr Vector2Unsigned<T> operator-(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return Vector2Unsigned<T>{ left.X - right.X, left.Y - right.Y }; }
	template<class T> INLINE Vector2Unsigned<T>& operator+=(Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { left.X += right.X; left.Y += right.Y; return left; }
	template<class T> INLINE Vector2Unsigned<T>& operator-=(Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; return left; }

	template<class T> INLINE constexpr Vector2Unsigned<T> operator*(const Vector2Unsigned<T>& left, T right)noexcept { return Vector2Unsigned<T>{ left.X* right, left.Y* right }; }
	template<class T> INLINE constexpr Vector2Unsigned<T> operator/(const Vector2Unsigned<T>& left, T right)noexcept { return Vector2Unsigned<T>{ left.X / right, left.Y / right }; }
	template<class T> INLINE constexpr Vector2Unsigned<T> operator*(T left, const Vector2Unsigned<T>& right)noexcept { return Vector2Unsigned<T>{ left* right.X, left* right.Y }; }
	template<class T> INLINE Vector2Unsigned<T>& operator*=(Vector2Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; return left; }
	template<class T> INLINE Vector2Unsigned<T>& operator/=(Vector2Unsigned<T>& left, T right)noexcept { left.X /= right; left.Y /= right; return left; }

	template<class T> INLINE constexpr bool operator==(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return left.X == right.X && left.Y == right.Y; }
	template<class T> INLINE constexpr bool operator!=(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return !(left == right); }

	class Vector2b
	{
	public:
		static constexpr sizet ComponentCount = 2;
		using value_type = bool;

		bool X{ false };
		bool Y{ false };

		constexpr Vector2b()noexcept = default;
		INLINE constexpr Vector2b(bool x, bool y)noexcept :X(x), Y(y) {  }
		INLINE constexpr explicit Vector2b(const std::array<bool, ComponentCount>& arr) : X(arr[0]), Y(arr[1]) {  }

		INLINE constexpr bool& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr const bool& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		INLINE constexpr std::array<bool, ComponentCount> ToArray()const noexcept
		{
			return { X, Y };
		}
		INLINE void Set(const Vector2b& other)noexcept
		{
			X = other.X;
			Y = other.Y;
		}
		INLINE void Set(bool x, bool y)noexcept
		{
			X = x;
			Y = y;
		}
		INLINE void SetZero()noexcept
		{
			X = false;
			Y = false;
		}
		INLINE constexpr bool IsEqual(const Vector2b& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ false, false });
		}
		INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y;
		}
		INLINE String ToString()const noexcept
		{
			auto x = X ? "true"sv : "false"sv;
			auto y = Y ? "true"sv : "false"sv;
			auto space = ", "sv;
			return String(x).append(space).append(y);
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

			return true;
		}

		static const Vector2b ZERO;
		static const Vector2b UNIT;
	};

	const Vector2b Vector2b::ZERO = Vector2b{};
	const Vector2b Vector2b::UNIT = Vector2b(true, true);

	INLINE constexpr bool operator==(const Vector2b& left, const Vector2b& right)noexcept { return left.X == right.X && left.Y == right.Y; }
	INLINE constexpr bool operator!=(const Vector2b& left, const Vector2b& right)noexcept { return !(left == right); }
}

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

	template<class T>
	struct hash<greaper::math::Vector2Signed<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2Signed<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};

	template<class T>
	struct hash<greaper::math::Vector2Unsigned<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2Unsigned<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};

	template<>
	struct hash<greaper::math::Vector2b>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2b& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};
}

#endif /* MATH_VECTOR2_H */