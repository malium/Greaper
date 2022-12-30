/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR4UNSIGNED_H
#define MATH_VECTOR4UNSIGNED_H 1

#include "Vector3Unsigned.inl"
#include "VecRef.h"

namespace greaper::math
{
	template<class T>
	class Vector4Unsigned
	{
		static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Vector4Unsigned can only work with float, double or long double types");

		template<class U> struct Print {  };
		template<> struct Print<uint8> { static constexpr auto fmt = "%" PRIu8 ", %" PRIu8 ", %" PRIu8 ", %" PRIu8; };
		template<> struct Print<uint16> { static constexpr auto fmt = "%" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu16; };
		template<> struct Print<uint32> { static constexpr auto fmt = "%" PRIu32 ", %" PRIu32 ", %" PRIu32 ", %" PRIu32; };
		template<> struct Print<uint64> { static constexpr auto fmt = "%" PRIu64 ", %" PRIu64 ", %" PRIu64 ", %" PRIu64; };

		template<class U> struct Scan {  };
		template<> struct Scan<uint8> { static constexpr auto fmt = "%" SCNu8 ", %" SCNu8 ", %" SCNu8 ", %" SCNu8; };
		template<> struct Scan<uint16> { static constexpr auto fmt = "%" SCNu16 ", %" SCNu16 ", %" SCNu16 ", %" SCNu16; };
		template<> struct Scan<uint32> { static constexpr auto fmt = "%" SCNu32 ", %" SCNu32 ", %" SCNu32 ", %" SCNu32; };
		template<> struct Scan<uint64> { static constexpr auto fmt = "%" SCNu64 ", %" SCNu64 ", %" SCNu64 ", %" SCNu64; };

	public:
		static constexpr sizet ComponentCount = 4;
		using value_type = T;

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
		NODISCARD INLINE constexpr bool IsEqual(const Vector4Unsigned& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0) });
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z && X == W;
		}
		NODISCARD INLINE constexpr T GetMaxComponent()const noexcept
		{
			return ::Max(::Max3(X, Y, Z), W);
		}
		NODISCARD INLINE constexpr T GetMinComponent()const noexcept
		{
			return ::Min(::Min3(X, Y, Z), W);
		}
		NODISCARD INLINE constexpr Vector4Unsigned GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector4Unsigned{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal),
				::Clamp(Z, minAxeVal, maxAxeVal),
				::Clamp(W, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector4Unsigned GetClamped(const Vector4Unsigned& min, const Vector4Unsigned& max)const noexcept
		{
			return Vector4Unsigned{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y),
				::Clamp(Z, min.Z, max.Z),
				::Clamp(W, min.W, max.W)
			};
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y, Z, W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y, &Z, &W);
		}

		static const Vector4Unsigned ZERO;
		static const Vector4Unsigned UNIT;
	};

	template<class T> const Vector4Unsigned<T> Vector4Unsigned<T>::ZERO = Vector4Unsigned<T>{};
	template<class T> const Vector4Unsigned<T> Vector4Unsigned<T>::UNIT = Vector4Unsigned<T>((T)1, (T)1, (T)1, T(1));
}

template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Unsigned<T> operator+(const greaper::math::Vector4Unsigned<T>& left, const greaper::math::Vector4Unsigned<T>& right)noexcept { return greaper::math::Vector4Unsigned<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Unsigned<T> operator-(const greaper::math::Vector4Unsigned<T>& left, const greaper::math::Vector4Unsigned<T>& right)noexcept { return greaper::math::Vector4Unsigned<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W }; }
template<class T> INLINE greaper::math::Vector4Unsigned<T>& operator+=(greaper::math::Vector4Unsigned<T>& left, const greaper::math::Vector4Unsigned<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; left.W += right.W; return left; }
template<class T> INLINE greaper::math::Vector4Unsigned<T>& operator-=(greaper::math::Vector4Unsigned<T>& left, const greaper::math::Vector4Unsigned<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; left.W -= right.W; return left; }

template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Unsigned<T> operator*(const greaper::math::Vector4Unsigned<T>& left, T right)noexcept { return greaper::math::Vector4Unsigned<T>{ left.X* right, left.Y* right, left.Z* right, left.W* right }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Unsigned<T> operator/(const greaper::math::Vector4Unsigned<T>& left, T right)noexcept { return greaper::math::Vector4Unsigned<T>{ left.X / right, left.Y* right, left.Z* right, left.W* right }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Unsigned<T> operator*(T left, const greaper::math::Vector4Unsigned<T>& right)noexcept { return greaper::math::Vector4Unsigned<T>{ left* right.X, left* right.Y, left* right.Z, left* right.W }; }
template<class T> INLINE greaper::math::Vector4Unsigned<T>& operator*=(greaper::math::Vector4Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }
template<class T> INLINE greaper::math::Vector4Unsigned<T>& operator/=(greaper::math::Vector4Unsigned<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }

template<class T> NODISCARD INLINE constexpr bool operator==(const greaper::math::Vector4Unsigned<T>& left, const greaper::math::Vector4Unsigned<T>& right)noexcept { return left.IsEqual(right); }
template<class T> NODISCARD INLINE constexpr bool operator!=(const greaper::math::Vector4Unsigned<T>& left, const greaper::math::Vector4Unsigned<T>& right)noexcept { return !(left == right); }

template<class T>
NODISCARD INLINE constexpr greaper::math::Vector4Unsigned<T> Clamp<greaper::math::Vector4Unsigned<T>>(const greaper::math::Vector4Unsigned<T> a, const greaper::math::Vector4Unsigned<T> min, const greaper::math::Vector4Unsigned<T> max)noexcept
{
	return a.GetClamped(min, max);
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector4Unsigned<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector4Unsigned<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR4UNSIGNED_H */