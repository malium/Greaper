/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "ComplexType.h"
//#include "../Base/Rect.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<RectT<T>> : public BaseType<RectT<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		using ValueCat = GetCategoryType<T>::Type;

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

		static cJSON* ToJSON(const RectT<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
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
		static void SetDynamicSize(UNUSED TProperty<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}