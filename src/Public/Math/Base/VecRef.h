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
NODISCARD INLINE Impl::Vector2XYRef<value_type> XY()noexcept { return Impl::Vector2XYRef<value_type>{ (value_type&)X, (value_type&)Y }; } \
NODISCARD INLINE Impl::Vector2XYCRef<value_type> XY()const noexcept { return Impl::Vector2XYCRef<value_type>{ X, Y }; } 

#define DEF_SWIZZLE_XZ() \
NODISCARD INLINE Impl::Vector2XZRef<value_type> XZ()noexcept { return Impl::Vector2XZRef<value_type>{ (value_type&)X, (value_type&)Z }; } \
NODISCARD INLINE Impl::Vector2XZCRef<value_type> XZ()const noexcept { return Impl::Vector2XZCRef<value_type>{ X, Z }; } 

#define DEF_SWIZZLE_XW() \
NODISCARD INLINE Impl::Vector2XWRef<value_type> XW()noexcept { return Impl::Vector2XWRef<value_type>{ (value_type&)X, (value_type&)W }; } \
NODISCARD INLINE Impl::Vector2XWCRef<value_type> XW()const noexcept { return Impl::Vector2XWCRef<value_type>{ X, W }; } 

#define DEF_SWIZZLE_YZ() \
NODISCARD INLINE Impl::Vector2YZRef<value_type> YZ()noexcept { return Impl::Vector2YZRef<value_type>{ (value_type&)Y, (value_type&)Z }; } \
NODISCARD INLINE Impl::Vector2YZCRef<value_type> YZ()const noexcept { return Impl::Vector2YZCRef<value_type>{ Y, Z }; } 

#define DEF_SWIZZLE_YW() \
NODISCARD INLINE Impl::Vector2YWRef<value_type> YW()noexcept { return Impl::Vector2YWRef<value_type>{ (value_type&)Y, (value_type&)W }; } \
NODISCARD INLINE Impl::Vector2YWCRef<value_type> YW()const noexcept { return Impl::Vector2YWCRef<value_type>{ Y, W }; } 

#define DEF_SWIZZLE_ZW() \
NODISCARD INLINE Impl::Vector2ZWRef<value_type> ZW()noexcept { return Impl::Vector2ZWRef<value_type>{ (value_type&)Z, (value_type&)W }; } \
NODISCARD INLINE Impl::Vector2ZWCRef<value_type> ZW()const noexcept { return Impl::Vector2ZWCRef<value_type>{ Z, W }; } 

#define DEF_SWIZZLE_XYZ() \
NODISCARD INLINE Impl::Vector3XYZRef<value_type> XYZ()noexcept { return Impl::Vector3XYZRef<value_type>{ (value_type&)X, (value_type&)Y, (value_type&)Z }; } \
NODISCARD INLINE Impl::Vector3XYZCRef<value_type> XYZ()const noexcept { return Impl::Vector3XYZCRef<value_type>{ X, Y, Z }; } 

#define DEF_SWIZZLE_XYW() \
NODISCARD INLINE Impl::Vector3XYWRef<value_type> XYW()noexcept { return Impl::Vector3XYWRef<value_type>{ (value_type&)X, (value_type&)Y, (value_type&)W }; } \
NODISCARD INLINE Impl::Vector3XYWCRef<value_type> XYW()const noexcept { return Impl::Vector3XYWCRef<value_type>{ X, Y, W }; }

#define DEF_SWIZZLE_XZW() \
NODISCARD INLINE Impl::Vector3XZWRef<value_type> XZW()noexcept { return Impl::Vector3XZWRef<value_type>{ (value_type&)X, (value_type&)Z, (value_type&)W }; } \
NODISCARD INLINE Impl::Vector3XZWCRef<value_type> XZW()const noexcept { return Impl::Vector3XZWCRef<value_type>{ X, Z, W }; }

#define DEF_SWIZZLE_YZW() \
NODISCARD INLINE Impl::Vector3YZWRef<value_type> YZW()noexcept { return Impl::Vector3YZWRef<value_type>{ (value_type&)Y, (value_type&)Z, (value_type&)W }; } \
NODISCARD INLINE Impl::Vector3YZWCRef<value_type> YZW()const noexcept { return Impl::Vector3YZWCRef<value_type>{ Y, Z, W }; }

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