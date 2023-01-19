/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

CREATE_TYPEINFO_CNAME(greaper::math::Half, 			greaper::refl::RTI_Half,		PlainType, "Half");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2f,		greaper::refl::RTI_Vector2f,	ComplexType, "Vector2f");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2d,		greaper::refl::RTI_Vector2d,	ComplexType, "Vector2d");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2i,		greaper::refl::RTI_Vector2i,	ComplexType, "Vector2i");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2i8,		greaper::refl::RTI_Vector2i8,	ComplexType, "Vector2i8");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2i16,	greaper::refl::RTI_Vector2i16,	ComplexType, "Vector2i16");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2i64,	greaper::refl::RTI_Vector2i64,	ComplexType, "Vector2i64");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2u,		greaper::refl::RTI_Vector2u,	ComplexType, "Vector2u");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2u8,		greaper::refl::RTI_Vector2u8,	ComplexType, "Vector2u8");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2u16,	greaper::refl::RTI_Vector2u16,	ComplexType, "Vector2u16");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2u64,	greaper::refl::RTI_Vector2u64,	ComplexType, "Vector2u64");
CREATE_TYPEINFO_CNAME(greaper::math::Vector2b,		greaper::refl::RTI_Vector2b,	ComplexType, "Vector2b");

CREATE_TYPEINFO_CNAME(greaper::math::Vector3f,		greaper::refl::RTI_Vector3f,	ComplexType, "Vector3f");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3d,		greaper::refl::RTI_Vector3d,	ComplexType, "Vector3d");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3i,		greaper::refl::RTI_Vector3i,	ComplexType, "Vector3i");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3i8,		greaper::refl::RTI_Vector3i8,	ComplexType, "Vector3i8");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3i16,	greaper::refl::RTI_Vector3i16,	ComplexType, "Vector3i16");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3i64,	greaper::refl::RTI_Vector3i64,	ComplexType, "Vector3i64");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3u,		greaper::refl::RTI_Vector3u,	ComplexType, "Vector3u");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3u8,		greaper::refl::RTI_Vector3u8,	ComplexType, "Vector3u8");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3u16,	greaper::refl::RTI_Vector3u16,	ComplexType, "Vector3u16");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3u64,	greaper::refl::RTI_Vector3u64,	ComplexType, "Vector3u64");
CREATE_TYPEINFO_CNAME(greaper::math::Vector3b,		greaper::refl::RTI_Vector3b,	ComplexType, "Vector3b");

CREATE_TYPEINFO_CNAME(greaper::math::Vector4f,		greaper::refl::RTI_Vector4f,	ComplexType, "Vector4f");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4d,		greaper::refl::RTI_Vector4d,	ComplexType, "Vector4d");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4i,		greaper::refl::RTI_Vector4i,	ComplexType, "Vector4i");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4i8,		greaper::refl::RTI_Vector4i8,	ComplexType, "Vector4i8");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4i16,	greaper::refl::RTI_Vector4i16,	ComplexType, "Vector4i16");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4i64,	greaper::refl::RTI_Vector4i64,	ComplexType, "Vector4i64");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4u,		greaper::refl::RTI_Vector4u,	ComplexType, "Vector4u");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4u8,		greaper::refl::RTI_Vector4u8,	ComplexType, "Vector4u8");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4u16,	greaper::refl::RTI_Vector4u16,	ComplexType, "Vector4u16");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4u64,	greaper::refl::RTI_Vector4u64,	ComplexType, "Vector4u64");
CREATE_TYPEINFO_CNAME(greaper::math::Vector4b,		greaper::refl::RTI_Vector4b,	ComplexType, "Vector4b");

CREATE_TYPEINFO_CNAME(greaper::math::Matrix2f,		greaper::refl::RTI_Matrix2f,	ContainerType, "Matrix2f");
CREATE_TYPEINFO_CNAME(greaper::math::Matrix2d,		greaper::refl::RTI_Matrix2d,	ContainerType, "Matrix2d");

CREATE_TYPEINFO_CNAME(greaper::math::Matrix3f,		greaper::refl::RTI_Matrix3f,	ContainerType, "Matrix3f");
CREATE_TYPEINFO_CNAME(greaper::math::Matrix3d,		greaper::refl::RTI_Matrix3d,	ContainerType, "Matrix3d");

CREATE_TYPEINFO_CNAME(greaper::math::Matrix4f,		greaper::refl::RTI_Matrix4f,	ContainerType, "Matrix4f");
CREATE_TYPEINFO_CNAME(greaper::math::Matrix4d,		greaper::refl::RTI_Matrix4d,	ContainerType, "Matrix4d");

CREATE_TYPEINFO_CNAME(greaper::math::QuaternionF, 	greaper::refl::RTI_QuaternionF, ComplexType, "QuaternionF");
CREATE_TYPEINFO_CNAME(greaper::math::QuaternionD, 	greaper::refl::RTI_QuaternionD, ComplexType, "QuaternionD");

CREATE_TYPEINFO_CNAME(greaper::math::RectF, greaper::refl::RTI_RectF, ComplexType, "RectF");
CREATE_TYPEINFO_CNAME(greaper::math::RectD, greaper::refl::RTI_RectD, ComplexType, "RectD");
CREATE_TYPEINFO_CNAME(greaper::math::RectI, greaper::refl::RTI_RectI, ComplexType, "RectI");
CREATE_TYPEINFO_CNAME(greaper::math::RectU, greaper::refl::RTI_RectU, ComplexType, "RectU");