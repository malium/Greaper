/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFL_RECT_H
#define CORE_REFL_RECT_H 1

#include "ComplexType.h"
#include "../Base/Rect.h"

#define CreateRectRefl(recttype)\
namespace greaper::refl{\
	template<>\
	const Vector<SPtr<IField>> ComplexType<recttype>::Fields = Vector<SPtr<IField>>({\
			SPtr<IField>(Construct<TField<recttype::value_type>>("Left"sv, \
			[](const void* obj) -> const void* { return &(((const recttype*)obj)->Left); },\
			[](void* obj, const void* value) { ((recttype*)obj)->Left = *((const recttype::value_type*)value); })),\
			SPtr<IField>(Construct<TField<recttype::value_type>>("Top"sv, \
			[](const void* obj) -> const void* { return &(((const recttype*)obj)->Top); },\
			[](void* obj, const void* value) { ((recttype*)obj)->Top = *((const recttype::value_type*)value); })),\
			SPtr<IField>(Construct<TField<recttype::value_type>>("Right"sv, \
			[](const void* obj) -> const void* { return &(((const recttype*)obj)->Right); },\
			[](void* obj, const void* value) { ((recttype*)obj)->Right = *((const recttype::value_type*)value); })),\
			SPtr<IField>(Construct<TField<recttype::value_type>>("Bottom"sv, \
			[](const void* obj) -> const void* { return &(((const recttype*)obj)->Bottom); },\
			[](void* obj, const void* value) { ((recttype*)obj)->Bottom = *((const recttype::value_type*)value); })),\
		});\
}

CreateRectRefl(greaper::RectF);
CreateRectRefl(greaper::RectD);
CreateRectRefl(greaper::RectI);
CreateRectRefl(greaper::RectU);

#undef CreateRectRefl

/*namespace greaper::refl
{
	template<class T> 
	struct ComplexType<RectT<T>> : public BaseType<RectT<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		using ValueCat = TypeInfo<T>::Type;

		static TResult<ssizet> ToStream(const RectT<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<RectT>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(RectT<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<RectT>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> CreateJSON(const RectT<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const RectT<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "left", data.Left);
			cJSON_AddNumberToObject(obj, "top", data.Top);
			cJSON_AddNumberToObject(obj, "right", data.Right);
			cJSON_AddNumberToObject(obj, "bottom", data.Bottom);
			return obj;
		}

		static EmptyResult FromJSON(RectT<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<RectT>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* left = cJSON_GetObjectItemCaseSensitive(item, "left");
			cJSON* top = cJSON_GetObjectItemCaseSensitive(item, "top");
			cJSON* right = cJSON_GetObjectItemCaseSensitive(item, "right");
			cJSON* bottom = cJSON_GetObjectItemCaseSensitive(item, "bottom");
			if(!cJSON_IsNumber(left) || !cJSON_IsNumber(top) || !cJSON_IsNumber(right) || !cJSON_IsNumber(bottom))
				return Result::CreateFailure("[refl::ComplexType<RectT>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.Left = cJSON_GetNumberValue(left);
			data.Top = cJSON_GetNumberValue(top);
			data.Right = cJSON_GetNumberValue(right);
			data.Bottom = cJSON_GetNumberValue(bottom);
			return Result::CreateSuccess();
		}

		NODISCARD static String ToString(const RectT<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, RectT<T>& data)
		{
			if(data.FromString(str))
				return Result::CreateSuccess();
			return Result::CreateFailure("[refl::ComplexType<RectT>]::FromString failed."sv);
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const RectT<T>& data)
		{
			return 0ll;
		}

		NODISCARD static sizet GetArraySize(UNUSED const UuRectT<T>id& data)
		{
			Break("[refl::ComplexType<RectT>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED RectT<T>& data, UNUSED sizet size)
		{
			Break("[refl::ComplexType<RectT>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const RectT<T>& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::ComplexType<RectT>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED RectT<T>& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::ComplexType<RectT>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};
}*/

#endif /* CORE_REFL_RECT_H */