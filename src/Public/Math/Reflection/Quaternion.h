/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_REFL_QUATERNION_H
#define MATH_REFL_QUATERNION_H 1

#include "../../Core/Reflection/ComplexType.h"
#include "../Quaternion.h"

#define CreateQuatRefl(quatType)\
namespace greaper::refl{\
	template<>\
	const Vector<SPtr<IField>> ComplexType<quatType>::Fields = Vector<SPtr<IField>>({\
			SPtr<IField>(Construct<TField<quatType::value_type>>("W"sv, \
			[](const void* obj) -> const void* { return &(((const quatType*)obj)->W); },\
			[](void* obj, const void* value) { ((quatType*)obj)->W = *((const quatType::value_type*)value); })),\
			SPtr<IField>(Construct<TField<quatType::value_type>>("X"sv, \
			[](const void* obj) -> const void* { return &(((const quatType*)obj)->X); },\
			[](void* obj, const void* value) { ((quatType*)obj)->X = *((const quatType::value_type*)value); })),\
			SPtr<IField>(Construct<TField<quatType::value_type>>("Y"sv, \
			[](const void* obj) -> const void* { return &(((const quatType*)obj)->Y); },\
			[](void* obj, const void* value) { ((quatType*)obj)->Y = *((const quatType::value_type*)value); })),\
			SPtr<IField>(Construct<TField<quatType::value_type>>("Z"sv, \
			[](const void* obj) -> const void* { return &(((const quatType*)obj)->Z); },\
			[](void* obj, const void* value) { ((quatType*)obj)->Z = *((const quatType::value_type*)value); })),\
		});\
}

CreateQuatRefl(greaper::math::QuaternionF);
CreateQuatRefl(greaper::math::QuaternionD);

#undef CreateQuatRefl

/*namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::QuaternionReal<T>> : public BaseType<math::QuaternionReal<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::QuaternionReal<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<QuaternionReal>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::QuaternionReal<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<QuaternionReal>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> CreateJSON(const math::QuaternionReal<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::QuaternionReal<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "w", data.W);
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::QuaternionReal<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<QuaternionReal>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(w) || !cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z))
				return Result::CreateFailure("[refl::ComplexType<QuaternionReal>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.W = cJSON_GetNumberValue(w);
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			return Result::CreateSuccess();
		}

		static String ToString(const math::QuaternionReal<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::QuaternionReal<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::QuaternionReal<T>& data)
		{
			return 0ll;
		}

		NODISCARD static sizet GetArraySize(UNUSED const math::QuaternionReal<T>& data)
		{
			Break("[refl::ComplexType<QuaternionReal>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED math::QuaternionReal<T>& data, UNUSED sizet size)
		{
			Break("[refl::ComplexType<QuaternionReal>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const math::QuaternionReal<T>& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::ComplexType<QuaternionReal>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED math::QuaternionReal<T>& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::ComplexType<QuaternionReal>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};
}*/

#endif /* MATH_REFL_QUATERNION_H */