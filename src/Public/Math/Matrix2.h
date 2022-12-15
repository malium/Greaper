/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_MATRIX2_H
#define MATH_MATRIX2_H 1

#include "Vector2.h"
#include "Base/VecRef.h"

namespace greaper::math
{
	template<class T>
	class Matrix2Real
	{
		static_assert(std::is_floating_point_v<T>, "Matrix2Real can only work with float, double or long double types");
	public:
		using Type_t = T;

		static constexpr sizet RowCount = 2;
		static constexpr sizet ColumnCount = 2;
		static constexpr sizet ComponentCount = RowCount * ColumnCount;

		Vector2Real<T> R0, R1;

		constexpr Matrix2Real()noexcept = default;
		INLINE constexpr Matrix2Real(T r00, T r01, T r10, T r11)noexcept :R0(r00, r01), R1(r10, r11) {  }
		INLINE constexpr explicit Matrix2Real(const std::array<T, ComponentCount>& arr)noexcept :R0(arr[0], arr[1]), R1(arr[2], arr[3]) {  }
		INLINE constexpr Matrix2Real(const Vector2Real<T>& r0, const Vector2Real<T>& r1)noexcept :R0(r0), R1(r1) {  }
		
		INLINE T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Matrix2, but the index %" PRIuPTR " was out of range.", index);
			return ((float*)&R0)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Matrix2, but the index %" PRIuPTR " was out of range.", index);
			return ((const float*)&R0)[index];
		}

		INLINE Impl::Vector2XYRef<T> C0()noexcept
		{
			return Impl::Vector2XYRef<T>{ (T&)R0.X, (T&)R1.X };
		}
		INLINE Impl::Vector2XYCRef<T> C0()const noexcept
		{
			return Impl::Vector2XYCRef<T>{ R0.X, R1.X };
		}
		INLINE Impl::Vector2XYRef<T> C1()noexcept
		{
			return Impl::Vector2XYRef<T>{ (T&)R0.Y, (T&)R1.Y };
		}
		INLINE Impl::Vector2XYCRef<T> C1()const noexcept
		{
			return Impl::Vector2XYCRef<T>{ R0.Y, R1.Y };
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { R0.X, R0.Y, R1.X, R1.Y };
		}
		INLINE void Set(const Matrix2Real& other)noexcept
		{
			R0 = other.R0;
			R1 = other.R1;
		}
		INLINE void Set(T r00, T r01, T r10, T r11)noexcept
		{
			R0.Set(r00, r01);
			R1.Set(r10, r11);
		}
		INLINE void Set(const Vector2Real<T>& r0, const Vector2Real<T>& r1)noexcept
		{
			R0 = r0;
			R1 = r1;
		}
		INLINE void SetZero()noexcept
		{
			R0.Zero();
			R1.Zero();
		}
		INLINE void SetIdentity()noexcept
		{
			R0.Set(T(1), T(0));
			R1.Set(T(0), T(1));
		}
		INLINE constexpr T Determinant()const noexcept
		{
			return R0.X * R1.Y - R0.Y * R1.X;
		}
		INLINE constexpr Matrix2Real Transposed()const noexcept
		{
			return { R0.x, R1.X,
					 R0.Y, R1.Y };
		}
		INLINE void Transpose()noexcept
		{
			*this = Transposed();
		}
		INLINE constexpr Matrix2Real Inverted()const noexcept
		{
			auto determinant = Determinant();
			if (::IsNearlyEqual(determinant, T(0), (T)MATH_TOLERANCE))
				return *this; // No inverse

			auto invDeterminant = T(1) / determinant;
			return { invDeterminant * R1.Y, -invDeterminant * R0.Y,
					 -invDeterminant * R1.X, invDeterminant * R0.X };
		}
		INLINE void Inverse()noexcept
		{
			*this = Inverted();
		}
		INLINE constexpr float Trace()const noexcept
		{
			return R0.X + R1.Y;
		}
		INLINE constexpr bool IsNearlyEqual(const Matrix2Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return R0.IsNearlyEqual(other.R0, tolerance) && R1.IsNearlyEqual(other.R1, tolerance);
		}
		INLINE constexpr bool IsEqual(const Matrix2Real& other)const noexcept
		{
			return R0.IsEqual(other.R0) && R1.IsEqual(other.R1);
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0), T(0) }, tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0) });
		}
		INLINE constexpr bool IsNearlyIdentity(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(1), T(0), T(0), T(1) }, tolerance);
		}
		INLINE constexpr bool IsIdentity()const noexcept
		{
			return IsEqual({ T(1), T(0), T(0), T(1) });
		}
		INLINE constexpr bool IsNearlySymmetric(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(R0.Y, R1.X, tolerance);
		}
		INLINE constexpr bool IsSymmetric()const noexcept
		{
			return R0.Y == R1.X;
		}
		INLINE constexpr bool IsNearlyDiagonal(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(R0.Y, T(0), tolerance) && ::IsNearlyEqual(R1.X, T(0), tolerance);
		}
		INLINE constexpr bool IsDiagonal()const noexcept
		{
			return R0.Y == T(0) && R1.X == T(0);
		}
		INLINE String ToString()const noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				return Format("%f, %f, %f, %f", R0.X, R0.Y, R1.X, R1.Y);
			else
				return Format("%lf, %lf, %lf, %lf", R0.X, R0.Y, R1.X, R1.Y);
		}
		INLINE void FromString(StringView str)noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				sscanf(str.data(), "%f, %f, %f, %f", &R0.X, &R0.Y, &R1.X, &R1.Y);
			else
				sscanf(str.data(), "%lf, %lf, %lf, %lf", &R0.X, &R0.Y, &R1.X, &R1.Y);
		}

		static const Matrix2Real IDENTITY;
		static const Matrix2Real ZERO;
	};

	template<class T> const Matrix2Real<T> Matrix2Real<T>::IDENTITY = { T(1), T(0),
																		T(0), T(1) };
	template<class T> const Matrix2Real<T> Matrix2Real<T>::ZERO = {  };

	template<class T> INLINE constexpr Matrix2Real<T> operator+(const Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { return { left.R0 + right.R0, left.R1 + right.R1 }; }
	template<class T> INLINE constexpr Matrix2Real<T> operator-(const Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { return { left.R0 - right.R0, left.R1 - right.R1 }; }
	template<class T> INLINE constexpr Matrix2Real<T> operator*(const Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept
	{
		return { 
			left.R0.X * right.R0.X + left.R0.Y * right.R1.X,	left.R0.X * right.R0.Y + left.R0.Y * right.R1.Y,
			left.R1.X * right.R0.X + left.R1.Y * right.R1.X,	left.R1.X * right.R0.Y + left.R1.Y * right.R1.Y };
	}
	template<class T> INLINE Matrix2Real<T>& operator*=(Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { left = (left * right); return left; }
	template<class T> INLINE constexpr Vector2Real<T> operator*(const Vector2Real<T>& left, const Matrix2Real<T>& right)noexcept
	{
		return {
			left.X * right.R0.X + left.Y * right.R0.Y,
			left.X * right.R1.X + left.Y * right.R1.Y
		};
	}
	template<class T> INLINE Vector2Real<T>& operator*=(Vector2Real<T>& left, const Matrix2Real<T>& right)noexcept { left = (left * right); return left; }
	template<class T> INLINE constexpr Vector2Real<T> operator*(const Matrix2Real<T>& left, const Vector2Real<T>& right)noexcept
	{
		return {
			left.R0.X * right.X + left.R0.Y * right.Y,
			left.R1.X * right.X + left.R1.Y * right.Y
		};
	}

	template<class T> INLINE Matrix2Real<T> operator+=(Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { left.R0 += right.R0; left.R1 += right.R1; return left; }
	template<class T> INLINE Matrix2Real<T> operator-=(Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { left.R0 -= right.R0; left.R1 -= right.R1; return left; }
	template<class T> INLINE Matrix2Real<T> operator*=(Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { left = (left * right); return left; }
	
	template<class T> INLINE constexpr Matrix2Real<T> operator*(const Matrix2Real<T>& left, T right)noexcept { return { left.R0 * right, left.R1 * right }; }
	template<class T> INLINE Matrix2Real<T> operator*=(Matrix2Real<T>& left, T right)noexcept { left.R0 *= right; left.R1 *= right; return left; }
	template<class T> INLINE constexpr Matrix2Real<T> operator*(T left, const Matrix2Real<T>& right)noexcept { return { right.R0 * left, right.R1 * left }; }

	template<class T> INLINE constexpr bool operator==(const Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { return left.IsEqual(right); }
	template<class T> INLINE constexpr bool operator!=(const Matrix2Real<T>& left, const Matrix2Real<T>& right)noexcept { return !(left == right); }
}

#endif /* MATH_MATRIX2_H */