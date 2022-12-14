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

		static ssizet ToStream(const math::Matrix2Real<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Matrix2Real<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
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
		
		static bool FromJSON(math::Matrix2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(item) || cJSON_GetArraySize(item) != math::Matrix2Real<T>::ComponentCount)
				return false;
			
			
			return true;
		}

		static String ToString(const math::Matrix2Real<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Matrix2Real<T>& data)
		{
			data.FromString(str);
			return true;
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