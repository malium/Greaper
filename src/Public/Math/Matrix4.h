/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_MATRIX4_H
#define MATH_MATRIX4_H 1

#include "Vector4.h"
#include "Matrix3.h"

namespace greaper::math
{
	template<class T>
	class Matrix4Real
	{
		static_assert(std::is_floating_point_v<T>, "Matrix4Real can only work with float, double or long double types");
	public:
		using Type_t = T;

		static constexpr sizet RowCount = 4;
		static constexpr sizet ColumnCount = 4;
		static constexpr sizet ComponentCount = RowCount * ColumnCount;

		Vector4Real<T> R0, R1, R2, R3;

		constexpr Matrix4Real()noexcept = default;
		INLINE constexpr Matrix4Real(T r00, T r01, T r02, T r03, T r10, T r11, T r12, T r13, T r20, T r21, T r22, T r23, T r30, T r31, T r32, T r33)noexcept 
			:R0(r00, r01, r02,r03), R1(r10, r11, r12, r13), R2(r20, r21, r22, r23), R3(r30, r31, r32, r33) {  }
		INLINE constexpr explicit Matrix4Real(const std::array<T, ComponentCount>& arr)noexcept
			:R0(arr[0], arr[1], arr[2], arr[3]), R1(arr[4], arr[5], arr[6], arr[7]), R2(arr[8], arr[9], arr[10], arr[11]), R3(arr[12], arr[13], arr[14], arr[15]) {  }
		INLINE constexpr Matrix4Real(const Vector4Real<T>& r0, const Vector4Real<T>& r1, const Vector4Real<T>& r2, const Vector4Real<T>& r3)noexcept :R0(r0), R1(r1), R2(r2), R3(r3) {  }
		INLINE constexpr explicit Matrix4Real(const Matrix3Real<T>& m3)noexcept :R0(m3.R0, T(0)), R1(m3.R1, T(0)), R2(m3.R2, T(0)), R3(T(0), T(0), T(0), T(1)) {  }

		INLINE T& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Matrix4, but the index %" PRIuPTR " was out of range.", index);
			return ((float*)&R0)[index];
		}
		INLINE constexpr const T& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Matrix4, but the index %" PRIuPTR " was out of range.", index);
			return ((const float*)&R0)[index];
		}

		INLINE Impl::Vector4Ref<T> C0()noexcept
		{
			return Impl::Vector4Ref<T>{ (T&)R0.X, (T&)R1.X, (T&)R2.X, (T&)R3.X };
		}
		INLINE Impl::Vector4CRef<T> C0()const noexcept
		{
			return Impl::Vector4CRef<T>{ R0.X, R1.X, R2.X, R3.X };
		}
		INLINE Impl::Vector4Ref<T> C1()noexcept
		{
			return Impl::Vector4Ref<T>{ (T&)R0.Y, (T&)R1.Y, (T&)R2.Y, (T&)R3.Y };
		}
		INLINE Impl::Vector4CRef<T> C1()const noexcept
		{
			return Impl::Vector4CRef<T>{ R0.Y, R1.Y, R2.Y, R3.Y };
		}
		INLINE Impl::Vector4Ref<T> C2()noexcept
		{
			return Impl::Vector4Ref<T>{ (T&)R0.Z, (T&)R1.Z, (T&)R2.Z, (T&)R3.Z };
		}
		INLINE Impl::Vector4CRef<T> C2()const noexcept
		{
			return Impl::Vector4CRef<T>{ R0.Z, R1.Z, R2.Z, R3.Z };
		}
		INLINE constexpr std::array<T, ComponentCount> ToArray()const noexcept
		{
			return { R0.X, R0.Y, R0.Z, R0.W, R1.X, R1.Y, R1.Z, R1.W, R2.X, R2.Y, R2.Z, R2.W, R3.X, R3.Y, R3.Z, R3.W };
		}
		INLINE void Set(const Matrix4Real& other)noexcept
		{
			R0 = other.R0;
			R1 = other.R1;
			R2 = other.R2;
			R3 = other.R3;
		}
		INLINE void Set(T r00, T r01, T r02, T r03, T r10, T r11, T r12, T r13, T r20, T r21, T r22, T r23, T r30, T r31, T r32, T r33)noexcept
		{
			R0.Set(r00, r01, r02, r03);
			R1.Set(r10, r11, r12, r13);
			R2.Set(r20, r21, r22, r23);
			R3.Set(r30, r31, r32, r33);
		}
		INLINE void Set(const Vector4Real<T>& r0, const Vector4Real<T>& r1, const Vector4Real<T>& r2, const Vector4Real<T>& r3)noexcept
		{
			R0 = r0;
			R1 = r1;
			R2 = r2;
			R3 = r3;
		}
		INLINE void SetZero()noexcept
		{
			R0.Zero();
			R1.Zero();
			R2.Zero();
			R3.Zero();
		}
		INLINE void SetIdentity()noexcept
		{
			R0.Set(T(1), T(0), T(0), T(0));
			R1.Set(T(0), T(1), T(0), T(0));
			R2.Set(T(0), T(0), T(1), T(0));
			R3.Set(T(0), T(0), T(0), T(1));
		}
		INLINE constexpr T Determinant()const noexcept
		{
			return R0.X * Matrix3Real<T>{R1.Y, R1.Z, R1.W, R2.Y, R2.Z, R2.W, R3.Y, R3.Z, R3.W}.Determinant()
				- R0.Y * Matrix3Real<T>{R1.X, R1.Z, R1.W, R2.X, R2.Z, R2.W, R3.X, R3.Z, R3.W}.Determinant()
				+ R0.Z * Matrix3Real<T>{R1.X, R1.Y, R1.W, R2.X, R2.Y, R2.W, R3.X, R3.Y, R3.W}.Determinant()
				- R0.W * Matrix3Real<T>{R1.X, R1.Y, R1.Z, R2.X, R2.Y, R2.Z, R3.X, R3.Y, R3.Z}.Determinant();
		}
		INLINE constexpr Matrix4Real Transposed()const noexcept
		{
			return { 
				R0.X, R1.X, R2.X, R3.X,
				R0.Y, R1.Y, R2.Y, R3.Y,
				R0.Z, R1.Z, R2.Z, R3.Z,
				R0.W, R1.W, R2.W, R3.W
				};
		}
		INLINE void Transpose()noexcept
		{
			*this = Transposed();
		}
		INLINE constexpr Matrix4Real Adjoint()const noexcept
		{
			return Matrix4Real{
				 (Matrix3Real<T>{R1.Y, R1.Z, R1.W, R2.Y, R2.Z, R2.W, R3.Y, R3.Z, R3.W}.Determinant()), -(Matrix3Real<T>{R0.Y, R0.Z, R0.W, R2.Y, R2.Z, R2.W, R3.Y, R3.Z, R3.W}.Determinant()),  (Matrix3Real<T>{R0.Y, R0.Z, R0.W, R1.Y, R1.Z, R1.W, R3.Y, R3.Z, R3.W}.Determinant()), -(Matrix3Real<T>{R0.Y, R0.Z, R0.W, R1.Y, R1.Z, R1.W, R2.Y, R2.Z, R2.W}.Determinant()),
				-(Matrix3Real<T>{R1.X, R1.Z, R1.W, R2.X, R2.Z, R2.W, R3.X, R3.Z, R3.W}.Determinant()),  (Matrix3Real<T>{R0.X, R0.Z, R0.W, R2.X, R2.Z, R2.W, R3.X, R3.Z, R3.W}.Determinant()), -(Matrix3Real<T>{R0.X, R0.Z, R0.W, R1.X, R1.Z, R1.W, R3.X, R3.Z, R3.W}.Determinant()),  (Matrix3Real<T>{R0.X, R0.Z, R0.W, R1.X, R1.Z, R1.W, R2.X, R2.Z, R2.W}.Determinant()),
				 (Matrix3Real<T>{R1.X, R1.Y, R1.W, R2.X, R2.Y, R2.W, R3.X, R3.Y, R3.W}.Determinant()), -(Matrix3Real<T>{R0.X, R0.Y, R0.W, R2.X, R2.Y, R2.W, R3.X, R3.Y, R3.W}.Determinant()),  (Matrix3Real<T>{R0.X, R0.Y, R0.W, R1.X, R1.Y, R1.W, R3.X, R3.Y, R3.W}.Determinant()), -(Matrix3Real<T>{R0.X, R0.Y, R0.W, R1.X, R1.Y, R1.W, R2.X, R2.Y, R2.W}.Determinant()),
				-(Matrix3Real<T>{R1.X, R1.Y, R1.Z, R2.X, R2.Y, R2.Z, R3.X, R3.Y, R3.Z}.Determinant()),  (Matrix3Real<T>{R0.X, R0.Y, R0.Z, R2.X, R2.Y, R2.Z, R3.X, R3.Y, R3.Z}.Determinant()), -(Matrix3Real<T>{R0.X, R0.Y, R0.Z, R1.X, R1.Y, R1.Z, R3.X, R3.Y, R3.Z}.Determinant()),  (Matrix3Real<T>{R0.X, R0.Y, R0.Z, R1.X, R1.Y, R1.Z, R2.X, R2.Y, R2.Z}.Determinant()),
			};
			
		}
		INLINE constexpr Matrix4Real Inverted()const noexcept
		{
			T determinant = Determinant();
			if (::IsNearlyEqual(determinant, T(0), (T)MATH_TOLERANCE))
				return *this; // No inverse

			T invDeterminant = T(1) / determinant;
			Matrix4Real inv = Adjoint().Transposed();
			return {
				inv.R0 * invDeterminant,
				inv.R1 * invDeterminant,
				inv.R2 * invDeterminant,
				inv.R3 * invDeterminant
			};
		}
		INLINE void Inverse()noexcept
		{
			*this = Inverted();
		}
		INLINE constexpr float Trace()const noexcept
		{
			return R0.X + R1.Y + R2.Z + R3.W;
		}
		INLINE constexpr bool IsNearlyEqual(const Matrix4Real& other, T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return R0.IsNearlyEqual(other.R0, tolerance) && R1.IsNearlyEqual(other.R1, tolerance) && R2.IsNearlyEqual(other.R2, tolerance) && R3.IsNearlyEqual(other.R3, tolerance);
		}
		INLINE constexpr bool IsEqual(const Matrix4Real& other)const noexcept
		{
			return R0.IsEqual(other.R0) && R1.IsEqual(other.R1) && R2.IsEqual(other.R2) && R3.IsEqual(other.R3);
		}
		INLINE constexpr bool IsNearlyZero(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }, tolerance);
		}
		INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) });
		}
		INLINE constexpr bool IsNearlyIdentity(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return IsNearlyEqual({ T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1) }, tolerance);
		}
		INLINE constexpr bool IsIdentity()const noexcept
		{
			return IsEqual({ T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1) });
		}
		INLINE constexpr bool IsNearlySymmetric(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(R0.Y, R1.X, tolerance)
				&& ::IsNearlyEqual(R0.Z, R2.X, tolerance)
				&& ::IsNearlyEqual(R0.W, R3.X, tolerance)
				&& ::IsNearlyEqual(R1.Z, R2.Y, tolerance)
				&& ::IsNearlyEqual(R1.W, R3.Y, tolerance)
				&& ::IsNearlyEqual(R2.W, R3.Z, tolerance);
		}
		INLINE constexpr bool IsSymmetric()const noexcept
		{
			return R0.Y == R1.X
				&& R0.Z == R2.X
				&& R0.W == R3.X
				&& R1.Z == R2.Y
				&& R1.W == R3.Y
				&& R2.W == R3.Z;
		}
		INLINE constexpr bool IsNearlyDiagonal(T tolerance = (T)MATH_TOLERANCE)const noexcept
		{
			return ::IsNearlyEqual(R0.Y, T(0), tolerance)
				&& ::IsNearlyEqual(R0.Z, T(0), tolerance)
				&& ::IsNearlyEqual(R0.W, T(0), tolerance)
				&& ::IsNearlyEqual(R1.X, T(0), tolerance)
				&& ::IsNearlyEqual(R1.Z, T(0), tolerance)
				&& ::IsNearlyEqual(R1.W, T(0), tolerance)
				&& ::IsNearlyEqual(R2.X, T(0), tolerance)
				&& ::IsNearlyEqual(R2.Y, T(0), tolerance)
				&& ::IsNearlyEqual(R2.W, T(0), tolerance)
				&& ::IsNearlyEqual(R3.X, T(0), tolerance)
				&& ::IsNearlyEqual(R3.Y, T(0), tolerance)
				&& ::IsNearlyEqual(R3.Z, T(0), tolerance);
		}
		INLINE constexpr bool IsDiagonal()const noexcept
		{
			return R0.Y == T(0)
				&& R0.Z == T(0)
				&& R0.W == T(0)
				&& R1.X == T(0)
				&& R1.Z == T(0)
				&& R1.W == T(0)
				&& R2.X == T(0)
				&& R2.Y == T(0)
				&& R2.W == T(0)
				&& R3.X == T(0)
				&& R3.Y == T(0)
				&& R3.Z == T(0);
		}
		INLINE String ToString()const noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				return Format("%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", R0.X, R0.Y, R0.Z, R0.W, R1.X, R1.Y, R1.Z, R1.W, R2.X, R2.Y, R2.Z, R2.W, R3.X, R3.Y, R3.Z, R3.W);
			else
				return Format("%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", R0.X, R0.Y, R0.Z, R0.W, R1.X, R1.Y, R1.Z, R1.W, R2.X, R2.Y, R2.Z, R2.W, R3.X, R3.Y, R3.Z, R3.W);
		}
		INLINE void FromString(StringView str)noexcept
		{
			if constexpr (std::is_same_v<T, float>)
				sscanf(str.data(), "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", &R0.X, &R0.Y, &R0.Z, &R0.W, &R1.X, &R1.Y, &R1.Z, &R1.W, &R2.X, &R2.Y, &R2.Z, &R2.W, &R3.X, &R3.Y, &R3.Z, &R3.W);
			else
				sscanf(str.data(), "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &R0.X, &R0.Y, &R0.Z, &R0.W, &R1.X, &R1.Y, &R1.Z, &R1.W, &R2.X, &R2.Y, &R2.Z, &R2.W, &R3.X, &R3.Y, &R3.Z, &R3.W);
		}

		static const Matrix4Real IDENTITY;
		static const Matrix4Real ZERO;
	};

	template<class T> const Matrix4Real<T> Matrix4Real<T>::IDENTITY = { T(1), T(0), T(0), T(0),
																		T(0), T(1), T(0), T(0),
																		T(0), T(0), T(1), T(0),
																		T(0), T(0), T(0), T(1)};
	template<class T> const Matrix4Real<T> Matrix4Real<T>::ZERO = {  };

	template<class T> INLINE constexpr Matrix4Real<T> operator+(const Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { return { left.R0 + right.R0, left.R1 + right.R1, left.R2 + right.R2, left.R3 + right.R3 }; }
	template<class T> INLINE constexpr Matrix4Real<T> operator-(const Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { return { left.R0 - right.R0, left.R1 - right.R1, left.R2 - right.R2, left.R3 - right.R3 }; }
	template<class T> INLINE constexpr Matrix4Real<T> operator*(const Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept
	{
		return {
			left.R0.X * right.R0.X + left.R0.Y * right.R1.X + left.R0.Z * right.R2.X + left.R0.W * right.R3.X,	left.R0.X * right.R0.Y + left.R0.Y * right.R1.Y + left.R0.Z * right.R2.Y + left.R0.W * right.R3.Y,	left.R0.X * right.R0.Z + left.R0.Y * right.R1.Z + left.R0.Z * right.R2.Z + left.R0.W * right.R3.Z,	left.R0.X * right.R0.W + left.R0.Y * right.R1.W + left.R0.Z * right.R2.W + left.R0.W * right.R3.W,
			left.R1.X * right.R0.X + left.R1.Y * right.R1.X + left.R1.Z * right.R2.X + left.R1.W * right.R3.X,	left.R1.X * right.R0.Y + left.R1.Y * right.R1.Y + left.R1.Z * right.R2.Y + left.R1.W * right.R3.Y,	left.R1.X * right.R0.Z + left.R1.Y * right.R1.Z + left.R1.Z * right.R2.Z + left.R1.W * right.R3.Z,	left.R1.X * right.R0.W + left.R1.Y * right.R1.W + left.R1.Z * right.R2.W + left.R1.W * right.R3.W,
			left.R2.X * right.R0.X + left.R2.Y * right.R1.X + left.R2.Z * right.R2.X + left.R2.W * right.R3.X,	left.R2.X * right.R0.Y + left.R2.Y * right.R1.Y + left.R2.Z * right.R2.Y + left.R2.W * right.R3.Y,	left.R2.X * right.R0.Z + left.R2.Y * right.R1.Z + left.R2.Z * right.R2.Z + left.R2.W * right.R3.Z,	left.R2.X * right.R0.W + left.R2.Y * right.R1.W + left.R2.Z * right.R2.W + left.R2.W * right.R3.W,
			left.R3.X * right.R0.X + left.R3.Y * right.R1.X + left.R3.Z * right.R2.X + left.R3.W * right.R3.X,	left.R3.X * right.R0.Y + left.R3.Y * right.R1.Y + left.R3.Z * right.R2.Y + left.R3.W * right.R3.Y,	left.R3.X * right.R0.Z + left.R3.Y * right.R1.Z + left.R3.Z * right.R2.Z + left.R3.W * right.R3.Z,	left.R3.X * right.R0.W + left.R3.Y * right.R1.W + left.R3.Z * right.R2.W + left.R3.W * right.R3.W,
		};
	}
	template<class T> INLINE Matrix4Real<T>& operator*=(Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { left = (left * right); return left; }
	template<class T> INLINE constexpr Vector4Real<T> operator*(const Vector4Real<T>& left, const Matrix4Real<T>& right)noexcept
	{
		return {
			left.X * right.R0.X + left.Y * right.R0.Y + left.Z * right.R0.Z + left.W * right.R0.W,
			left.X * right.R1.X + left.Y * right.R1.Y + left.Z * right.R1.Z + left.W * right.R1.W,
			left.X * right.R2.X + left.Y * right.R2.Y + left.Z * right.R2.Z + left.W * right.R2.W,
			left.X * right.R3.X + left.Y * right.R3.Y + left.Z * right.R3.Z + left.W * right.R3.W,
		};
	}
	template<class T> INLINE Vector4Real<T>& operator*=(Vector4Real<T>& left, const Matrix4Real<T>& right)noexcept { left = (left * right); return left; }
	template<class T> INLINE constexpr Vector4Real<T> operator*(const Matrix4Real<T>& left, const Vector4Real<T>& right)noexcept
	{
		return {
			left.R0.X * right.X + left.R0.Y * right.Y + left.R0.Z * right.Z + left.R0.W * right.W,
			left.R1.X * right.X + left.R1.Y * right.Y + left.R1.Z * right.Z + left.R1.W * right.W,
			left.R2.X * right.X + left.R2.Y * right.Y + left.R2.Z * right.Z + left.R2.W * right.W,
			left.R3.X * right.X + left.R3.Y * right.Y + left.R3.Z * right.Z + left.R3.W * right.W
		};
	}

	template<class T> INLINE Matrix4Real<T> operator+=(Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { left.R0 += right.R0; left.R1 += right.R1; left.R2 += right.R2; left.R3 += right.R3; return left; }
	template<class T> INLINE Matrix4Real<T> operator-=(Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { left.R0 -= right.R0; left.R1 -= right.R1; left.R2 -= right.R2; left.R3 -= right.R3; return left; }
	template<class T> INLINE Matrix4Real<T> operator*=(Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { left = (left * right); return left; }

	template<class T> INLINE constexpr Matrix4Real<T> operator*(const Matrix4Real<T>& left, T right)noexcept { return { left.R0 * right, left.R1 * right, left.R2 * right, left.R3 * right }; }
	template<class T> INLINE Matrix4Real<T> operator*=(Matrix4Real<T>& left, T right)noexcept { left.R0 *= right; left.R1 *= right; left.R2 *= right; left.R3 *= right; return left; }
	template<class T> INLINE constexpr Matrix4Real<T> operator*(T left, const Matrix4Real<T>& right)noexcept { return { right.R0 * left, right.R1 * left, right.R2 * left, right.R3 * left }; }

	template<class T> INLINE constexpr bool operator==(const Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { return left.IsEqual(right); }
	template<class T> INLINE constexpr bool operator!=(const Matrix4Real<T>& left, const Matrix4Real<T>& right)noexcept { return !(left == right); }
}

#endif /* MATH_MATRIX4_H */