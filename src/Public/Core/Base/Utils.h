/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <cstring>
#include <type_traits>
#include <tuple>

/** Checks if a value is within range [min,max) */
template<typename T>
INLINE constexpr bool IsWithin(const T& value, const T& min, const T& max)
{
	return value >= min && value < max;
}
/** Checks if a value is within range (min,max) */
template<typename T>
INLINE constexpr bool IsWithinExclusive(const T& value, const T& min, const T& max)
{
	return value > min && value < max;
}
/** Checks if a value is within range [min, max] */
template<class T>
INLINE constexpr bool IsWithinInclusive(const T& value, const T& min, const T& max)
{
	return value >= min && value <= max;
}
INLINE constexpr uint32 BitsLog2(uint32 v)
{
	switch(v)
	{
	case 16:
		return 4;
	case 32:
		return 5;
	case 64:
		return 6;
	};
	return 3;
}
/** Checks if a number is a power of two */
template<typename T>
INLINE constexpr bool IsPowerOfTwo(const T value)
{
	return value != 0 && !(value & (value - 1));
}
/** Rounds number up/down to the next multiple, multiple must be power of two */
template<typename T>
INLINE constexpr T RoundUp(const T number, const T multiple)
{
	static_assert(!IsPowerOfTwo(multiple), "Multiple must be power of two.");
	const T result = (number + multiple - 1) & ~(multiple - 1);
	static_assert(number <= result && result < number + multiple);
	return result;
}
/** Computes the factor that tells by how much is bigger/smaller */
template<typename T>
INLINE constexpr T DivideAndRoundUp(T n, T d) noexcept
{
	return (n + d - 1) / d;
}
/** Rounds the given number up to the next higher power of two */
template<typename T>
INLINE constexpr T RoundUpToPowerOf2(T val)
{
	--val;
	val |= val >> 1;
	val |= val >> 2;
	val |= val >> 4;
	val |= val >> 8;
	val |= val >> 16;
	++val;
	return val;
}
/** Returns the highest of 3 values */
template<typename T>
INLINE constexpr T Max3(const T a, const T b, const T c)
{
	return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}
/** Returns the lowest of 3 values */
template<typename T>
INLINE constexpr T Min3(const T a, const T b, const T c)
{
	return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}
/** Returns a higher value */
template<class T>
INLINE constexpr T Max(const T a, const T b)
{
	return (a >= b) ? a : b;
}
/** Returns the lower value */
template<class T>
INLINE constexpr T Min(const T a, const T b)
{
	return (a <= b) ? a : b;
}
/** Clamps the value between a minimum and a maximum */
template<typename T>
INLINE constexpr T Clamp(const T a, const T min, const T max)
{
	return Max(min, Min(max, a));
}
/** Computes the absolute value */
template<class T>
INLINE constexpr T Abs(const T a)
{
	return (a >= (T)0) ? a : -a;
}
INLINE constexpr bool IsNearlyEqual(float a, float b, float tolerance = 0.0001f)
{
	return Abs(a - b) <= tolerance;
}
INLINE constexpr bool IsNearlyEqual(double a, double b, double tolerance = 0.0001)
{
	return Abs(a - b) <= tolerance;
}
/** Returns a * a */
template<typename T>
INLINE constexpr T Square(const T a)
{
	return a * a;
}
/** Returns a * a * a */
template<typename T>
INLINE constexpr T Cube(const T a)
{
	return a * a * a;
}
/** Returns 1, 0 or -1 depending on relation of T to 0 */
template<class T>
INLINE constexpr T Sign(const T a)
{
	return ((a > (T)0) ? (T)1 : ((a) < (T)0) ? (T)-1 : (T)0);
}
/** Clamps an int32 into the int8 range. */
INLINE constexpr int8 ClampChar(const int32 i)
{
	return static_cast<int8>(Clamp(i, -128, 127));
}
/** Clamps an int32 into the int16 range. */
INLINE constexpr int16 ClampShort(const int32 i)
{
	return static_cast<int16>(Clamp(i, -32768, 32767));
}
/** Retruns true if value is NaN */
INLINE bool IsNaN(const float f) noexcept
{
	return (*reinterpret_cast<const uint32*>(&f) & 0x7FFFFFFF) > 0x7F800000;
}
/** Returns true if a value is finite */
INLINE bool IsFinite(const float f)
{
	return (*reinterpret_cast<const uint32*>(&f) & 0x7F800000) != 0x7F800000;
}
/** Returns true if a value is infinite */
INLINE bool IsInfinite(const float f)
{
	return !IsFinite(f);
}
/** Get the boolean value from a flag */
INLINE bool GetBitValue(const uint8* ptr, const uint32 index)noexcept
{
	const auto* const bytePtr = ptr + index / 8;
	const auto mask = static_cast<uint8>(1u << (index & 0x7u));
	return (*bytePtr & mask) != 0;
}
/** Set the boolean value to a flag */
INLINE void SetBitValue(uint8* ptr, const uint32 index, const bool set)noexcept
{
	const auto bytePtr = ptr + index / 8;
	const auto mask = static_cast<uint8>(1u << (index & 0x7u));
	if (set)
		*bytePtr |= mask;
	else
		*bytePtr &= (uint8)~mask;
}
/** Returns a tuple of each element of the version number */
INLINE constexpr std::tuple<uint8, uint8, uint8, uint8> GetGreaperVersionValues(uint32 version)
{
	return { (uint8)VERSION_GET_MAJOR(version), (uint8)VERSION_GET_MINOR(version),
		(uint8)VERSION_GET_PATCH(version), (uint8)VERSION_GET_REV(version) };
}
/***********************************************************************************
*                              HASH HELPER FUNCTIONS                               *
***********************************************************************************/
namespace std { template<class T> struct hash; }
/**
*	Computes the hash of a type given the seed.
*/
template<typename T>
INLINE void HashCombine(sizet& seed, const T& val)
{
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
/**
*	Computes the hash of the concatenated types given the seed.
*/
template<typename FirstArgType, typename... RestArgsType>
INLINE void HashCombine(sizet& seed, const FirstArgType& firstArg, const RestArgsType&... restArgs)
{
	HashCombine(seed, firstArg);
	HashCombine(seed, restArgs...);
}
/**
*	Computes the hash of the concatenated types.
*/
template<typename... ArgsType>
INLINE sizet ComputeHash(const ArgsType&... args)
{
	sizet seed = 0;
	HashCombine(seed, args...);
	return seed;
}

/***********************************************************************************
*                             MEMORY HELPER FUNCITONS                              *
***********************************************************************************/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

template<class T, sizet N>
INLINE constexpr sizet ArraySize(T(&)[N]) noexcept { return N; }

template<class T>
INLINE void ClearMemory(T& obj, sizet count = 1) noexcept
{
	memset(&obj, 0, sizeof(T) * count);
}
template<class T, size_t N>
INLINE void ClearMemory(T(&arr)[N]) noexcept
{
	memset(&arr[0], 0, sizeof(std::remove_extent<T>::type) * N);
}
template<class T>
INLINE void DuplicateMemory(const T& source, T& dst) noexcept
{
	memcpy(&dst, &source, sizeof(T));
}
template<class T>
INLINE bool IsMemoryCleared(const T& data) noexcept
{
	static const uint8 zeros[sizeof(T)]  = {};
	return ::memcmp(&data, &zeros, sizeof(T)) == 0;
}

/***********************************************************************************
*                              MACRO HELPER FUNCITONS                              *
***********************************************************************************/

#define MACRO_EVAL(...) __VA_ARGS__

#define MACRO_ARG_N(													\
		 _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, 						\
		 _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, 				\
		 _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, 				\
		 _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, 				\
		 _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, 				\
		 _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, 				\
		 _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, 				\
		 _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, 				\
		 _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, 				\
		 _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, 			\
		 _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, 	\
		 _111, _112, _113, _114, _115, _116, _117, _118, _119, N, ...) N

#define MACRO_RSEQ_N() 								\
		 119,118,117,116,115,114,113,112,111,110,	\
		 109,108,107,106,105,104,103,102,101,100,	\
		 99,98,97,96,95,94,93,92,91,90, 			\
		 89,88,87,86,85,84,83,82,81,80, 			\
		 79,78,77,76,75,74,73,72,71,70, 			\
		 69,68,67,66,65,64,63,62,61,60, 			\
		 59,58,57,56,55,54,53,52,51,50, 			\
		 49,48,47,46,45,44,43,42,41,40, 			\
		 39,38,37,36,35,34,33,32,31,30, 			\
		 29,28,27,26,25,24,23,22,21,20, 			\
		 19,18,17,16,15,14,13,12,11,10, 			\
		 9,8,7,6,5,4,3,2,1,0

#define _MACRO_GET_ARG_COUNT(...) MACRO_EVAL(MACRO_ARG_N(__VA_ARGS__))
#define MACRO_GET_ARG_COUNT(...) _MACRO_GET_ARG_COUNT(__VA_ARGS__, MACRO_RSEQ_N())

#define _MACRO_CONCAT(A, B) A##B
#define MACRO_CONCAT(A, B) _MACRO_CONCAT(A, B)
