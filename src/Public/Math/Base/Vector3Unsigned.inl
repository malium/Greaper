/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR3UNSIGNED_H
#define MATH_VECTOR3UNSIGNED_H 1

#include "Vector2Unsigned.inl"
#include "VecRef.h"

namespace greaper::math
{
	template<class T>
	class Vector3Unsigned
	{
		static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Vector3Unsigned can only work with unsigned intXX types");

		template<class U> struct Print {  };
		template<> struct Print<uint8> { static constexpr auto fmt = "%" PRIu8 ", %" PRIu8 ", %" PRIu8; };
		template<> struct Print<uint16> { static constexpr auto fmt = "%" PRIu16 ", %" PRIu16 ", %" PRIu16; };
		template<> struct Print<uint32> { static constexpr auto fmt = "%" PRIu32 ", %" PRIu32 ", %" PRIu32; };
		template<> struct Print<uint64> { static constexpr auto fmt = "%" PRIu64 ", %" PRIu64 ", %" PRIu64; };

		template<class U> struct Scan {  };
		template<> struct Scan<uint8> { static constexpr auto fmt = "%" SCNu8 ", %" SCNu8 ", %" SCNu8; };
		template<> struct Scan<uint16> { static constexpr auto fmt = "%" SCNu16 ", %" SCNu16 ", %" SCNu16; };
		template<> struct Scan<uint32> { static constexpr auto fmt = "%" SCNu32 ", %" SCNu32 ", %" SCNu32; };
		template<> struct Scan<uint64> { static constexpr auto fmt = "%" SCNu64 ", %" SCNu64 ", %" SCNu64; };

	public:
		static constexpr sizet ComponentCount = 3;
		using value_type = T;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr Vector3Unsigned()noexcept = default;
		INLINE constexpr Vector3Unsigned(T x, T y, T z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3Unsigned(const std::array<T, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr explicit Vector3Unsigned(const Vector2Unsigned<T>& v2, T z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }

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
		NODISCARD INLINE constexpr bool IsEqual(const Vector3Unsigned& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0) });
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z;
		}
		NODISCARD INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max3(X, Y, Z);
		}
		NODISCARD INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min3(X, Y, Z);
		}
		NODISCARD INLINE constexpr Vector3Unsigned GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector3Unsigned{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal),
				::Clamp(Z, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector3Unsigned GetClamped(const Vector3Unsigned& min, const Vector3Unsigned& max)const noexcept
		{
			return Vector3Unsigned{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y),
				::Clamp(Z, min.Z, max.Z)
			};
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y, Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y, &Z);
		}

		static const Vector3Unsigned ZERO;
		static const Vector3Unsigned UNIT;
		static const Vector3Unsigned UP;
		static const Vector3Unsigned RIGHT;
		static const Vector3Unsigned FRONT;
	};

	template<class T> inline const Vector3Unsigned<T> Vector3Unsigned<T>::ZERO = Vector3Unsigned<T>{};
	template<class T> inline const Vector3Unsigned<T> Vector3Unsigned<T>::UNIT = Vector3Unsigned<T>((T)1, (T)1, (T)1);

	template<class T> inline const Vector3Unsigned<T> Vector3Unsigned<T>::UP = Vector3Unsigned<T>((T)0, (T)1, (T)0);
	template<class T> inline const Vector3Unsigned<T> Vector3Unsigned<T>::RIGHT = Vector3Unsigned<T>((T)1, (T)0, (T)0);
	template<class T> inline const Vector3Unsigned<T> Vector3Unsigned<T>::FRONT = Vector3Unsigned<T>((T)0, (T)0, (T)1);

	template<class T> NODISCARD INLINE constexpr Vector3Unsigned<T> operator+(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return Vector3Unsigned<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
	template<class T> NODISCARD INLINE constexpr Vector3Unsigned<T> operator-(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return Vector3Unsigned<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
	template<class T> INLINE Vector3Unsigned<T>& operator+=(Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
	template<class T> INLINE Vector3Unsigned<T>& operator-=(Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

	template<class T> NODISCARD INLINE constexpr Vector3Unsigned<T> operator*(const Vector3Unsigned<T>& left, T right)noexcept { return Vector3Unsigned<T>{ left.X* right, left.Y* right, left.Z* right }; }
	template<class T> NODISCARD INLINE constexpr Vector3Unsigned<T> operator/(const Vector3Unsigned<T>& left, T right)noexcept { return Vector3Unsigned<T>{ left.X / right, left.Y / right, left.Z / right }; }
	template<class T> NODISCARD INLINE constexpr Vector3Unsigned<T> operator*(T left, const Vector3Unsigned<T>& right)noexcept { return Vector3Unsigned<T>{ left* right.X, left* right.Y, left* right.Z }; }
	template<class T> INLINE Vector3Unsigned<T>& operator*=(Vector3Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; return left; }
	template<class T> INLINE Vector3Unsigned<T>& operator/=(Vector3Unsigned<T>& left, T right)noexcept { left.X /= right; left.Y /= right; left.Z /= right; return left; }

	template<class T> NODISCARD INLINE constexpr bool operator==(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return left.IsEqual(right); }
	template<class T> NODISCARD INLINE constexpr bool operator!=(const Vector3Unsigned<T>& left, const Vector3Unsigned<T>& right)noexcept { return !(left == right); }
}

#define INSTANTIATE_VEC3U_UTILS(type)\
template<> NODISCARD INLINE constexpr greaper::math::Vector3Unsigned<type> Clamp<greaper::math::Vector3Unsigned<type>>(const greaper::math::Vector3Unsigned<type> a, const greaper::math::Vector3Unsigned<type> min, const greaper::math::Vector3Unsigned<type> max)noexcept{\
	return a.GetClamped(min, max);\
}

INSTANTIATE_VEC3U_UTILS(uint8);
INSTANTIATE_VEC3U_UTILS(uint16);
INSTANTIATE_VEC3U_UTILS(uint32);
INSTANTIATE_VEC3U_UTILS(uint64);

#undef INSTANTIATE_VEC3U_UTILS

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector3Unsigned<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector3Unsigned<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR3UNSIGNED_H */