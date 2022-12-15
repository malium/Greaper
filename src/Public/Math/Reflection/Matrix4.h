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

		static ssizet ToStream(const math::Matrix4Real<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Matrix4Real<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
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
		
		static bool FromJSON(math::Matrix4Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr) || cJSON_GetArraySize(arr) != math::Matrix4Real<T>::ComponentCount)
				return false;
			
			bool ok = true;
			for(decltype(math::Matrix4Real<T>::ComponentCount) i = 0; i < math::Matrix4Real<T>::ComponentCount; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				if(!cJSON_IsNumber(item))
				{
					ok = false;
					continue;
				}
				data[i] = static_cast<T>(cJSON_GetNumberValue(item));
			}
			
			return ok;
		}

		static String ToString(const math::Matrix4Real<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Matrix4Real<T>& data)
		{
			data.FromString(str);
			return true;
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