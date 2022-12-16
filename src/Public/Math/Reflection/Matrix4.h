/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../../Core/Reflection/PlainType.h"
#include "../Matrix4.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::Matrix4Real<T>> : public BaseType<math::Matrix4Real<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Matrix4Real<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Matrix4Real>]::ToStream Failure while writing to stream, not all data was written, expected:%"PRIuPTR" obtained:%"PRIdPTR".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Matrix4Real<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Matrix4Real>]::FromStream Failure while reading from stream, not all data was read, expected:%"PRIuPTR" obtained:%"PRIdPTR".", sizeof(data), size));
		}

		static cJSON* ToJSON(const math::Matrix4Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Matrix4Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr;
			if constexpr(std::is_same_v<T, float>)
				arr = cJSON_CreateFloatArray((const float*)&data, math::Matrix4Real<T>::ComponentCount);
			else
				arr = cJSON_CreateDoubleArray((const double*)&data, math::Matrix4Real<T>::ComponentCount);
			
			cJSON_AddItemToObject(json, name.data(), arr);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Matrix4Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Matrix4Real>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ComplexType<Matrix4Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsArray."sv);
			
			int32 arrSize = cJSON_GetArraySize(arr);
			if(arrSize != math::Matrix4Real<T>::ComponentCount)
				return Result::CreateFailure(Format("[refl::ComplexType<Matrix4Real>]::FromJSON Couldn't obtain the value, it had different size, expected:%"PRIuPTR", obtained:%"PRId32".", math::Matrix2Real<T>::ComponentCount, arr));
			
			for(decltype(math::Matrix4Real<T>::ComponentCount) i = 0; i < math::Matrix4Real<T>::ComponentCount; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				if(!cJSON_IsNumber(item))
					return Result::CreateFailure("[refl::ComplexType<Matrix4Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);

				data[i] = static_cast<T>(cJSON_GetNumberValue(item));
			}
			
			return Result::CreateSuccess();
		}

		static String ToString(const math::Matrix4Real<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Matrix4Real<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Matrix4Real<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Matrix4Real<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}