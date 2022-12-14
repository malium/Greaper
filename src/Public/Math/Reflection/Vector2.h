/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../../Core/Reflection/PlainType.h"
//#include "../Vector2.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::Vector2Real<T>> : public BaseType<math::Vector2Real<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector2Real<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector2Real<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector2Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static bool FromJSON(math::Vector2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return false;
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			return true;
		}

		static String ToString(const math::Vector2Real<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector2Real<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector2Real<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector2Real<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T> 
	struct ComplexType<math::Vector2Signed<T>> : public BaseType<math::Vector2Signed<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector2Signed<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector2Signed<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector2Signed<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector2Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static bool FromJSON(math::Vector2Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return false;
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			return true;
		}

		static String ToString(const math::Vector2Signed<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector2Signed<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector2Signed<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector2Signed<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T> 
	struct ComplexType<math::Vector2Unsigned<T>> : public BaseType<math::Vector2Unsigned<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector2Unsigned<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector2Unsigned<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector2Unsigned<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector2Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static bool FromJSON(math::Vector2Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return false;
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			return true;
		}

		static String ToString(const math::Vector2Unsigned<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector2Unsigned<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector2Unsigned<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector2Unsigned<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<> 
	struct ComplexType<math::Vector2b> : public BaseType<math::Vector2b>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector2b& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector2b& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector2b& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector2b& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddBoolToObject(obj, "x", data.X);
			cJSON_AddBoolToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static bool FromJSON(math::Vector2b& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsBool(x) || !cJSON_IsBool(y))
				return false;
			
			data.X = cJSON_IsTrue(x);
			data.Y = cJSON_IsTrue(y);
			return true;
		}

		static String ToString(const math::Vector2b& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector2b& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector2b& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector2b& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}