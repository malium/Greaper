/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_CONTAINERTYPE_H
#define CORE_REFLECTION_CONTAINERTYPE_H 1

#include "PlainType.h"
#include <inttypes.h>

namespace greaper::refl
{
	template<>
	struct ContainerType<String> : public BaseType<String>
	{
		using Type = String;

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			int64 elementCount = data.size();
			size += stream.Write(&elementCount, sizeof(elementCount));
			int64 dynamicSize = GetDynamicSize(data);
			size += stream.Write(data.data(), dynamicSize);
			auto expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<String>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));
			data.clear();
			data.resize(elementCount);
			int64 dynamicSize = GetDynamicSize(data);
			size += stream.Read(data.data(), dynamicSize);
			auto expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<String>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* obj, StringView name)
		{
			return cJSON_AddStringToObject(obj, name.data(), data.c_str());
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<WString>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(cJSON_IsString(item))
			{
				data.assign(cJSON_GetStringValue(item));
				return Result::CreateSuccess();
			}
			return Result::CreateFailure("[refl::ContainerType<String>]::FromJSON Couldn't obtain the value from the json, expected: cJSON_IsString"sv);
		}

		static String ToString(const Type& data)
		{
			return data;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			data.assign(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			return data.size() * sizeof(Type::value_type);
		}

		static void SetDynamicSize(UNUSED Type& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<>
	struct ContainerType<WString> : public BaseType<WString>
	{
		using Type = WString;

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			int64 elementCount = data.size();
			size += stream.Write(&elementCount, sizeof(elementCount));
			int64 dynamicSize = GetDynamicSize(data);
			size += stream.Write(data.data(), dynamicSize);
			auto expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<WString>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));
			data.clear();
			data.resize(elementCount);
			int64 dynamicSize = GetDynamicSize(data);
			size += stream.Read(data.data(), dynamicSize);
			auto expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<WString>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* obj, StringView name)
		{
			// TODO: Convert into UTF8 instead of ANSI
			return cJSON_AddStringToObject(obj, name.data(), StringUtils::FromWIDE(data).c_str());
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<WString>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(cJSON_IsString(item))
			{
				// TODO: Convert from UTF8 instead of ANSI
				data = StringUtils::ToWIDE(cJSON_GetStringValue(item));
				return Result::CreateSuccess();
			}
			return Result::CreateFailure("[refl::ContainerType<WString>]::FromJSON Couldn't obtain the value from the json, expected: cJSON_IsString"sv);
		}

		static String ToString(const Type& data)
		{
			return StringUtils::FromWIDE(data);
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			data = StringUtils::ToWIDE(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			return data.size() * sizeof(Type::value_type);
		}

		static void SetDynamicSize(UNUSED Type& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T, sizet N>
	struct ContainerType<std::array<T, N>> : public BaseType<std::array<T, N>>
	{
		using Type = std::array<T, N>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::array>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = ValueCat::StaticSize * N;

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			ssizet dynamicSize = GetDynamicSize(data);
			if constexpr(std::is_same_v<ValueCat, PlainType<T>>)
			{
				size += stream.Write(data.data(), StaticSize);
			}
			else
			{
				for(const T& elem : data)
				{
					TResult<ssizet> res = ValueCat::ToStream(elem, stream);
					if(res.HasFailed())
						return res;

					size += res.GetValue();
				}
			}
			auto expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::array>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			ssizet size = 0;
			ssizet dynamicSize = GetDynamicSize(data);
			if constexpr(std::is_same_v<ValueCat, PlainType<T>>)
			{
				size += stream.Read(data.data(), StaticSize);
			}
			else
			{
				for(T& elem : data)
				{
					TResult<ssizet> res = ValueCat::FromStream(elem, stream);
					if(res.HasFailed())
						return res;

					size += res.GetValue();
				}
			}
			auto expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::array>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			for(sizet i = 0; i < N; ++i)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::array>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::array>]::FromJSON expected an Array."sv);
			sizet arrSize = cJSON_GetArraySize(arr);
			if(arrSize != N)
				return Result::CreateFailure(Format("[refl::ContainerType<std::array>]::FromJSON expected an Array with size '" PRIuPTR "' but obtained '" PRIuPTR "'.", N, arrSize));
			
			achar buff[128];
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				EmptyResult res = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "array"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			auto res = FromJSON(data, json, "array"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>>)
				return 0ll;
			
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(UNUSED Type& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class A>
	struct ContainerType<Vector<T, A>> : public BaseType<Vector<T, A>>
	{
		using Type = Vector<T, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::vector>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			if constexpr(std::is_same_v<ValueCat, PlainType<T>>)
			{
				size += stream.Write(data.data(), dynamicSize);
			}
			else
			{
				for(const T& elem : data)
				{
					TResult<ssizet> res = ValueCat::ToStream(elem, stream);
					if(res.HasFailed())
						return res;
					
					size += res.GetValue();
				}
			}
			auto expectedSize = dynamicSize + StaticSize; 
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::vector>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			data.resize(elementCount);
			int64 dynamicSize = 0;
			if constexpr(std::is_same_v<ValueCat, PlainType<T>>)
			{
				dynamicSize = elementCount * sizeof(T);
				size += stream.Read(&data.data(), dynamicSize);
			}
			else
			{
				for(auto it = data.begin(); it != data.end(); ++it)
				{
					T elem;
					TResult<ssizet> res = ValueCat::FromStream(elem, stream);
					if(res.HasFailed())
						return res;
					
					(*it) = elem;
					dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
					size += res.GetValue();
				}
			}
			auto expectedSize = dynamicSize + StaticSize; 
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::vector>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::vector>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::vector>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			data.resize(N);
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				EmptyResult res = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "vector"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "vector"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>>)
			{
				return sizeof(T) * data.size();
			}
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class A>
	struct ContainerType<List<T, A>> : public BaseType<List<T, A>>
	{
		using Type = List<T, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::list>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const T& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::list>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			int64 dynamicSize = 0;
			data.clear();
			data.resize(elementCount);
			for(auto it = data.begin(); it != data.end(); ++it)
			{
				T elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				(*it) = elem;
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::list>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::list>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::list>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			data.resize(N);
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				EmptyResult res = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "list"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "list"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class A>
	struct ContainerType<Deque<T, A>> : public BaseType<Deque<T, A>>
	{
		using Type = Deque<T, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::deque>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const T& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::deque>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			int64 dynamicSize = 0;
			data.clear();
			data.resize(elementCount);
			for(auto it = data.begin(); it != data.end(); ++it)
			{
				T elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				(*it) = elem;
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::deque>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);

				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::deque>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::deque>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			data.resize(N);
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				EmptyResult res = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "queue"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "queue"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class C, class A>
	struct ContainerType<Set<T, C, A>> : public BaseType<Set<T, C, A>>
	{
		using Type = Set<T, C, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::set>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const T& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::set>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			int64 dynamicSize = 0;
			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::set>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::set>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::set>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				T elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "set"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "set"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class C, class A>
	struct ContainerType<MultiSet<T, C, A>> : public BaseType<MultiSet<T, C, A>>
	{
		using Type = MultiSet<T, C, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::multiset>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const T& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::multiset>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			int64 dynamicSize = 0;
			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::multiset>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::multiset>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::multiset>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				T elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "multiset"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "multiset"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class H, class C, class A>
	struct ContainerType<UnorderedSet<T, H, C, A>> : public BaseType<UnorderedSet<T, H, C, A>>
	{
		using Type = UnorderedSet<T, H, C, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::unordered_set>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const T& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_set>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			int64 dynamicSize = 0;
			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_set>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::unorderd_set>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::unordered_set>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				T elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "unorderedset"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "unorderedset"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class T, class H, class C, class A>
	struct ContainerType<UnorderedMultiSet<T, H, C, A>> : public BaseType<UnorderedSet<T, H, C, A>>
	{
		using Type = UnorderedMultiSet<T, H, C, A>;
		using ValueCat = typename GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::unordered_multiset>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const T& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_multiset>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			int64 dynamicSize = 0;
			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_multiset>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				cJSON* obj = ValueCat::ToJSON(elem, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
				++i;
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::unordered_multiset>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::unordered_multiset>]::FromJSON expected an Array."sv);
			
			achar buff[128];
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				T elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "unorderedmultiset"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "unorderedmultiset"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class K, class V, class P, class A>
	struct ContainerType<Map<K, V, P, A>> : public BaseType<Map<K, V, P, A>>
	{
		using Type = Map<K, V, P, A>;
		using KeyCat = typename GetCategoryType<K>::Type;
		using ValueCat = typename GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::map>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);
			for(const auto& [key, value] : data)
			{
				TResult<ssizet> res = KeyCat::ToStream(key, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();

				res = ValueCat::ToStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			int64 dynamicSize = 0;
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				TResult<ssizet> res = KeyCat::FromStream(key, stream);
				if(res.HasFailed())
					return res;

				size += res.GetValue();
				dynamicSize += KeyCat::StaticSize + KeyCat::GetDynamicSize(key);

				V value;
				res = ValueCat::FromStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(value);
				
				data.emplace(key, value);
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			for(const auto& [key, value] : data)
			{
				cJSON* obj = cJSON_CreateObject();
				KeyCat::ToJSON(key, obj, "key"sv);
				ValueCat::ToJSON(value, obj, "value"sv);
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::map>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::map>]::FromJSON expected an Array."sv);
			
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				EmptyResult res = KeyCat::FromJSON(key, item, "key"sv);
				if(res.HasFailed())
					return res;
				
				V value;
				res = ValueCat::FromJSON(value, item, "value"sv);
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "map"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "map"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class K, class V, class P, class A>
	struct ContainerType<MultiMap<K, V, P, A>> : public BaseType<MultiMap<K, V, P, A>>
	{
		using Type = MultiMap<K, V, P, A>;
		using KeyCat = typename GetCategoryType<K>::Type;
		using ValueCat = typename GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::multimap>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);
			for(const auto& [key, value] : data)
			{
				TResult<ssizet> res = KeyCat::ToStream(key, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();

				res = ValueCat::ToStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::multimap>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			int64 dynamicSize = 0;
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				TResult<ssizet> res = KeyCat::FromStream(key, stream);
				if(res.HasFailed())
					return res;

				size += res.GetValue();
				dynamicSize += KeyCat::StaticSize + KeyCat::GetDynamicSize(key);

				V value;
				res = ValueCat::FromStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(value);
				
				data.emplace(key, value);
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			for(const auto& [key, value] : data)
			{
				cJSON* obj = cJSON_CreateObject();
				KeyCat::ToJSON(key, obj, "key"sv);
				ValueCat::ToJSON(value, obj, "value"sv);
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::multimap>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::multimap>]::FromJSON expected an Array."sv);
			
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				EmptyResult res = KeyCat::FromJSON(key, item, "key"sv);
				if(res.HasFailed())
					return res;
				
				V value;
				res = ValueCat::FromJSON(value, item, "value"sv);
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "multimap"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "multimap"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class K, class V, class H, class C, class A>
	struct ContainerType<UnorderedMap<K, V, H, C, A>> : public BaseType<UnorderedMap<K, V, H, C, A>>
	{
		using Type = UnorderedMap<K, V, H, C, A>;
		using KeyCat = typename GetCategoryType<K>::Type;
		using ValueCat = typename GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::unordered_map>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);
			for(const auto& [key, value] : data)
			{
				TResult<ssizet> res = KeyCat::ToStream(key, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();

				res = ValueCat::ToStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_map>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			int64 dynamicSize = 0;
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				TResult<ssizet> res = KeyCat::FromStream(key, stream);
				if(res.HasFailed())
					return res;

				size += res.GetValue();
				dynamicSize += KeyCat::StaticSize + KeyCat::GetDynamicSize(key);

				V value;
				res = ValueCat::FromStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(value);
				
				data.emplace(key, value);
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			for(const auto& [key, value] : data)
			{
				cJSON* obj = cJSON_CreateObject();
				KeyCat::ToJSON(key, obj, "key"sv);
				ValueCat::ToJSON(value, obj, "value"sv);
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::unordered_map>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::unordered_map>]::FromJSON expected an Array."sv);
			
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				EmptyResult res = KeyCat::FromJSON(key, item, "key"sv);
				if(res.HasFailed())
					return res;
				
				V value;
				res = ValueCat::FromJSON(value, item, "value"sv);
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "unorderedmap"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "unorderedmap"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};

	template<class K, class V, class H, class C, class A>
	struct ContainerType<UnorderedMultiMap<K, V, H, C, A>> : public BaseType<UnorderedMultiMap<K, V, H, C, A>>
	{
		using Type = UnorderedMultiMap<K, V, H, C, A>;
		using KeyCat = typename GetCategoryType<K>::Type;
		using ValueCat = typename GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::unordered_multimap>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);
			for(const auto& [key, value] : data)
			{
				TResult<ssizet> res = KeyCat::ToStream(key, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();

				res = ValueCat::ToStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_multimap>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			ssizet size = 0;
			size += stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			int64 dynamicSize = 0;
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				TResult<ssizet> res = KeyCat::FromStream(key, stream);
				if(res.HasFailed())
					return res;

				size += res.GetValue();
				dynamicSize += KeyCat::StaticSize + KeyCat::GetDynamicSize(key);

				V value;
				res = ValueCat::FromStream(value, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(value);
				
				data.emplace(key, value);
			}
			auto expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static cJSON* ToJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			for(const auto& [key, value] : data)
			{
				cJSON* obj = cJSON_CreateObject();
				KeyCat::ToJSON(key, obj, "key"sv);
				ValueCat::ToJSON(value, obj, "value"sv);
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<std::unordered_multimap>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<std::unordered_multimap>]::FromJSON expected an Array."sv);
			
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				EmptyResult res = KeyCat::FromJSON(key, item, "key"sv);
				if(res.HasFailed())
					return res;
				
				V value;
				res = ValueCat::FromJSON(value, item, "value"sv);
				if(res.HasFailed())
					return res;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			cJSON* json = ToJSON(data, "unorderedmultimap"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			EmptyResult res = FromJSON(data, json, "unorderedmultimap"sv);
			cJSON_Delete(json);
			return res;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			/* No-op */
		}
	};
}

#endif /* CORE_REFLECTION_CONTAINERTYPE_H */