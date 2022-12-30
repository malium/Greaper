/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR2SIGNED_H
#define MATH_VECTOR2SIGNED_H 1

#include "../MathPrerequisites.h"
#include <Core/StringUtils.h>
#include <array>

namespace greaper::math
{
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

		NODISCARD INLINE constexpr T& operator[](sizet index)noexcept
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
		NODISCARD INLINE constexpr bool IsEqual(const Vector2Signed& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0) });
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y;
		}
		NODISCARD INLINE constexpr Vector2Signed GetAbs()const noexcept
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
		NODISCARD INLINE constexpr Vector2Signed GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector2Signed{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector2Signed GetClamped(const Vector2Signed& min, const Vector2Signed& max)const noexcept
		{
			return Vector2Signed{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y)
			};
		}
		NODISCARD INLINE constexpr Vector2Signed GetSignVector()const noexcept
		{
			return { ::Sign(X), ::Sign(Y) };
		}
		NODISCARD INLINE String ToString()const noexcept
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

	template<class T> NODISCARD INLINE constexpr Vector2Signed<T> operator+(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return Vector2Signed<T>{ left.X + right.X, left.Y + right.Y }; }
	template<class T> NODISCARD INLINE constexpr Vector2Signed<T> operator-(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return Vector2Signed<T>{ left.X - right.X, left.Y - right.Y }; }
	template<class T> INLINE Vector2Signed<T>& operator+=(Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { left.X += right.X; left.Y += right.Y; return left; }
	template<class T> INLINE Vector2Signed<T>& operator-=(Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; return left; }

	template<class T> NODISCARD INLINE constexpr Vector2Signed<T> operator*(const Vector2Signed<T>& left, T right)noexcept { return Vector2Signed<T>{ left.X* right, left.Y* right }; }
	template<class T> NODISCARD INLINE constexpr Vector2Signed<T> operator/(const Vector2Signed<T>& left, T right)noexcept { return Vector2Signed<T>{ left.X / right, left.Y / right }; }
	template<class T> NODISCARD INLINE constexpr Vector2Signed<T> operator*(T left, const Vector2Signed<T>& right)noexcept { return Vector2Signed<T>{ left* right.X, left* right.Y }; }
	template<class T> INLINE Vector2Signed<T>& operator*=(Vector2Signed<T>& left, T right)noexcept { left.X *= right; left.Y *= right; return left; }
	template<class T> INLINE Vector2Signed<T>& operator/=(Vector2Signed<T>& left, T right)noexcept { left.X /= right; left.Y /= right; return left; }

	template<class T> NODISCARD INLINE constexpr bool operator==(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return left.IsEqual(right); }
	template<class T> NODISCARD INLINE constexpr bool operator!=(const Vector2Signed<T>& left, const Vector2Signed<T>& right)noexcept { return !(left == right); }
}

#define INSTANTIATE_VEC2S_UTILS(type)\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Signed<type> Abs<greaper::math::Vector2Signed<type>>(const greaper::math::Vector2Signed<type> a)noexcept{\
	return a.GetAbs();\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Signed<type> Clamp<greaper::math::Vector2Signed<type>>(const greaper::math::Vector2Signed<type> a, const greaper::math::Vector2Signed<type> min, const greaper::math::Vector2Signed<type> max)noexcept{\
	return a.GetClamped(min, max);\
}\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Signed<type> Sign<greaper::math::Vector2Signed<type>>(const greaper::math::Vector2Signed<type> a)noexcept{\
	return a.GetSignVector();\
}

INSTANTIATE_VEC2S_UTILS(int8);
INSTANTIATE_VEC2S_UTILS(int16);
INSTANTIATE_VEC2S_UTILS(int32);
INSTANTIATE_VEC2S_UTILS(int64);

#undef INSTANTIATE_VEC2S_UTILS

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector2Signed<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2Signed<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};
}

#endif /* MATH_VECTOR2SIGNED_H */