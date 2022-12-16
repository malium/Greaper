/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../../Core/Reflection/PlainType.h"
#include "../Matrix2.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::Matrix2Real<T>> : public BaseType<math::Matrix2Real<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Matrix2Real<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Matrix2Real>]::ToStream Failure while writing to stream, not all data was written, expected:%"PRIuPTR" obtained:%"PRIdPTR".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Matrix2Real<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Matrix2Real>]::FromStream Failure while reading from stream, not all data was read, expected:%"PRIuPTR" obtained:%"PRIdPTR".", sizeof(data), size));
		}

		static cJSON* ToJSON(const math::Matrix2Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Matrix2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr;
			if constexpr(std::is_same_v<T, float>)
				arr = cJSON_CreateFloatArray((const float*)&data, math::Matrix2Real<T>::ComponentCount);
			else
				arr = cJSON_CreateDoubleArray((const double*)&data, math::Matrix2Real<T>::ComponentCount);
			
			cJSON_AddItemToObject(json, name.data(), arr);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Matrix2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Matrix2Real>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ComplexType<Matrix2Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsArray."sv);
			
			int32 arrSize = cJSON_GetArraySize(arr);
			if(arrSize != math::Matrix2Real<T>::ComponentCount)
				return Result::CreateFailure(Format("[refl::ComplexType<Matrix2Real>]::FromJSON Couldn't obtain the value, it had different size, expected:%"PRIuPTR", obtained:%"PRId32".", math::Matrix2Real<T>::ComponentCount, arr));
			
			for(decltype(math::Matrix2Real<T>::ComponentCount) i = 0; i < math::Matrix2Real<T>::ComponentCount; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				if(!cJSON_IsNumber(item))
					return Result::CreateFailure("[refl::ComplexType<Matrix2Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);

				data[i] = static_cast<T>(cJSON_GetNumberValue(item));
			}
			
			return Result::CreateSuccess();
		}

		static String ToString(const math::Matrix2Real<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Matrix2Real<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Matrix2Real<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Matrix2Real<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}