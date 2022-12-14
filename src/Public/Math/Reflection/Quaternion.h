/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../../Core/Reflection/PlainType.h"
//#include "../Quaternion.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::QuaternionReal<T>> : public BaseType<math::QuaternionReal<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::QuaternionReal<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::QuaternionReal<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::QuaternionReal<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
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
		
		static bool FromJSON(math::QuaternionReal<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			if(!cJSON_IsNumber(w) || !cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z))
				return false;
			
			data.W = cJSON_GetNumberValue(w);
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			return true;
		}

		static String ToString(const math::QuaternionReal<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::QuaternionReal<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::QuaternionReal<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::QuaternionReal<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}