/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_PREREQUISITES_H
#define MATH_PREREQUISITES_H 1

#include <Core/CorePrerequisites.h>

#define GLM_FORCE_SSE42
#include "glm/glm.hpp"
#include "glm/ext/quaternion_float.hpp"

namespace greaper::math
{
#ifndef MATH_TOLERANCE_VAL
	inline constexpr float MATH_TOLERANCE = 0.001f;
#else
	inline constexpr float MATH_TOLERANCE = MATH_TOLERANCE_VAL;
#endif

	template<class T> class Vector2Real;
	using Vector2f = Vector2Real<float>;
	using Vector2d = Vector2Real<double>;
	template<class T> class Vector2Signed;
	using Vector2i8 = Vector2Signed<int8>;
	using Vector2i16 = Vector2Signed<int16>;
	using Vector2i = Vector2Signed<int32>;
	using Vector2i64 = Vector2Signed<int64>;
	template<class T> class Vector2Unsigned;
	using Vector2u8 = Vector2Unsigned<uint8>;
	using Vector2u16 = Vector2Unsigned<uint16>;
	using Vector2u = Vector2Unsigned<uint32>;
	using Vector2u64 = Vector2Unsigned<uint64>;
	class Vector2b;

	template<class T> class Vector3Real;
	using Vector3f = Vector3Real<float>;
	using Vector3d = Vector3Real<double>;
	template<class T> class Vector3Signed;
	using Vector3i8 = Vector3Signed<int8>;
	using Vector3i16 = Vector3Signed<int16>;
	using Vector3i = Vector3Signed<int32>;
	using Vector3i64 = Vector3Signed<int64>;
	template<class T> class Vector3Unsigned;
	using Vector3u8 = Vector3Unsigned<uint8>;
	using Vector3u16 = Vector3Unsigned<uint16>;
	using Vector3u = Vector3Unsigned<uint32>;
	using Vector3u64 = Vector3Unsigned<uint64>;
	class Vector3b;

	template<class T> class Vector4Real;
	using Vector4f = Vector4Real<float>;
	using Vector4d = Vector4Real<double>;
	template<class T> class Vector4Signed;
	using Vector4i8 = Vector4Signed<int8>;
	using Vector4i16 = Vector4Signed<int16>;
	using Vector4i = Vector4Signed<int32>;
	using Vector4i64 = Vector4Signed<int64>;
	template<class T> class Vector4Unsigned;
	using Vector4u8 = Vector4Unsigned<uint8>;
	using Vector4u16 = Vector4Unsigned<uint16>;
	using Vector4u = Vector4Unsigned<uint32>;
	using Vector4u64 = Vector4Unsigned<uint64>;
	class Vector4b;

	class Vector4SSE;

	template<class T> class Matrix2Real;
	using Matrix2f = Matrix2Real<float>;
	using Matrix2d = Matrix2Real<double>;
	template<class T> class Matrix3Real;
	using Matrix3f = Matrix3Real<float>;
	using Matrix3d = Matrix3Real<double>;
	template<class T> class Matrix4Real;
	using Matrix4f = Matrix4Real<float>;
	using Matrix4d = Matrix4Real<double>;
	
	class Matrix4SSE;

	template<class T> class QuaternionReal;
	using QuaternionF = QuaternionReal<float>;
	using QuaternionD = QuaternionReal<double>;

	class QuaternionSSE;

	enum MathReflectedTypeID : ReflectedTypeID_t
	{
		// Math 
		RTI_Vector2f = 50,
		RTI_Vector2d,
		RTI_Vector2i,
		RTI_Vector2i8,
		RTI_Vector2i16,
		RTI_Vector2i64,
		RTI_Vector2u,
		RTI_Vector2u8,
		RTI_Vector2u16,
		RTI_Vector2u64,
		RTI_Vector2b,

		RTI_Vector3f,
		RTI_Vector3d,
		RTI_Vector3i,
		RTI_Vector3i8,
		RTI_Vector3i16,
		RTI_Vector3i64,
		RTI_Vector3u,
		RTI_Vector3u8,
		RTI_Vector3u16,
		RTI_Vector3u64,
		RTI_Vector3b,

		RTI_Vector4f,
		RTI_Vector4d,
		RTI_Vector4i,
		RTI_Vector4i8,
		RTI_Vector4i16,
		RTI_Vector4i64,
		RTI_Vector4u,
		RTI_Vector4u8,
		RTI_Vector4u16,
		RTI_Vector4u64,
		RTI_Vector4b,

		RTI_Vector4SSE,

		RTI_Matrix2f,
		RTI_Matrix2d,
		
		RTI_Matrix3f,
		RTI_Matrix3d,

		RTI_Matrix4f,
		RTI_Matrix4d,

		RTI_Matrix4SSE,

		RTI_QuaternionF,
		RTI_QuaternionD,
		RTI_QuaternionSSE,
	};

	void a()
	{
		glm::quat q = glm::quat();
	}
}

#include "Base/Utils.inl"
#include "Base/ReflectedConversions.h"

#endif /* MATH_PREREQUISITES_H */