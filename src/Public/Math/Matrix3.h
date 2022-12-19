/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_MATRIX3_H
#define MATH_MATRIX3_H 1

#include "Vector3.h"
#include "Matrix2.h"

namespace greaper::math
{
	template<class T>
	class Matrix3Real
	{
		static_assert(std::is_floating_point_v<T>, "Matrix3Real can only work with float, double or long double types");

		template<class U> struct Print {  };
		template<> struct Print<float> { static constexpr auto fmt = "%f, %f, %f, %f, %f, %f, %f, %f, %f"; };
		template<> struct Print<double> { static constexpr auto fmt = "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf"; };
		template<> struct Print<long double> { static constexpr auto fmt = "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf"; };

		template<class U> struct Scan {  };
		template<> struct Scan<float> { static constexpr auto fmt = "%f, %f, %f, %f, %f, %f, %f, %f, %f"; };
		template<> struct Scan<double> { static constexpr auto fmt = "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf"; };
		template<> struct Scan<long double> { static constexpr auto fmt = "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf"; };

	public:
		using Type_t = T;

		static constexpr sizet RowCount = 3;
		static constexpr sizet ColumnCount = 3;
		static constexpr sizet ComponentCount = RowCount * ColumnCount;

		Vector3Real<T> R0, R1, R2;

		constexpr Matrix3Real()noexcept = default;
		INLINE constexpr Matrix3Real(T r00, T r01, T r02, T r10, T r11, T r12, T r20, T r21, T r22)noexcept :R0(r00, r01, r02), R1(r10, r11, r12), R2(r20, r21, r22) {  }
		INLINE constexpr explicit Matrix3Real(const std::array<T, ComponentCount>& arr)noexcept : R0(arr[0], arr[1], arr[2]), R1(arr[3], arr[4], arr[5]), R2(arr[6], arr[7], arr[8]) {  }
		INLINE constexpr Matrix3Real(const Vector3Real<T>& r0, const Vector3Real<T>& r1, const Vector3Real<T>& r2)noexcept :R0(r0), R1(r1), R2(r2) {  }
		INLINE constexpr explicit Matrix3Real(const Matrix2Real<T>& m2)noexcept :R0(m2.R0, T(0)), R1(m2.R1, T(0)), R2(T(0), T(0), T(1)) {  }
			
		INLINE T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Matrix3, but the index %" PRIuPTR " was out of range.", index);
			return ((float*)&R0)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Matrix3, but the index %" PRIuPTR " was out of range.", index);
			return ((const float*)&R0)[index];
		}

		INLINE Impl::Vector3XYZRef<T> C0()noexcept
		{
			return Impl::Vector3XYZRef<T>{ (T&)R0.X, (T&)R1.X, (T&)R2.X };
		}
		INLINE Impl::Vector3XYZCRef<T> C0()const noexcept
		{
			return Impl::Vector3XYZCRef<T>{ R0.X, R1.X, R2.X };
		}
		INLINE Impl::Vector3XYZRef<T> C1()noexcept
		{
			return Impl::Vector3XYZRef<T>{ (T&)R0.Y, (T&)R1.Y, (T&)R2.Y };
		}
		INLINE Impl::Vector3XYZCRef<T> C1()const noexcept
		{
			return Impl::Vector3XYZCRef<T>{ R0.Y, R1.Y, R2.Y };
		}
		INLINE Impl::Vector3XYZRef<T> C2()noexcept
		{
			return Impl::Vector3XYZRef<T>{ (T&)R0.Z, (T&)R1.Z, (T&)R2.Z };
		}
		INLINE Impl::Vector3XYZCRef<T> C2()const noexcept
		{
			return Impl::Vector3XYZCRef<T>{ R0.Z, R1.Z, R2.Z };
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { R0.X, R0.Y, R0.Z, R1.X, R1.Y, R1.Z, R2.X, R2.Y, R2.Z };
		}
		INLINE void Set(const Matrix3Real& other)noexcept
		{
			R0 = other.R0;
			R1 = other.R1;
			R2 = other.R2;
		}
		INLINE void Set(T r00, T r01, T r02, T r10, T r11, T r12, T r20, T r21, T r22)noexcept
		{
			R0.Set(r00, r01, r02);
			R1.Set(r10, r11, r12);
			R2.Set(r20, r21, r22);
		}
		INLINE void Set(const Vector3Real<T>& r0, const Vector3Real<T>& r1, const Vector3Real<T>& r2)noexcept
		{
			R0 = r0;
			R1 = r1;
			R2 = r2;
		}
		INLINE void SetZero()noexcept
		{
			R0.Zero();
			R1.Zero();
			R2.Zero();
		}
		INLINE void SetIdentity()noexcept
		{
			R0.Set(T(1), T(0), T(0));
			R1.Set(T(0), T(1), T(0));
			R2.Set(T(0), T(0), T(1));
		}
		INLINE constexpr T Determinant()const noexcept
		{
			return R0.X * Matrix2Real<T>{R1.Y, R1.Z, R2.Y, R2.Z}.Determinant() 
				- R0.Y * Matrix2Real<T>{R1.X, R1.Z, R2.X, R2.Z}.Determinant() 
				+ R0.Z * Matrix2Real<T>{R1.X, R1.Y, R2.X, R2.Y}.Determinant();
		}
		INLINE constexpr Matrix3Real Transposed()const noexcept
		{
			return {	R0.X, R1.X, R2.X,
						R0.Y, R1.Y, R2.Y,
						R0.Z, R1.Z, R2.Z };
		}
		INLINE void Transpose()noexcept
		{
			*this = Transposed();
		}
		INLINE constexpr Matrix3Real Adjoint()const noexcept
		{
			return Matrix3Real{
				 Matrix2Real<T>{R1.Y, R1.Z, R2.Y, R2.Z}.Determinant(), -(Matrix2Real<T>{R1.X, R1.Z, R2.X, R2.Z}.Determinant()),  Matrix2Real<T>{R1.X, R1.Y, R2.X, R2.Y}.Determinant(),
				-(Matrix2Real<T>{R0.Y, R0.Z, R2.Y, R2.Z}.Determinant()), Matrix2Real<T>{R0.X, R0.Z, R2.X, R2.Z}.Determinant(), -(Matrix2Real<T>{R0.X, R0.Y, R2.X, R2.Y}.Determinant()),
				 Matrix2Real<T>{R0.Y, R0.Z, R1.Y, R1.Z}.Determinant(), -(Matrix2Real<T>{R0.X, R0.Z, R1.X, R1.Z}.Determinant()),  Matrix2Real<T>{R0.X, R0.Y, R1.X, R1.Y}.Determinant()
			};
		}
		INLINE constexpr Matrix3Real Inverted()const noexcept
		{
			T determinant = Determinant();
			if (::IsNearlyEqual(determinant, T(0), (T)MATH_TOLERANCE))
				return *this; // No inverse

			T invDeterminant = T(1) / determinant;
			Matrix3Real inv = Adjoint().Transposed();
			return {
				inv.R0 * invDeterminant,
				inv.R1 * invDeterminant,
				inv.R2 * invDeterminant
			};
		}
		INLINE void Inverse()noexcept
		{
			*this = Inverted();
		}
		INLINE constexpr float Trace()const noexcept
		{
			return R0.X + R1.Y + R2.Z;
		}
		INLINE constexpr bool IsNearlyEqual(const Matrix3Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return R0.IsNearlyEqual(other.R0, tolerance) && R1.IsNearlyEqual(other.R1, tolerance) && R2.IsNearlyEqual(other.R2, tolerance);
		}
		INLINE constexpr bool IsEqual(const Matrix3Real& other)const noexcept
		{
			return R0.IsEqual(other.R0) && R1.IsEqual(other.R1) && R2.IsEqual(other.R2);
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }, tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) });
		}
		INLINE constexpr bool IsNearlyIdentity(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) }, tolerance);
		}
		INLINE constexpr bool IsIdentity()const noexcept
		{
			return IsEqual({ T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) });
		}
		INLINE constexpr bool IsNearlySymmetric(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(R1.X, R0.Y, tolerance)
				&& ::IsNearlyEqual(R1.Z, R2.Y, tolerance)
				&& ::IsNearlyEqual(R2.X, R0.Z, tolerance);
		}
		INLINE constexpr bool IsSymmetric()const noexcept
		{
			return R1.X == R0.Y 
				&& R1.Z == R2.Y
				&& R2.X == R0.Z;
		}
		INLINE constexpr bool IsNearlyDiagonal(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(R0.Y, T(0), tolerance)
				&& ::IsNearlyEqual(R0.Z, T(0), tolerance)
				&& ::IsNearlyEqual(R1.X, T(0), tolerance)
				&& ::IsNearlyEqual(R1.Z, T(0), tolerance)
				&& ::IsNearlyEqual(R2.X, T(0), tolerance)
				&& ::IsNearlyEqual(R2.Y, T(0), tolerance);
		}
		INLINE constexpr bool IsDiagonal()const noexcept
		{
			return R0.Y == T(0)
				&& R0.Z == T(0)
				&& R1.X == T(0)
				&& R1.Z == T(0)
				&& R2.X == T(0)
				&& R2.Y == T(0);
		}
		INLINE String ToString()const noexcept
		{
			return Format(Print<T>::fmt, R0.X, R0.Y, R0.Z, R1.X, R1.Y, R1.Z, R2.X, R2.Y, R2.Z);
		}
		INLINE void FromString(StringView str)noexcept
		{
			sscanf(str.data(), Print<T>::fmt, &R0.X, &R0.Y, &R0.Z, &R1.X, &R1.Y, &R1.Z, &R2.X, &R2.Y, &R2.Z);
		}

		static const Matrix3Real IDENTITY;
		static const Matrix3Real ZERO;
	};

	template<class T> const Matrix3Real<T> Matrix3Real<T>::IDENTITY = { T(1), T(0), T(0),
																		T(0), T(1), T(0), 
																		T(0), T(0), T(1) };
	template<class T> const Matrix3Real<T> Matrix3Real<T>::ZERO = {  };

	template<class T> INLINE constexpr Matrix3Real<T> operator+(const Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { return { left.R0 + right.R0, left.R1 + right.R1, left.R2 + right.R2 }; }
	template<class T> INLINE constexpr Matrix3Real<T> operator-(const Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { return { left.R0 - right.R0, left.R1 - right.R1, left.R2 - right.R2 }; }
	template<class T> INLINE constexpr Matrix3Real<T> operator*(const Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept
	{
		return {
			left.R0.X * right.R0.X + left.R0.Y * right.R1.X + left.R0.Z * right.R2.X,	left.R0.X * right.R0.Y + left.R0.Y * right.R1.Y + left.R0.Z * right.R2.Y,	left.R0.X * right.R0.Z + left.R0.Y * right.R1.Z + left.R0.Z * right.R2.Z,
			left.R1.X * right.R0.X + left.R1.Y * right.R1.X + left.R1.Z * right.R2.X,	left.R1.X * right.R0.Y + left.R1.Y * right.R1.Y + left.R1.Z * right.R2.Y,	left.R1.X * right.R0.Z + left.R1.Y * right.R1.Z + left.R1.Z * right.R2.Z,
			left.R2.X * right.R0.X + left.R2.Y * right.R1.X + left.R2.Z * right.R2.X,	left.R2.X * right.R0.Y + left.R2.Y * right.R1.Y + left.R2.Z * right.R2.Y,	left.R2.X * right.R0.Z + left.R2.Y * right.R1.Z + left.R2.Z * right.R2.Z
		};
	}
	template<class T> INLINE Matrix3Real<T>& operator*=(Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { left = (left * right); return left; }
	template<class T> INLINE constexpr Vector3Real<T> operator*(const Vector3Real<T>& left, const Matrix3Real<T>& right)noexcept
	{
		return {
			left.X * right.R0.X + left.Y * right.R0.Y + left.Z * right.R0.Z,
			left.X * right.R1.X + left.Y * right.R1.Y + left.Z * right.R1.Z, 
			left.X * right.R2.X + left.Y * right.R2.Y + left.Z * right.R2.Z 
		};
	}
	template<class T> INLINE Vector3Real<T>& operator*=(Vector3Real<T>& left, const Matrix3Real<T>& right)noexcept { left = (left * right); return left; }
	template<class T> INLINE constexpr Vector3Real<T> operator*(const Matrix3Real<T>& left, const Vector3Real<T>& right)noexcept
	{
		return {
			left.R0.X * right.X + left.R0.Y * right.Y + left.R0.Z * right.Z,
			left.R1.X * right.X + left.R1.Y * right.Y + left.R1.Z * right.Z,
			left.R2.X * right.X + left.R2.Y * right.Y + left.R2.Z * right.Z
		};
	}

	template<class T> INLINE Matrix3Real<T> operator+=(Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { left.R0 += right.R0; left.R1 += right.R1; left.R2 += right.R2; return left; }
	template<class T> INLINE Matrix3Real<T> operator-=(Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { left.R0 -= right.R0; left.R1 -= right.R1; left.R2 -= right.R2; return left; }
	template<class T> INLINE Matrix3Real<T> operator*=(Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { left = (left * right); return left; }

	template<class T> INLINE constexpr Matrix3Real<T> operator*(const Matrix3Real<T>& left, T right)noexcept { return { left.R0 * right, left.R1 * right, left.R2 * right }; }
	template<class T> INLINE Matrix3Real<T> operator*=(Matrix3Real<T>& left, T right)noexcept { left.R0 *= right; left.R1 *= right; left.R2 *= right; return left; }
	template<class T> INLINE constexpr Matrix3Real<T> operator*(T left, const Matrix3Real<T>& right)noexcept { return { right.R0 * left, right.R1 * left, right.R2 * left }; }

	template<class T> INLINE constexpr bool operator==(const Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { return left.IsEqual(right); }
	template<class T> INLINE constexpr bool operator!=(const Matrix3Real<T>& left, const Matrix3Real<T>& right)noexcept { return !(left == right); }
}

#endif /* MATH_MATRIX3_H */