/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../../Core/Reflection/PlainType.h"
//#include "../Vector4.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::Vector4Real<T>> : public BaseType<math::Vector4Real<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector4Real<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector4Real<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector4Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector4Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			cJSON_AddNumberToObject(obj, "w", data.W);
			return obj;			
		}
		
		static bool FromJSON(math::Vector4Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z) || !cJSON_IsNumber(w))
				return false;
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			data.W = cJSON_GetNumberValue(w);
			return true;
		}

		static String ToString(const math::Vector4Real<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector4Real<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector4Real<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector4Real<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T> 
	struct ComplexType<math::Vector4Signed<T>> : public BaseType<math::Vector4Signed<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector4Signed<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector4Signed<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector4Signed<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector4Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			cJSON_AddNumberToObject(obj, "w", data.W);
			return obj;			
		}
		
		static bool FromJSON(math::Vector4Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z) || !cJSON_IsNumber(w))
				return false;
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			data.W = cJSON_GetNumberValue(w);
			return true;
		}

		static String ToString(const math::Vector4Signed<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector4Signed<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector4Signed<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector4Signed<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T> 
	struct ComplexType<math::Vector4Unsigned<T>> : public BaseType<math::Vector4Unsigned<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector4Unsigned<T>& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector4Unsigned<T>& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector4Unsigned<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector4Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			cJSON_AddNumberToObject(obj, "w", data.W);
			return obj;			
		}
		
		static bool FromJSON(math::Vector4Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z) || !cJSON_IsNumber(w))
				return false;
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			data.W = cJSON_GetNumberValue(w);
			return true;
		}

		static String ToString(const math::Vector4Unsigned<T>& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector4Unsigned<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector4Unsigned<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector4Unsigned<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<> 
	struct ComplexType<math::Vector4b> : public BaseType<math::Vector4b>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static ssizet ToStream(const math::Vector4b& data, IStream& stream)
		{
			return stream.Write(&data, sizeof(data));
		}

		static ssizet FromStream(math::Vector4b& data, IStream& stream)
		{
			return stream.Read(&data, sizeof(data));
		}

		static cJSON* ToJSON(const math::Vector4b& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const math::Vector4b& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddBoolToObject(obj, "x", data.X);
			cJSON_AddBoolToObject(obj, "y", data.Y);
			cJSON_AddBoolToObject(obj, "z", data.Z);
			cJSON_AddBoolToObject(obj, "w", data.W);
			return obj;			
		}
		
		static bool FromJSON(math::Vector4b& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsBool(x) || !cJSON_IsBool(y) || !cJSON_IsBool(z) || !cJSON_IsBool(w))
				return false;
			
			data.X = cJSON_IsTrue(x);
			data.Y = cJSON_IsTrue(y);
			data.Z = cJSON_IsTrue(z);
			data.Z = cJSON_IsTrue(w);
			return true;
		}

		static String ToString(const math::Vector4b& data)
		{
			return data.ToString();
		}

		static bool FromString(const String& str, math::Vector4b& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector4b& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector4b& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}