/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H 1

#include "MathPrerequisites.h"

namespace greaper::math
{
	template<class T>
	class alignas(16) QuaternionReal
	{
		static_assert(std::is_floating_point_v<T>, "QuaternionReal can only work with float, double or long double types");

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

		// Scalar part
		T W{ 0 };

		// Imaginary part
		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		constexpr QuaternionReal()noexcept = default;
		constexpr QuaternionReal(T w, T x, T y, T z)noexcept :W(w), X(x), Y(y), Z(z) {  }
		constexpr explicit QuaternionReal(const std::array<T, ComponentCount>& arr)noexcept :W(arr[0]), X(arr[1]), Y(arr[2]), Z(arr[3]) {  }
		INLINE constexpr QuaternionReal operator-()const noexcept { return { W, -X, -Y, -Z }; }

		NODISCARD INLINE constexpr T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Quaternion, but the index %" PRIuPTR " was out of range.", index);
			return (&W)[index];
		}
		NODISCARD INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Quaternion, but the index %" PRIuPTR " was out of range.", index);
			return (&W)[index];
		}
		NODISCARD INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { W, X, Y, Z };
		}
		INLINE void Set(const QuaternionReal& other)noexcept
		{
			W = other.W;
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		INLINE void Set(T w, T x, T y, T z)noexcept
		{
			W = w;
			X = x;
			Y = y;
			Z = z;
		}
		INLINE void SetZero()noexcept
		{
			W = T(0);
			X = T(0);
			Y = T(0);
			Z = T(0);
		}
		NODISCARD INLINE static QuaternionReal<T> FromEuler(T x, T y, T z)noexcept
		{
			auto hx = x * T(0.5);
			auto hy = y * T(0.5);
			auto hz = z * T(0.5);

			auto cosx = Cos(hx);
			auto sinx = Sin(hx);
			auto cosy = Cos(hy);
			auto siny = Sin(hy);
			auto cosz = Cos(hz);
			auto sinz = Sin(hz);

			return QuaternionReal<T> {
					cosx * cosy * cosz + sinx * siny * sinz,
					sinx * cosy * cosz - cosx * siny * sinz,
					cosx * siny * cosz + sinx * cosy * sinz,
					cosx * cosy * sinz - sinx * siny * cosz
			};
		}
		NODISCARD INLINE static QuaternionReal<T> FromEuler(const Vector3Real<T>& v)noexcept
		{
			return FromEuler(v.X, v.Y, v.Z);
		}
		NODISCARD INLINE static QuaternionReal<T> FromEuler(const std::array<T, 3>& a)noexcept
		{
			return FromEuler(a[0], a[1], a[2]);
		}
		NODISCARD INLINE T GetXEuler()const noexcept
		{
			auto wx = W * X;
			auto yz = Y * Z;
			auto xx = X * X;
			auto yy = Y * Y;

			auto sinx = T(2) * (wx + yz);
			auto cosx = T(1) - T(2) * (xx + yy);
			return ATan2(sinx, cosx);
		}
		NODISCARD INLINE T GetYEuler()const noexcept
		{
			auto wy = W * Y;
			auto zx = Z * X;

			auto siny = T(2) * (wy - zx);

			T y;
			if (Abs(siny) >= T(1))
				y = std::copysign(PI<T> * T(0.5), siny);
			else
				y = ASin(siny);

			return y;
		}
		NODISCARD INLINE T GetZEuler()const noexcept
		{
			auto wz = W * Z;
			auto xy = X * Y;
			auto yy = Y * Y;
			auto zz = Z * Z;

			auto sinz = T(2) * (wz + xy);
			auto cosz = T(1) - T(2) * (yy + zz);
			return ATan2(sinz, cosz);
		}
		NODISCARD INLINE Vector3Real<T> ToEulerAngles()const noexcept
		{
			return Vector3Real<T>(GetXEuler(), GetYEuler(), GetZEuler());
		}
		NODISCARD INLINE constexpr QuaternionReal Conjugated()const noexcept
		{
			return { W, -X, -Y, -Z };
		}
		INLINE void Conjugate()noexcept
		{
			*this = Conjugated();
		}
		NODISCARD INLINE constexpr T DotProduct(const QuaternionReal<T>& other)const noexcept
		{
			return W * other.W + X * other.X + Y * other.Y + Z * other.Z;
		}
		NODISCARD INLINE constexpr QuaternionReal<T> CrossProduct(const QuaternionReal<T>& other)const noexcept
		{
			return QuaternionReal<T> {
					W * other.W - X * other.X - Y * other.Y - Z * other.Z,
					W * other.X + X * other.W + Y * other.Z - Z * other.Y,
					W * other.Y + Y * other.W + Z * other.X - X * other.Z,
					W * other.Z + Z * other.W + X * other.Y - Y * other.X
			};
		}
		NODISCARD INLINE constexpr QuaternionReal<T> GetInverse()const noexcept
		{
			return Conjugated() / DotProduct(*this);
		}
		NODISCARD INLINE constexpr T LengthSquared()const noexcept
		{
			return DotProduct(*this);
		}
		NODISCARD INLINE T Length()const noexcept
		{
			return Sqrt(LengthSquared());
		}
		NODISCARD INLINE QuaternionReal GetNormalized(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			auto len = LengthSquared();
			if (len > tolerance)
			{
				auto invScale = InvSqrt(len);
				return QuaternionReal{ W * invScale, X * invScale, Y * invScale, Z * invScale };
			}
			return *this;
		}
		INLINE void Normalize(T tolerance = MATH_TOLERANCE<T>)noexcept
		{
			*this = GetNormalized(tolerance);
		}

		NODISCARD INLINE constexpr bool IsNearlyEqual(const QuaternionReal& other, T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(W, other.W, tolerance)
				&& ::IsNearlyEqual(X, other.X, tolerance)
				&& ::IsNearlyEqual(Y, other.Y, tolerance)
				&& ::IsNearlyEqual(Z, other.Z, tolerance);
		}
		NODISCARD INLINE constexpr bool IsEqual(const QuaternionReal& other)const noexcept
		{
			return W == other.W
				&& X == other.X
				&& Y == other.Y
				&& Z == other.Z;
		}
		NODISCARD INLINE constexpr bool IsNearlyZero(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(W, T(0), tolerance)
				&& ::IsNearlyEqual(X, T(0), tolerance)
				&& ::IsNearlyEqual(Y, T(0), tolerance)
				&& ::IsNearlyEqual(Z, T(0), tolerance);
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return W == T(0)
				&& X == T(0)
				&& Y == T(0)
				&& Z == T(0);
		}
		NODISCARD INLINE constexpr bool IsNearlyUnit(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return ::IsNearlyEqual(Length(), T(1), tolerance);
		}
		NODISCARD INLINE constexpr bool IsUnit()const noexcept
		{
			return Length() == T(1);
		}
		NODISCARD INLINE constexpr bool IsReal(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return !::IsNearlyEqual(W, T(0), tolerance)
				&& ::IsNearlyEqual(X, T(0), tolerance)
				&& ::IsNearlyEqual(Y, T(0), tolerance)
				&& ::IsNearlyEqual(Z, T(0), tolerance);
		}
		NODISCARD INLINE constexpr bool IsImaginary(T tolerance = MATH_TOLERANCE<T>)const noexcept
		{
			return !IsImaginary(tolerance);
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, W, X, Y, Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Scan<T>::fmt, &W, &X, &Y, &Z);
		}

		static const QuaternionReal ZERO;
		static const QuaternionReal IDENTITY;
	};

	template<class T> const QuaternionReal<T> QuaternionReal<T>::ZERO{};
	template<class T> const QuaternionReal<T> QuaternionReal<T>::IDENTITY{ T(1), T(0), T(0), T(0) };

	template<class T> NODISCARD INLINE constexpr QuaternionReal<T> operator+(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return QuaternionReal<T>{ left.W + right.W, left.X + right.X, left.Y + right.Y, left.Z + right.Z }; }
	template<class T> NODISCARD INLINE constexpr QuaternionReal<T> operator-(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return QuaternionReal<T>{ left.W - right.W, left.X - right.X, left.Y - right.Y, left.Z - right.Z }; }
	template<class T> INLINE QuaternionReal<T>& operator+=(QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { left.W += right.W; left.X += right.X; left.Y += right.Y; left.Z += right.Z; return left; }
	template<class T> INLINE QuaternionReal<T>& operator-=(QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { left.W -= right.W; left.X -= right.X; left.Y -= right.Y; left.Z -= right.Z; return left; }

	template<class T> NODISCARD INLINE constexpr QuaternionReal<T> operator*(const QuaternionReal<T>& left, T right)noexcept { return QuaternionReal<T>{ left.W * right, left.X * right, left.Y * right, left.Z * right }; }
	template<class T> NODISCARD INLINE constexpr QuaternionReal<T> operator/(const QuaternionReal<T>& left, T right)noexcept { float invRight = T(1) / right; return QuaternionReal<T>{ left.W * invRight, left.X * invRight, left.Y * invRight, left.Z * invRight }; }
	template<class T> NODISCARD INLINE constexpr QuaternionReal<T> operator*(T left, const QuaternionReal<T>& right)noexcept { return QuaternionReal<T>{ left * right.W, left * right.X, left * right.Y, left * right.Z }; }
	template<class T> INLINE QuaternionReal<T>& operator*=(QuaternionReal<T>& left, T right)noexcept { left.W *= right; left.X *= right; left.Y *= right; left.Z *= right; return left; }
	template<class T> INLINE QuaternionReal<T>& operator/=(QuaternionReal<T>& left, T right)noexcept { float invRight = T(1) / right; left.W *= invRight; left.X *= invRight; left.Y *= invRight; left.Z *= invRight; return left; }
	template<class T> NODISCARD INLINE constexpr QuaternionReal<T> operator*(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept
	{
		return {
			
			left.W * right.W - left.X * right.X - left.Y * right.Y - left.Z * right.Z,
			left.W * right.X + left.X * right.W + left.Y * right.Z - left.Z * right.Y,
			left.W * right.Y + left.Y * right.W + left.Z * right.X - left.X * right.Z,
			left.W * right.Z + left.Z * right.W + left.X * right.Y - left.Y * right.X
			
			/*
			left.W * right.W - left.X * right.X - left.Y * right.Y - left.Z * right.Z,
			left.W * right.X + left.X * right.W + left.Y * right.Z - left.Z * right.Y,
			left.W * right.Y - left.X * right.Z + left.Y * right.W + left.Z * right.X,
			left.W * right.Z + left.X * right.Y - left.Y * right.X + left.Z * right.W
			*/
		};
	}
	template<class T> INLINE QuaternionReal<T>& operator*=(QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { left = (left * right); return left; }

	template<class T> NODISCARD INLINE constexpr bool operator==(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return left.IsNearlyEqual(right); }
	template<class T> NODISCARD INLINE constexpr bool operator!=(const QuaternionReal<T>& left, const QuaternionReal<T>& right)noexcept { return !(left == right); }
}

namespace std
{
	template<class T>
	struct hash<greaper::math::QuaternionReal<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::QuaternionReal<T>& q)const noexcept
		{
			return ComputeHash(q.W, q.X, q.Y, q.Z);
		}
	};
}

#endif /* MATH_QUATERNION_H */