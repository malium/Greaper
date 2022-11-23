/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper
{
	template<> struct ReflectedTypeToID<math::Vector2f> { static constexpr ReflectedTypeID_t ID = RTI_Vector2f; };
	template<> struct ReflectedTypeToID<math::Vector2d> { static constexpr ReflectedTypeID_t ID = RTI_Vector2d; };
	template<> struct ReflectedTypeToID<math::Vector2i> { static constexpr ReflectedTypeID_t ID = RTI_Vector2i; };
	template<> struct ReflectedTypeToID<math::Vector2i8> { static constexpr ReflectedTypeID_t ID = RTI_Vector2i8; };
	template<> struct ReflectedTypeToID<math::Vector2i16> { static constexpr ReflectedTypeID_t ID = RTI_Vector2i16; };
	template<> struct ReflectedTypeToID<math::Vector2i64> { static constexpr ReflectedTypeID_t ID = RTI_Vector2i64; };
	template<> struct ReflectedTypeToID<math::Vector2u> { static constexpr ReflectedTypeID_t ID = RTI_Vector2u; };
	template<> struct ReflectedTypeToID<math::Vector2u8> { static constexpr ReflectedTypeID_t ID = RTI_Vector2u8; };
	template<> struct ReflectedTypeToID<math::Vector2u16> { static constexpr ReflectedTypeID_t ID = RTI_Vector2u16; };
	template<> struct ReflectedTypeToID<math::Vector2u64> { static constexpr ReflectedTypeID_t ID = RTI_Vector2u64; };
	template<> struct ReflectedTypeToID<math::Vector2b> { static constexpr ReflectedTypeID_t ID = RTI_Vector2b; };

	template<> struct ReflectedTypeToID<math::Vector3f> { static constexpr ReflectedTypeID_t ID = RTI_Vector3f; };
	template<> struct ReflectedTypeToID<math::Vector3d> { static constexpr ReflectedTypeID_t ID = RTI_Vector3d; };
	template<> struct ReflectedTypeToID<math::Vector3i> { static constexpr ReflectedTypeID_t ID = RTI_Vector3i; };
	template<> struct ReflectedTypeToID<math::Vector3i8> { static constexpr ReflectedTypeID_t ID = RTI_Vector3i8; };
	template<> struct ReflectedTypeToID<math::Vector3i16> { static constexpr ReflectedTypeID_t ID = RTI_Vector3i16; };
	template<> struct ReflectedTypeToID<math::Vector3i64> { static constexpr ReflectedTypeID_t ID = RTI_Vector3i64; };
	template<> struct ReflectedTypeToID<math::Vector3u> { static constexpr ReflectedTypeID_t ID = RTI_Vector3u; };
	template<> struct ReflectedTypeToID<math::Vector3u8> { static constexpr ReflectedTypeID_t ID = RTI_Vector3u8; };
	template<> struct ReflectedTypeToID<math::Vector3u16> { static constexpr ReflectedTypeID_t ID = RTI_Vector3u16; };
	template<> struct ReflectedTypeToID<math::Vector3u64> { static constexpr ReflectedTypeID_t ID = RTI_Vector3u64; };
	template<> struct ReflectedTypeToID<math::Vector3b> { static constexpr ReflectedTypeID_t ID = RTI_Vector3b; };

	template<> struct ReflectedTypeToID<math::Vector4f> { static constexpr ReflectedTypeID_t ID = RTI_Vector4f; };
	template<> struct ReflectedTypeToID<math::Vector4d> { static constexpr ReflectedTypeID_t ID = RTI_Vector4d; };
	template<> struct ReflectedTypeToID<math::Vector4i> { static constexpr ReflectedTypeID_t ID = RTI_Vector4i; };
	template<> struct ReflectedTypeToID<math::Vector4i8> { static constexpr ReflectedTypeID_t ID = RTI_Vector4i8; };
	template<> struct ReflectedTypeToID<math::Vector4i16> { static constexpr ReflectedTypeID_t ID = RTI_Vector4i16; };
	template<> struct ReflectedTypeToID<math::Vector4i64> { static constexpr ReflectedTypeID_t ID = RTI_Vector4i64; };
	template<> struct ReflectedTypeToID<math::Vector4u> { static constexpr ReflectedTypeID_t ID = RTI_Vector4u; };
	template<> struct ReflectedTypeToID<math::Vector4u8> { static constexpr ReflectedTypeID_t ID = RTI_Vector4u8; };
	template<> struct ReflectedTypeToID<math::Vector4u16> { static constexpr ReflectedTypeID_t ID = RTI_Vector4u16; };
	template<> struct ReflectedTypeToID<math::Vector4u64> { static constexpr ReflectedTypeID_t ID = RTI_Vector4u64; };
	template<> struct ReflectedTypeToID<math::Vector4b> { static constexpr ReflectedTypeID_t ID = RTI_Vector4b; };

	template<> struct ReflectedTypeToID<math::Vector4SSE> { static constexpr ReflectedTypeID_t ID = RTI_Vector4SSE; };

	template<> struct ReflectedTypeToID<math::Matrix2f> { static constexpr ReflectedTypeID_t ID = RTI_Matrix2f; };
	template<> struct ReflectedTypeToID<math::Matrix2d> { static constexpr ReflectedTypeID_t ID = RTI_Matrix2d; };

	template<> struct ReflectedTypeToID<math::Matrix3f> { static constexpr ReflectedTypeID_t ID = RTI_Matrix3f; };
	template<> struct ReflectedTypeToID<math::Matrix3d> { static constexpr ReflectedTypeID_t ID = RTI_Matrix3d; };

	template<> struct ReflectedTypeToID<math::Matrix4f> { static constexpr ReflectedTypeID_t ID = RTI_Matrix4f; };
	template<> struct ReflectedTypeToID<math::Matrix4d> { static constexpr ReflectedTypeID_t ID = RTI_Matrix4d; };

	template<> struct ReflectedTypeToID<math::Matrix4SSE> { static constexpr ReflectedTypeID_t ID = RTI_Matrix4SSE; };

	template<> struct ReflectedTypeToID<math::QuaternionF> { static constexpr ReflectedTypeID_t ID = RTI_QuaternionF; };
	template<> struct ReflectedTypeToID<math::QuaternionD> { static constexpr ReflectedTypeID_t ID = RTI_QuaternionD; };

	template<> struct ReflectedTypeToID<math::QuaternionSSE> { static constexpr ReflectedTypeID_t ID = RTI_QuaternionSSE; };
}