/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_PLAINTYPE_H
#define CORE_REFLECTION_PLAINTYPE_H 1

#include "BaseType.h"

#define CREATE_MEMCPY_PLAINTYPE(type, toString, fromString, jsonAddFn, jsonIsFn, jsonGetFn)\
namespace greaper::refl { \
	template<> struct PlainType<type> : public BaseType<type> {\
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain; \
		static TResult<ssizet> ToStream(const type& data, IStream& stream){ \
			ssizet size = stream.Write(&data, sizeof(data));\
			if(size == sizeof(data))\
				return Result::CreateSuccess(size); \
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<"#type">]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));\
		}\
		static TResult<ssizet> FromStream(type& data, IStream& stream){ \
			ssizet size = stream.Read(&data, sizeof(data));\
			if(size == sizeof(data))\
				return Result::CreateSuccess(size);\
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<"#type">]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));\
		}\
		static SPtr<cJSON> ToJSON(const type& data, StringView name){ \
			cJSON* obj = cJSON_CreateObject(); \
			ToJSON(data, obj, name); \
			return SPtr<cJSON>(obj, cJSON_Delete); \
		} \
		static cJSON* ToJSON(const type& data, cJSON* obj, StringView name){\
			return jsonAddFn (obj, name.data(), data);\
		}\
		static EmptyResult FromJSON(type& data, cJSON* obj, StringView name){\
			cJSON* item = cJSON_GetObjectItemCaseSensitive(obj, name.data());\
			if(item == nullptr)\
				return Result::CreateFailure(Format("[refl::PlainType<"#type">]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));\
			if(jsonIsFn (item)){\
				data = static_cast<type>( jsonGetFn (item) );\
				return Result::CreateSuccess();\
			}\
			return Result::CreateFailure("[refl::PlainType<"#type">]::FromJSON Couldn't obtain the value from the json, expected: "#jsonIsFn"."sv);\
		}\
		NODISCARD static String ToString(const type& data){\
			return toString ;\
		}\
		static EmptyResult FromString(const String& str, type& data){\
			fromString ; \
			return Result::CreateSuccess(); \
		}\
		NODISCARD static int64 GetDynamicSize(UNUSED const type& data){\
			return 0ll; \
		}\
	};\
}

namespace greaper::refl
{
	/* Reflected enums need a special BaseType class */
	template<class T>
	struct BaseType<TEnum<T>>
	{
		static_assert(std::is_enum_v<T>, "[refl::BaseType<TEnum>] trying to instantiate a TEnum with a non enum type.");
		
		static inline constexpr uint64 ID = RTI_Enum;

		static inline constexpr ssizet StaticSize = sizeof(std::underlying_type_t<T>);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::COUNT;

		static TResult<ssizet> ToStream(UNUSED const T& data, UNUSED IStream& stream)
		{
			return Result::CreateFailure("[refl::BaseType<TEnum>]::ToStream Trying to use the generic refl::BaseType!"sv);
		}

		static TResult<ssizet> FromStream(UNUSED T& data, UNUSED IStream& stream)
		{
			return Result::CreateFailure("[refl::BaseType<TEnum>]::FromStream Trying to use the generic refl::BaseType!"sv);
		}

		static SPtr<cJSON> ToJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(UNUSED const T& data, UNUSED cJSON* obj, UNUSED StringView name)
		{
			Break("[refl::BaseType<TEnum>]::ToJSON Trying to use the generic refl::BaseType!");
			return nullptr;
		}
		
		static EmptyResult FromJSON(UNUSED T& data, UNUSED cJSON* json, UNUSED StringView name)
		{
			return Result::CreateFailure("[refl::BaseType<TEnum>]::FromJSON Trying to use the generic refl::BaseType!"sv);
		}

		NODISCARD static String ToString(UNUSED const T& data)
		{
			Break("[refl::BaseType<TEnum>]::ToString Trying to use the generic refl::BaseType!");
			return String{};
		}

		static EmptyResult FromString(UNUSED const String& str, UNUSED T& data)
		{
			return Result::CreateFailure("[refl::BaseType<TEnum>]::FromString Trying to use the generic refl::BaseType!"sv);
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const T& data)
		{
			Break("[refl::BaseType<TEnum>]::GetDyanmicSize Trying to use the generic refl::BaseType!");
			return 0ll;
		}
	};

	template<class T>
	struct PlainType<TEnum<T>> : public BaseType<TEnum<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain;
		static TResult<ssizet> ToStream(const T& data, IStream& stream)
		{ 
			ssizet size = stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<TEnum>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}
		static TResult<ssizet> FromStream(T& data, IStream& stream)
		{ 
			ssizet size = stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<TEnum>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}
		static SPtr<cJSON> ToJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}
		static cJSON* ToJSON(const T& data, cJSON* obj, StringView name)
		{
			auto str = TEnum<T>::ToString(data);
			return cJSON_AddStringToObject(obj, name.data(), str.data());
		}
		static EmptyResult FromJSON(T& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::PlainType<TEnum>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(cJSON_IsString(item))
			{
				data = TEnum<T>::FromString(cJSON_GetStringValue(item));
				return Result::CreateSuccess();
			}
			return Result::CreateFailure("[refl::PlainType<TEnum>]::FromJSON Couldn't obtain the value, it wasn't ENUM."sv);
		}
		NODISCARD static String ToString(const T& data)
		{
			return TEnum<T>::ToString(data);
		}
		static EmptyResult FromString(const String& str, T& data)
		{
			data = TEnum<T>::FromString(str);
			return Result::CreateSuccess();
		}
		NODISCARD static int64 GetDynamicSize(UNUSED const T& data)
		{
			return 0ll; 
		}
	};
}

CREATE_MEMCPY_PLAINTYPE(bool, 			data ? "true" : "false", 					data = StringUtils::ToLower(str) == "true", 			cJSON_AddBoolToObject, 		cJSON_IsBool, 	cJSON_IsTrue);
CREATE_MEMCPY_PLAINTYPE(int8,			String{ std::to_string(data).c_str() }, 	data = (int8)std::strtol(str.c_str(), nullptr, 10),		cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint8,			String{ std::to_string(data).c_str() }, 	data = (uint8)std::strtoul(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(int16,			String{ std::to_string(data).c_str() }, 	data = (int16)std::strtol(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint16,			String{ std::to_string(data).c_str() }, 	data = (uint16)std::strtoul(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(int32,			String{ std::to_string(data).c_str() }, 	data = (int32)std::strtol(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint32,			String{ std::to_string(data).c_str() }, 	data = (uint32)std::strtoul(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(int64,			String{ std::to_string(data).c_str() }, 	data = (int16)std::strtoll(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint64,			String{ std::to_string(data).c_str() }, 	data = (uint16)std::strtoull(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(float,			String{ std::to_string(data).c_str() }, 	data = std::strtof(str.c_str(), nullptr),				cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(double,			String{ std::to_string(data).c_str() }, 	data = std::strtod(str.c_str(), nullptr),				cJSON_AddNumberToObject, 	cJSON_IsNumber, cJSON_GetNumberValue);

#endif /* CORE_REFLECTION_PLAINTYPE_H */