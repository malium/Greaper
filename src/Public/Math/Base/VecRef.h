/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECREF_H
#define MATH_VECREF_H 1

namespace greaper::math::Impl
{
	template<class T> struct Vector2XYRef { T& X; T& Y; };				template<class T> struct Vector2XYCRef { const T X; const T Y; };
	template<class T> struct Vector2XZRef { T& X; T& Z; };				template<class T> struct Vector2XZCRef { const T X; const T Z; };
	template<class T> struct Vector2XWRef { T& X; T& W; };				template<class T> struct Vector2XWCRef { const T X; const T W; };
	template<class T> struct Vector2YZRef { T& Y; T& Z; };				template<class T> struct Vector2YZCRef { const T Y; const T Z; };
	template<class T> struct Vector2YWRef { T& Y; T& W; };				template<class T> struct Vector2YWCRef { const T Y; const T W; };
	template<class T> struct Vector2ZWRef { T& Z; T& W; };				template<class T> struct Vector2ZWCRef { const T Z; const T W; };
	template<class T> struct Vector3XYZRef { T& X; T& Y; T& Z; };		template<class T> struct Vector3XYZCRef { const T X; const T Y; const T Z; };
	template<class T> struct Vector3XYWRef { T& X; T& Y; T& W; };		template<class T> struct Vector3XYWCRef { const T X; const T Y; const T W; };
	template<class T> struct Vector3XZWRef { T& X; T& Z; T& W; };		template<class T> struct Vector3XZWCRef { const T X; const T Z; const T W; };
	template<class T> struct Vector3YZWRef { T& Y; T& Z; T& W; };		template<class T> struct Vector3YZWCRef { const T Y; const T Z; const T W; };
	template<class T> struct Vector4Ref { T& X; T& Y; T& Z; T& W; };	template<class T> struct Vector4CRef { const T X; const T Y; const T Z; const T W; };

#define DEF_SWIZZLE_XY() \
INLINE Impl::Vector2XYRef<Type_t> XY()noexcept { return Impl::Vector2XYRef<Type_t>{ (Type_t&)X, (Type_t&)Y }; } \
INLINE Impl::Vector2XYCRef<Type_t> XY()const noexcept { return Impl::Vector2XYCRef<Type_t>{ X, Y }; } 

#define DEF_SWIZZLE_XZ() \
INLINE Impl::Vector2XZRef<Type_t> XZ()noexcept { return Impl::Vector2XZRef<Type_t>{ (Type_t&)X, (Type_t&)Z }; } \
INLINE Impl::Vector2XZCRef<Type_t> XZ()const noexcept { return Impl::Vector2XZCRef<Type_t>{ X, Z }; } 

#define DEF_SWIZZLE_XW() \
INLINE Impl::Vector2XWRef<Type_t> XW()noexcept { return Impl::Vector2XWRef<Type_t>{ (Type_t&)X, (Type_t&)W }; } \
INLINE Impl::Vector2XWCRef<Type_t> XW()const noexcept { return Impl::Vector2XWCRef<Type_t>{ X, W }; } 

#define DEF_SWIZZLE_YZ() \
INLINE Impl::Vector2YZRef<Type_t> YZ()noexcept { return Impl::Vector2YZRef<Type_t>{ (Type_t&)Y, (Type_t&)Z }; } \
INLINE Impl::Vector2YZCRef<Type_t> YZ()const noexcept { return Impl::Vector2YZCRef<Type_t>{ Y, Z }; } 

#define DEF_SWIZZLE_YW() \
INLINE Impl::Vector2YWRef<Type_t> YW()noexcept { return Impl::Vector2YWRef<Type_t>{ (Type_t&)Y, (Type_t&)W }; } \
INLINE Impl::Vector2YWCRef<Type_t> YW()const noexcept { return Impl::Vector2YWCRef<Type_t>{ Y, W }; } 

#define DEF_SWIZZLE_ZW() \
INLINE Impl::Vector2ZWRef<Type_t> ZW()noexcept { return Impl::Vector2ZWRef<Type_t>{ (Type_t&)Z, (Type_t&)W }; } \
INLINE Impl::Vector2ZWCRef<Type_t> ZW()const noexcept { return Impl::Vector2ZWCRef<Type_t>{ Z, W }; } 

#define DEF_SWIZZLE_XYZ() \
INLINE Impl::Vector3XYZRef<Type_t> XYZ()noexcept { return Impl::Vector3XYZRef<Type_t>{ (Type_t&)X, (Type_t&)Y, (Type_t&)Z }; } \
INLINE Impl::Vector3XYZCRef<Type_t> XYZ()const noexcept { return Impl::Vector3XYZCRef<Type_t>{ X, Y, Z }; } 

#define DEF_SWIZZLE_XYW() \
INLINE Impl::Vector3XYWRef<Type_t> XYW()noexcept { return Impl::Vector3XYWRef<Type_t>{ (Type_t&)X, (Type_t&)Y, (Type_t&)W }; } \
INLINE Impl::Vector3XYWCRef<Type_t> XYW()const noexcept { return Impl::Vector3XYWCRef<Type_t>{ X, Y, W }; }

#define DEF_SWIZZLE_XZW() \
INLINE Impl::Vector3XZWRef<Type_t> XZW()noexcept { return Impl::Vector3XZWRef<Type_t>{ (Type_t&)X, (Type_t&)Z, (Type_t&)W }; } \
INLINE Impl::Vector3XZWCRef<Type_t> XZW()const noexcept { return Impl::Vector3XZWCRef<Type_t>{ X, Z, W }; }

#define DEF_SWIZZLE_YZW() \
INLINE Impl::Vector3YZWRef<Type_t> YZW()noexcept { return Impl::Vector3YZWRef<Type_t>{ (Type_t&)Y, (Type_t&)Z, (Type_t&)W }; } \
INLINE Impl::Vector3YZWCRef<Type_t> YZW()const noexcept { return Impl::Vector3YZWCRef<Type_t>{ Y, Z, W }; }

#define DEF_SWIZZLE_VEC3()\
DEF_SWIZZLE_XY()\
DEF_SWIZZLE_XZ()\
DEF_SWIZZLE_YZ()

#define DEF_SWIZZLE_VEC4()\
DEF_SWIZZLE_XY()\
DEF_SWIZZLE_XZ()\
DEF_SWIZZLE_XW()\
DEF_SWIZZLE_YZ()\
DEF_SWIZZLE_YW()\
DEF_SWIZZLE_ZW()\
DEF_SWIZZLE_XYZ()\
DEF_SWIZZLE_XYW()\
DEF_SWIZZLE_XZW()\
DEF_SWIZZLE_YZW()

}

#endif /* MATH_VECREF_H */