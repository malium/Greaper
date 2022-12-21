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
		static SPtr<cJSON> CreateJSON(const type& data, StringView name){ \
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
		NODISCARD static sizet GetArraySize(UNUSED const type& data){\
			Break("[refl::PlainType<"#type">]]::GetArraySize Trying to use a PlainType for array operations!");\
			return 0ll;\
		}\
		static void SetArraySize(UNUSED type& data, UNUSED sizet size){\
			Break("[refl::PlainType<"#type">]]::GetArraySize Trying to use a PlainType for array operations!");\
		}\
		NODISCARD static const int32& GetArrayValue(UNUSED const type& data, UNUSED sizet index){\
			static constexpr int32 dummy = 0;\
			Break("[refl::PlainType<"#type">]]::GetArraySize Trying to use a PlainType for array operations!");\
			return dummy;\
		}\
		static void SetArrayValue(UNUSED type& data, UNUSED const int32& value, UNUSED sizet index){\
			Break("[refl::PlainType<"#type">]]::GetArraySize Trying to use a PlainType for array operations!");\
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

		static SPtr<cJSON> CreateJSON(const T& data, StringView name)
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

		NODISCARD static sizet GetArraySize(UNUSED const T& data)
		{
			Break("[refl::BaseType<TEnum>]::GetArraySize Trying to use the generic refl::BaseType!");
			return 0ll;
		}

		static void SetArraySize(UNUSED T& data, UNUSED sizet size)
		{
			Break("[refl::BaseType<TEnum>]::SetArraySize Trying to use the generic refl::BaseType!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const T& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::BaseType<TEnum>]::GetArrayValue Trying to use the generic refl::BaseType!");
			return dummy;
		}

		static void SetArrayValue(UNUSED T& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::BaseType<TEnum>]::SetArrayValue Trying to use the generic refl::BaseType!");
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
		static SPtr<cJSON> CreateJSON(const T& data, StringView name)
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

		NODISCARD static sizet GetArraySize(UNUSED const T& data)
		{
			Break("[refl::PlainType<TEnum>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED T& data, UNUSED sizet size)
		{
			Break("[refl::PlainType<TEnum>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const T& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::PlainType<TEnum>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED T& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::PlainType<TEnum>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};

	template<class First, class Second>
	struct PlainType<std::pair<First, Second>> : public BaseType<std::pair<First, Second>>
	{
		using Type = std::pair<First, Second>;
		using FirstCat = typename TypeInfo<First>::Type;
		using SecondCat = typename TypeInfo<Second>::Type;

		static_assert(!std::is_same_v<FirstCat, void> && !std::is_same_v<SecondCat, void>, "[refl::PlainType<std::pair>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = FirstCat::StaticSize + SecondCat::StaticSize;

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			auto dynamicSize = GetDynamicSize(data);

			ssizet size = 0;
			TResult<ssizet> res = FirstCat::ToStream(data.first, stream);
			if (res.HasFailed())
				return res;
			size += res.GetValue();

			res = SecondCat::ToStream(data.second, stream);
			if (res.HasFailed())
				return res;
			size += res.GetValue();

			ssizet expectedSize = StaticSize + dynamicSize;
			if (size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<std::pair>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			ssizet size = 0;
			int64 dynamicSize = 0;
			TResult<ssizet> res = FirstCat::FromStream(data.first, stream);
			if (res.HasFailed())
				return res;
			size += res.GetValue();
			dynamicSize += FirstCat::GetDynamicSize(data.first);

			res = SecondCat::FromStream(data.second, stream);
			if (res.HasFailed())
				return res;
			size += res.GetValue();
			dynamicSize += SecondCat::GetDynamicSize(data.second);

			ssizet expectedSize = dynamicSize + StaticSize;
			if (size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<std::pair>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			
			FirstCat::ToJSON(data.first, obj, "first"sv);
			SecondCat::ToJSON(data.second, obj, "second"sv);
			
			return obj;
		}

		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if (obj == nullptr)
				return Result::CreateFailure(Format("[refl::PlainType<std::pair>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			EmptyResult res = FirstCat::FromJSON(data.first, obj, "first"sv);
			if (res.HasFailed())
				return res;
			
			res = SecondCat::FromJSON(data.second, obj, "second"sv);
			if (res.HasFailed())
				return res;
			
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = ToJSON(data, "pair"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "pair"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			return FirstCat::GetDynamicSize(data.first) + SecondCat::GetDynamicSize(data.second);
		}

		NODISCARD static sizet GetArraySize(UNUSED const Type& data)
		{
			Break("[refl::PlainType<std::pair>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED Type& data, UNUSED sizet size)
		{
			Break("[refl::PlainType<std::pair>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const Type& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::PlainType<std::pair>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED Type& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::PlainType<std::pair>]::SetArrayValue Trying to use a PlainType for array operations!");
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