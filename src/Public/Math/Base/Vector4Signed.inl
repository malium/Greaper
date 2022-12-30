/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR4SIGNED_H
#define MATH_VECTOR4SIGNED_H 1

#include "Vector3Signed.inl"
#include "VecRef.h"

namespace greaper::math
{
	template<class T>
	class Vector4Signed
	{
		static_assert(std::is_integral_v<T> && !std::is_unsigned_v<T>, "Vector4Signed can only work with float, double or long double types");

		template<class U> struct Print {  };
		template<> struct Print<int8> { static constexpr auto fmt = "%" PRIi8 ", %" PRIi8 ", %" PRIi8 ", %" PRIi8; };
		template<> struct Print<int16> { static constexpr auto fmt = "%" PRIi16 ", %" PRIi16 ", %" PRIi16 ", %" PRIi16; };
		template<> struct Print<int32> { static constexpr auto fmt = "%" PRIi32 ", %" PRIi32 ", %" PRIi32 ", %" PRIi32; };
		template<> struct Print<int64> { static constexpr auto fmt = "%" PRIi64 ", %" PRIi64 ", %" PRIi64 ", %" PRIi64; };

		template<class U> struct Scan {  };
		template<> struct Scan<int8> { static constexpr auto fmt = "%" SCNi8 ", %" SCNi8 ", %" SCNi8 ", %" SCNi8; };
		template<> struct Scan<int16> { static constexpr auto fmt = "%" SCNi16 ", %" SCNi16 ", %" SCNi16 ", %" SCNi16; };
		template<> struct Scan<int32> { static constexpr auto fmt = "%" SCNi32 ", %" SCNi32 ", %" SCNi32 ", %" SCNi32; };
		template<> struct Scan<int64> { static constexpr auto fmt = "%" SCNi64 ", %" SCNi64 ", %" SCNi64 ", %" SCNi64; };

	public:
		static constexpr sizet ComponentCount = 4;
		using value_type = T;

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
		NODISCARD INLINE constexpr bool IsEqual(const Vector4Signed& other)const noexcept
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
		NODISCARD INLINE constexpr Vector4Signed GetAbs()const noexcept
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
		NODISCARD INLINE constexpr Vector4Signed GetClampledAxes(T minAxeVal, T maxAxeVal)const noexcept
		{
			return Vector4Signed{
				::Clamp(X, minAxeVal, maxAxeVal),
				::Clamp(Y, minAxeVal, maxAxeVal),
				::Clamp(Z, minAxeVal, maxAxeVal),
				::Clamp(W, minAxeVal, maxAxeVal)
			};
		}
		NODISCARD INLINE constexpr Vector4Signed GetClamped(const Vector4Signed& min, const Vector4Signed& max)const noexcept
		{
			return Vector4Signed{
				::Clamp(X, min.X, max.X),
				::Clamp(Y, min.Y, max.Y),
				::Clamp(Z, min.Z, max.Z),
				::Clamp(W, min.W, max.W)
			};
		}
		NODISCARD INLINE constexpr Vector4Signed GetSignVector()const noexcept
		{
			return Vector4Signed{ ::Sign(X), ::Sign(Y), ::Sign(Z), ::Sign(W) };
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, X, Y, Z, W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &X, &Y, &Z, &W);
		}

		static const Vector4Signed ZERO;
		static const Vector4Signed UNIT;
	};

	template<class T> const Vector4Signed<T> Vector4Signed<T>::ZERO = Vector4Signed<T>{};
	template<class T> const Vector4Signed<T> Vector4Signed<T>::UNIT = Vector4Signed<T>((T)1, (T)1, (T)1, T(1));
}

template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> operator+(const greaper::math::Vector4Signed<T>& left, const greaper::math::Vector4Signed<T>& right)noexcept { return greaper::math::Vector4Signed<T>{ left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> operator-(const greaper::math::Vector4Signed<T>& left, const greaper::math::Vector4Signed<T>& right)noexcept { return greaper::math::Vector4Signed<T>{ left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W }; }
template<class T> INLINE greaper::math::Vector4Signed<T>& operator+=(greaper::math::Vector4Signed<T>& left, const greaper::math::Vector4Signed<T>& right)noexcept { left.X += right.X; left.Y += right.Y; left.Z += right.Z; left.W += right.W; return left; }
template<class T> INLINE greaper::math::Vector4Signed<T>& operator-=(greaper::math::Vector4Signed<T>& left, const greaper::math::Vector4Signed<T>& right)noexcept { left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; left.W -= right.W; return left; }

template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> operator*(const greaper::math::Vector4Signed<T>& left, T right)noexcept { return greaper::math::Vector4Signed<T>{ left.X * right, left.Y * right, left.Z * right, left.W * right }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> operator/(const greaper::math::Vector4Signed<T>& left, T right)noexcept { return greaper::math::Vector4Signed<T>{ left.X / right, left.Y / right, left.Z / right, left.W / right }; }
template<class T> NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> operator*(T left, const greaper::math::Vector4Signed<T>& right)noexcept { return greaper::math::Vector4Signed<T>{ left * right.X, left * right.Y, left * right.Z, left * right.W }; }
template<class T> INLINE greaper::math::Vector4Signed<T>& operator*=(greaper::math::Vector4Signed<T>& left, T right)noexcept { left.X *= right; left.Y *= right; left.Z *= right; left.W *= right; return left; }
template<class T> INLINE greaper::math::Vector4Signed<T>& operator/=(greaper::math::Vector4Signed<T>& left, T right)noexcept { left.X /= right; left.Y /= right; left.Z /= right; left.W /= right; return left; }

template<class T> NODISCARD INLINE constexpr bool operator==(const greaper::math::Vector4Signed<T>& left, const greaper::math::Vector4Signed<T>& right)noexcept { return left.X == right.X && left.IsEqual(right); }
template<class T> NODISCARD INLINE constexpr bool operator!=(const greaper::math::Vector4Signed<T>& left, const greaper::math::Vector4Signed<T>& right)noexcept { return !(left == right); }

template<class T>
NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> Abs<greaper::math::Vector4Signed<T>>(const greaper::math::Vector4Signed<T> a)noexcept
{
	return a.GetAbs();
}
template<class T>
NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> Clamp<greaper::math::Vector4Signed<T>>(const greaper::math::Vector4Signed<T> a, const greaper::math::Vector4Signed<T> min, const greaper::math::Vector4Signed<T> max)noexcept
{
	return a.GetClamped(min, max);
}
template<class T>
NODISCARD INLINE constexpr greaper::math::Vector4Signed<T> Sign<greaper::math::Vector4Signed<T>>(const greaper::math::Vector4Signed<T> a)noexcept
{
	return a.GetSignVector();
}

namespace std
{
	template<class T>
	struct hash<greaper::math::Vector4Signed<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector4Signed<T>& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR4SIGNED_H */