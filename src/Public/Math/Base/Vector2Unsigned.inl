/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR2UNSIGNED_H
#define MATH_VECTOR2UNSIGNED_H 1

#include "../MathPrerequisites.h"
#include <Core/StringUtils.h>
#include <array>

namespace greaper::math
{
	template<class T>
	class Vector2Unsigned
	{
		static_assert(std::is_integral_v<T>&& std::is_unsigned_v<T>, "Vector2Unsigned can only work with unsigned intXX types");

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
		NODISCARD INLINE constexpr bool IsEqual(const Vector2Unsigned& other)const noexcept
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
		NODISCARD INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(X, Y);
		}
		NODISCARD INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(X, Y);
		}
		NODISCARD INLINE constexpr Vector2Unsigned GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector2Unsigned{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector2Unsigned GetClamped(const Vector2Unsigned& min, const Vector2Unsigned& max)const noexcept
		{
			return Vector2Unsigned{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y)
			};
		}
		NODISCARD INLINE String ToString()const noexcept
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

	template<class T> inline const Vector2Unsigned<T> Vector2Unsigned<T>::ZERO = Vector2Unsigned<T>{};
	template<class T> inline const Vector2Unsigned<T> Vector2Unsigned<T>::UNIT = Vector2Unsigned<T>((T)1, (T)1);

	template<class T> NODISCARD INLINE constexpr Vector2Unsigned<T> operator+(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return Vector2Unsigned<T>{ left.X + right.X, left.Y + right.Y }; }
	template<class T> NODISCARD INLINE constexpr Vector2Unsigned<T> operator-(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return Vector2Unsigned<T>{ left.X - right.X, left.Y - right.Y }; }
	template<class T> INLINE Vector2Unsigned<T>& operator+=(Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { left.X += right.X; left.Y += right.Y; return left; }
	template<class T> INLINE Vector2Unsigned<T>& operator-=(Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; return left; }

	template<class T> NODISCARD INLINE constexpr Vector2Unsigned<T> operator*(const Vector2Unsigned<T>& left, T right)noexcept { return Vector2Unsigned<T>{ left.X* right, left.Y* right }; }
	template<class T> NODISCARD INLINE constexpr Vector2Unsigned<T> operator/(const Vector2Unsigned<T>& left, T right)noexcept { return Vector2Unsigned<T>{ left.X / right, left.Y / right }; }
	template<class T> NODISCARD INLINE constexpr Vector2Unsigned<T> operator*(T left, const Vector2Unsigned<T>& right)noexcept { return Vector2Unsigned<T>{ left* right.X, left* right.Y }; }
	template<class T> INLINE Vector2Unsigned<T>& operator*=(Vector2Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; return left; }
	template<class T> INLINE Vector2Unsigned<T>& operator/=(Vector2Unsigned<T>& left, T right)noexcept { left.X /= right; left.Y /= right; return left; }

	template<class T> NODISCARD INLINE constexpr bool operator==(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return left.IsEqual(right); }
	template<class T> NODISCARD INLINE constexpr bool operator!=(const Vector2Unsigned<T>& left, const Vector2Unsigned<T>& right)noexcept { return !(left == right); }
}

#define INSTANTIATE_VEC2U_UTILS(type)\
template<> NODISCARD INLINE constexpr greaper::math::Vector2Unsigned<type> Clamp<greaper::math::Vector2Unsigned<type>>(const greaper::math::Vector2Unsigned<type> a, const greaper::math::Vector2Unsigned<type> min, const greaper::math::Vector2Unsigned<type> max)noexcept{\
	return a.GetClamped(min, max);\
}

INSTANTIATE_VEC2U_UTILS(uint8);
INSTANTIATE_VEC2U_UTILS(uint16);
INSTANTIATE_VEC2U_UTILS(uint32);
INSTANTIATE_VEC2U_UTILS(uint64);

#undef INSTANTIATE_VEC2U_UTILS

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector2Unsigned<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2Unsigned<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};
}

#endif /* MATH_VECTOR2UNSIGNED_H */