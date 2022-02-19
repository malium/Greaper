/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_ENUMERATION_H
#define CORE_ENUMERATION_H 1

#include "Memory.h"

/*** Enums with introspection information!
*	All enums done with this will automatically have FromString, ToString methods
*	which are constexpr allowing at compile-time what name of the enum value.
* 
*	In order to use this is as easy as:
*	ENUMERATION(enumName, firstValue, secondValue);
*	this will create an enum like this:
*	namespace EenumName
*	{
*		enum Type
*		{
*			firstValue,
*			secondValue,
*			COUNT
*		}
*	}
*	using enumName_t = EenumName::Type;
*	COUNT will always be added, so its not necessary to add it by you
*	Then will be as easy as this to get the introspection:
*	TEnum<enumName_t>::FromString("firstValue"sv) -> will return 0 at run and compile time
*	TEnum<enumName_t>::ToString(enumName_t::firstValue) -> will return "firstValue"sv at run and compile time
*	FromString also accepts a runtime string type such as greaper::String
* 
*	Currently there are two limits to this way of introspection:
*	- 16 different enum values (soft-limit, can be expanded if needed)
*	- Does not support valued enums (hard-limit, string conversion won't work on this cases)
*/

/*** Base class for all introspected enumerations */
template<class T>
class TEnum
{
public:
	using EnumType = T;
	static_assert(false, "Trying to use IEnum as standalone type.");

	static constexpr greaper::StringView ToString(T type) noexcept
	{
		return greaper::StringView{};
	}

	static constexpr T FromString(const greaper::String& str) noexcept
	{
		return T();
	}

	static constexpr T FromString(const greaper::StringView& str) noexcept
	{
		return T();
	}
};

/*** Returns whether the given type T is a GreaperEnum or not */
template<class T> struct IsGreaperEnum { static constexpr bool value = false; };
/*** Returns the TEnum class for the given type T, if type T is not a GreaperEnum will return void */
template<class T> struct GetGreaperEnumReflection { using ReflectionType = void; };
/*** Returns whether the give type T has an implementation of TEnum */
template<class T> struct HasEnumReflection { static constexpr bool value = !std::is_same_v<GetGreaperEnumReflection<T>::ReflectionType, void>; };

#define _ENUMDEF_BEGIN(name)\
namespace E##name {

#define _ENUMDEF_END(name)\
}\
using name##_t = E##name::Type;

#define _ENUMDEF_CLASS_BEGIN(name)\
template<> \
class TEnum<name##_t> \
{

#define _ENUMDEF_CLASS_MIDDLE()\
public:

#define _ENUMDEF_CLASS_END(name)\
};\
template<> struct IsGreaperEnum<name##_t> { static constexpr bool value = true; }; \
template<> struct GetGreaperEnumReflection<name##_t> { using ReflectionType = TEnum<name##_t>; }; \

#define _ENUMDEF_LUT_BEGIN(name)\
static constexpr const greaper::StringView _LUT[name##_t::COUNT] = {

#define _ENUMDEF_TOSTRING(name)\
static constexpr greaper::StringView ToString(name##_t type)noexcept \
{ \
	return _LUT[static_cast<sizet>(type)]; \
}

#define _ENUMDEF_FROMSTRINGVIEW(name)\
static constexpr name##_t FromString(const greaper::StringView& type)noexcept \
{ \
	for(sizet i = 0; i < ArraySize(_LUT); ++i) \
	{ \
		if(type.size() != _LUT[i].size()) \
			continue; \
		bool found = true; \
		for(sizet j = 0; j < _LUT[i].size(); ++j) \
		{ \
			if(type[j] != _LUT[i][j]) \
			{ \
				found = false; \
				break; \
			} \
		} \
		if(found) \
			return static_cast<name##_t>(i); \
	} \
	return name##_t::COUNT; \
}

#define _ENUMDEF_FROMSTRING(name)\
static constexpr name##_t FromString(const greaper::String& type)noexcept \
{ \
	return FromString(greaper::StringView{type.c_str(), type.size()}); \
}

#define _ENUMDEF_ENUM_BEGIN()\
	enum Type {

#define _ENUMDEF_ENUMLUT_END()\
	};

#define _ENUMDEF_START(name)				\
_ENUMDEF_BEGIN(name)						\
_ENUMDEF_ENUM_BEGIN()

#define _ENUMDEF_MIDDLE(name)				\
_ENUMDEF_ENUMLUT_END()						\
_ENUMDEF_END(name)							\
_ENUMDEF_CLASS_BEGIN(name)					\
_ENUMDEF_LUT_BEGIN(name)

#define _ENUMDEF_ENDING(name)				\
_ENUMDEF_ENUMLUT_END()						\
_ENUMDEF_CLASS_MIDDLE()						\
_ENUMDEF_TOSTRING(name)						\
_ENUMDEF_FROMSTRINGVIEW(name)				\
_ENUMDEF_FROMSTRING(name)					\
_ENUMDEF_CLASS_END(name)

#define ENUMERATION_1(name, val0)			\
_ENUMDEF_START(name)						\
val0,										\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_2(name, val0, val1)		\
_ENUMDEF_START(name)						\
val0, val1,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1 \
_ENUMDEF_ENDING(name)

#define ENUMERATION_3(name, val0, val1, val2)			\
_ENUMDEF_START(name)						\
val0, val1, val2,										\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_4(name, val0, val1, val2, val3)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,										\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_5(name, val0, val1, val2, val3, val4)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_6(name, val0, val1, val2, val3, val4, val5)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_7(name, val0, val1, val2, val3, val4, val5, val6)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_8(name, val0, val1, val2, val3, val4, val5, val6, val7)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7										\
_ENUMDEF_ENDING(name)


#define ENUMERATION_9(name, val0, val1, val2, val3, val4, val5, val6, val7, val8)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_10(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_11(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9, val10,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9, #val10										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_12(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9, val10, val11,									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9, #val10, #val11										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_13(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9, val10, val11, val12									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9, #val10, #val11, #val12										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_14(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9, val10, val11, val12, val13									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9, #val10, #val11, #val12, #val13										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_15(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9, #val10, #val11, #val12, #val13, #val14										\
_ENUMDEF_ENDING(name)

#define ENUMERATION_16(name, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14, val15)			\
_ENUMDEF_START(name)						\
val0, val1, val2, val3,	val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14, val15									\
COUNT										\
_ENUMDEF_MIDDLE(name)						\
#val0, #val1, #val2, #val3, #val4, #val5, #val6, #val7, #val8, #val9, #val10, #val11, #val12, #val13, #val14, #val15										\
_ENUMDEF_ENDING(name)

#define ENUM_CONCAT(A, B) _ENUM_CONCAT(A, B)
#define _ENUM_CONCAT(A, B) A##_##B

#define ENUMERATION(name, ...) \
ENUM_CONCAT(ENUMERATION, MACRO_GET_ARG_COUNT(__VA_ARGS__)(name, __VA_ARGS__))

#endif /* CORE_ENUMERATION_H */