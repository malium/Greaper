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
		static ssizet ToStream(const type& data, IStream& stream){ \
			return stream.Write(&data, sizeof(data)); \
		}\
		static ssizet FromStream(type& data, IStream& stream){ \
			return stream.Read(&data, sizeof(data));\
		}\
		static cJSON* ToJSON(const type& data, StringView name){ \
			cJSON* obj = cJSON_CreateObject(); \
			return ToJSON(data, obj, name); \
		} \
		static cJSON* ToJSON(const type& data, cJSON* obj, StringView name){\
			return jsonAddFn (obj, name.data(), data);\
		}\
		static bool FromJSON(type& data, cJSON* obj, StringView name){\
			cJSON* item = cJSON_GetObjectItemCaseSensitive(obj, name.data());\
			if(jsonIsFn (item)){\
				data = jsonGetFn (item);\
				return true;\
			}\
			return false;\
		}\
		NODISCARD static String ToString(const type& data){\
			return toString ;\
		}\
		static bool FromString(const String& str, type& data){\
			fromString ; \
			return true; \
		}\
		NODISCARD static int64 GetDynamicSize(UNUSED const type& data){\
			return 0ll; \
		}\
		static void SetDynamicSize(UNUSED type& data, UNUSED int64 size){}\
	};\
}

namespace greaper::refl
{
	/* Reflected enums need a special BaseType class */
	template<class T>
	struct BaseType<TEnum<T>>
	{
		static_assert(std::is_enum_v<T>, "refl::BaseType trying to instantiate a TEnum with a non enum type.");
		
		static inline constexpr uint64 ID = RTI_Enum;

		static inline constexpr ssizet StaticSize = sizeof(std::underlying_type_t<T>);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::COUNT;

		static ssizet ToStream(UNUSED const T& data, UNUSED IStream& stream)
		{
			Break("Trying to use the generic refl::BaseType!");
			return -1ll;
		}

		static ssizet FromStream(UNUSED T& data, UNUSED IStream& stream)
		{
			Break("Trying to use the generic refl::BaseType!");
			return -1ll;
		}

		static cJSON* ToJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(UNUSED const T& data, UNUSED cJSON* obj, UNUSED StringView name)
		{
			Break("Trying to use the generic refl::BaseType!");
			return nullptr;
		}
		
		static bool FromJSON(UNUSED T& data, UNUSED cJSON* json, UNUSED StringView name)
		{
			Break("Trying to use the generic refl::BaseType!");
			return false;
		}

		NODISCARD static String ToString(UNUSED const T& data)
		{
			Break("Trying to use the generic refl::BaseType!");
			return String{};
		}

		static bool FromString(UNUSED const String& str, UNUSED T& data)
		{
			Break("Trying to use the generic refl::BaseType!");
			return false;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const T& data)
		{
			Break("Trying to use the generic refl::BaseType!");
			return 0ll;
		}

		static void SetDynamicSize(UNUSED T& data, UNUSED int64 size)
		{
			Break("Trying to use the generic refl::BaseType!");
		}
	};

	template<class T>
	struct PlainType<TEnum<T>> : public BaseType<TEnum<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain;
		static ssizet ToStream(const T& data, IStream& stream)
		{ 
			return stream.Write(&data, sizeof(data)); 
		}
		static ssizet FromStream(T& data, IStream& stream)
		{ 
			return stream.Read(&data, sizeof(data));
		}
		static cJSON* ToJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}
		static cJSON* ToJSON(const T& data, cJSON* obj, StringView name)
		{
			auto str = TEnum<T>::ToString(data);
			return cJSON_AddStringToObject(obj, name.data(), str.data());
		}
		static bool FromJSON(T& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(cJSON_IsString(item))
			{
				data = TEnum<T>::FromString(cJSON_GetStringValue(item));
				return true;
			}
			return false;
		}
		NODISCARD static String ToString(const T& data)
		{
			return TEnum<T>::ToString(data);
		}
		static bool FromString(const String& str, T& data)
		{
			data = TEnum<T>::FromString(str);
			return true;
		}
		NODISCARD static int64 GetDynamicSize(UNUSED const T& data)
		{
			return 0ll; 
		}
		static void SetDynamicSize(UNUSED T& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}

CREATE_MEMCPY_PLAINTYPE(bool, 			data ? "true" : "false", 				data = StringUtils::ToLower(str) == "true", 				cJSON_AddBoolToObject, cJSON_IsBool, cJSON_IsTrue);
CREATE_MEMCPY_PLAINTYPE(int8,			String{ std::to_string(data).c_str() }, 	data = (int8)std::strtol(str.c_str(), nullptr, 10),		cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint8,			String{ std::to_string(data).c_str() }, 	data = (uint8)std::strtoul(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(int16,			String{ std::to_string(data).c_str() }, 	data = (int16)std::strtol(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint16,			String{ std::to_string(data).c_str() }, 	data = (uint16)std::strtoul(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(int32,			String{ std::to_string(data).c_str() }, 	data = (int32)std::strtol(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint32,			String{ std::to_string(data).c_str() }, 	data = (uint32)std::strtoul(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(int64,			String{ std::to_string(data).c_str() }, 	data = (int16)std::strtoll(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(uint64,			String{ std::to_string(data).c_str() }, 	data = (uint16)std::strtoull(str.c_str(), nullptr, 10),	cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(float,			String{ std::to_string(data).c_str() }, 	data = std::strtof(str.c_str(), nullptr),				cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);
CREATE_MEMCPY_PLAINTYPE(double,			String{ std::to_string(data).c_str() }, 	data = std::strtod(str.c_str(), nullptr),				cJSON_AddNumberToObject, cJSON_IsNumber, cJSON_GetNumberValue);

#endif /* CORE_REFLECTION_PLAINTYPE_H */