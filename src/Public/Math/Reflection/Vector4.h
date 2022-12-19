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

		static TResult<ssizet> ToStream(const math::Vector4Real<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4Real>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector4Real<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4Real>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector4Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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
		
		static EmptyResult FromJSON(math::Vector4Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector4Real>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z) || !cJSON_IsNumber(w))
				return Result::CreateFailure("[refl::ComplexType<Vector4Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			data.W = cJSON_GetNumberValue(w);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector4Real<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector4Real<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
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

		static TResult<ssizet> ToStream(const math::Vector4Signed<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4Signed>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector4Signed<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4Signed>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector4Signed<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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
		
		static EmptyResult FromJSON(math::Vector4Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector4Signed>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z) || !cJSON_IsNumber(w))
				return Result::CreateFailure("[refl::ComplexType<Vector4Signed>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			data.W = cJSON_GetNumberValue(w);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector4Signed<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector4Signed<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
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

		static TResult<ssizet> ToStream(const math::Vector4Unsigned<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4Unsigned>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector4Unsigned<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4Unsigned>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector4Unsigned<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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
		
		static EmptyResult FromJSON(math::Vector4Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector4Unsigned>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z) || !cJSON_IsNumber(w))
				return Result::CreateFailure("[refl::ComplexType<Vector4Unsigned>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			data.W = cJSON_GetNumberValue(w);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector4Unsigned<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector4Unsigned<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
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

		static TResult<ssizet> ToStream(const math::Vector4b& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4b>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector4b& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector4b>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector4b& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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
		
		static EmptyResult FromJSON(math::Vector4b& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector4b>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			cJSON* w = cJSON_GetObjectItemCaseSensitive(item, "w");
			if(!cJSON_IsBool(x) || !cJSON_IsBool(y) || !cJSON_IsBool(z) || !cJSON_IsBool(w))
				return Result::CreateFailure("[refl::ComplexType<Vector4b>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsBool."sv);
			
			data.X = cJSON_IsTrue(x);
			data.Y = cJSON_IsTrue(y);
			data.Z = cJSON_IsTrue(z);
			data.W = cJSON_IsTrue(w);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector4b& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector4b& data)
		{
			if(data.FromString(str))
				return Result::CreateSuccess();
			return Result::CreateFailure("[refl::ComplexType<Vector4b>]::FromString parsing error!"sv);
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