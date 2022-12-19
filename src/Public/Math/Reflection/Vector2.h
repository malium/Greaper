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

		static TResult<ssizet> ToStream(const math::Vector2Real<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2Real>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR" obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector2Real<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2Real>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR" obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector2Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector2Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector2Real>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return Result::CreateFailure("[refl::ComplexType<Vector2Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector2Real<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector2Real<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
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

		static TResult<ssizet> ToStream(const math::Vector2Signed<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2Signed>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR" obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector2Signed<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2Signed>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR" obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector2Signed<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector2Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector2Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector2Signed>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return Result::CreateFailure("[refl::ComplexType<Vector2Signed>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector2Signed<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector2Signed<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
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

		static TResult<ssizet> ToStream(const math::Vector2Unsigned<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2Unsigned>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR" obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector2Unsigned<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2Unsigned>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR" obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector2Unsigned<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector2Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector2Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector2Unsigned>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return Result::CreateFailure("[refl::ComplexType<Vector2Unsigned>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector2Unsigned<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector2Unsigned<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
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

		static TResult<ssizet> ToStream(const math::Vector2b& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2b>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector2b& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector2b>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector2b& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector2b& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddBoolToObject(obj, "x", data.X);
			cJSON_AddBoolToObject(obj, "y", data.Y);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector2b& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector2b>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
				return Result::CreateFailure("[refl::ComplexType<Vector2b>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_IsTrue(x);
			data.Y = cJSON_IsTrue(y);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector2b& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector2b& data)
		{
			if(data.FromString(str))
				return Result::CreateSuccess();
			return Result::CreateFailure("[refl::ComplexType<Vector4b>]::FromString parsing error!"sv);
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