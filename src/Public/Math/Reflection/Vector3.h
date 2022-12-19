/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../../Core/Reflection/PlainType.h"
//#include "../Vector3.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::Vector3Real<T>> : public BaseType<math::Vector3Real<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Vector3Real<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3Real>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector3Real<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3Real>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector3Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector3Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector3Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector3Real>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z))
				return Result::CreateFailure("[refl::ComplexType<Vector3Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector3Real<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector3Real<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector3Real<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector3Real<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T> 
	struct ComplexType<math::Vector3Signed<T>> : public BaseType<math::Vector3Signed<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Vector3Signed<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3Signed>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector3Signed<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3Signed>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector3Signed<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector3Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector3Signed<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector3Signed>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z))
				return Result::CreateFailure("[refl::ComplexType<Vector3Signed>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector3Signed<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector3Signed<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector3Signed<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector3Signed<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T> 
	struct ComplexType<math::Vector3Unsigned<T>> : public BaseType<math::Vector3Unsigned<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Vector3Unsigned<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3Unsigned>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector3Unsigned<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3Unsigned>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector3Unsigned<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector3Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddNumberToObject(obj, "x", data.X);
			cJSON_AddNumberToObject(obj, "y", data.Y);
			cJSON_AddNumberToObject(obj, "z", data.Z);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector3Unsigned<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector3Unsigned>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			
			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			if(!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsNumber(z))
				return Result::CreateFailure("[refl::ComplexType<Vector3Unsigned>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);
			
			data.X = cJSON_GetNumberValue(x);
			data.Y = cJSON_GetNumberValue(y);
			data.Z = cJSON_GetNumberValue(z);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector3Unsigned<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector3Unsigned<T>& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector3Unsigned<T>& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector3Unsigned<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<> 
	struct ComplexType<math::Vector3b> : public BaseType<math::Vector3b>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Vector3b& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3b>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Vector3b& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Vector3b>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> ToJSON(const math::Vector3b& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Vector3b& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddBoolToObject(obj, "x", data.X);
			cJSON_AddBoolToObject(obj, "y", data.Y);
			cJSON_AddBoolToObject(obj, "z", data.Z);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Vector3b& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Vector3b>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			cJSON* x = cJSON_GetObjectItemCaseSensitive(item, "x");
			cJSON* y = cJSON_GetObjectItemCaseSensitive(item, "y");
			cJSON* z = cJSON_GetObjectItemCaseSensitive(item, "z");
			if(!cJSON_IsBool(x) || !cJSON_IsBool(y) || !cJSON_IsBool(z))
				return Result::CreateFailure("[refl::ComplexType<Vector3b>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsBool."sv);
			
			data.X = cJSON_IsTrue(x);
			data.Y = cJSON_IsTrue(y);
			data.Z = cJSON_IsTrue(z);
			return Result::CreateSuccess();
		}

		static String ToString(const math::Vector3b& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Vector3b& data)
		{
			if(data.FromString(str))
				return Result::CreateSuccess();
			return Result::CreateFailure("[refl::ComplexType<Vector4b>]::FromString parsing error!"sv);
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Vector3b& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED math::Vector3b& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}