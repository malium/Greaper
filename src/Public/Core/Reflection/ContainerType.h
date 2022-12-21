/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_CONTAINERTYPE_H
#define CORE_REFLECTION_CONTAINERTYPE_H 1

#include "BaseType.h"
#include <inttypes.h>

namespace greaper::refl
{
	template<>
	struct ContainerType<String> : public BaseType<String>
	{
		using Type = String;
		using ArrayValueType = typename Type::value_type;

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			int64 elementCount = data.size();
			size += stream.Write(&elementCount, sizeof(elementCount));
			int64 dynamicSize = GetDynamicSize(data);
			size += stream.Write(data.data(), dynamicSize);
			ssizet expectedSize = dynamicSize + StaticSize;
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
			ssizet expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<String>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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
			return data.size() * sizeof(ArrayValueType);
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			data.resize(size);
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
				return data[index];
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
				data[index] = value;
		}
	};

	template<>
	struct ContainerType<WString> : public BaseType<WString>
	{
		using Type = WString;
		using ArrayValueType = typename Type::value_type;

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			int64 elementCount = data.size();
			size += stream.Write(&elementCount, sizeof(elementCount));
			int64 dynamicSize = GetDynamicSize(data);
			size += stream.Write(data.data(), dynamicSize);
			ssizet expectedSize = dynamicSize + StaticSize;
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
			ssizet expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<WString>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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
			return data.size() * sizeof(ArrayValueType);
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			data.resize(size);
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
				return data[index];
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
				data[index] = value;
		}
	};

	template<class T, sizet N>
	struct ContainerType<std::array<T, N>> : public BaseType<std::array<T, N>>
	{
		using Type = std::array<T, N>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::array>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = ValueCat::StaticSize * N;

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			ssizet dynamicSize = GetDynamicSize(data);
			if constexpr(std::is_same_v<ValueCat, PlainType<ArrayValueType>>)
			{
				size += stream.Write(data.data(), StaticSize);
			}
			else
			{
				for(const ArrayValueType& elem : data)
				{
					TResult<ssizet> res = ValueCat::ToStream(elem, stream);
					if(res.HasFailed())
						return res;

					size += res.GetValue();
				}
			}
			ssizet expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::array>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			ssizet size = 0;
			ssizet dynamicSize = 0;
			if constexpr(std::is_same_v<ValueCat, PlainType<ArrayValueType>>)
			{
				size += stream.Read(data.data(), StaticSize);
			}
			else
			{
				for(ArrayValueType& elem : data)
				{
					TResult<ssizet> res = ValueCat::FromStream(elem, stream);
					if(res.HasFailed())
						return res;

					dynamicSize += ValueCat::GetDynamicSize(elem);
					size += res.GetValue();
				}
			}
			ssizet expectedSize = dynamicSize + StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::array>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for (const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{ buff });
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
			SPtr<cJSON> json = CreateJSON(data, "array"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "array"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<ArrayValueType>>)
				return 0ll;
			
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(UNUSED const Type& data)
		{
			return N;
		}

		static void SetArraySize(UNUSED Type& data, UNUSED sizet size)
		{
			/* No-op */
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
				return data[index];
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
				data[index] = value;
		}
	};

	template<class T, class A>
	struct ContainerType<Vector<T, A>> : public BaseType<Vector<T, A>>
	{
		using Type = Vector<T, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::vector>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			if constexpr(std::is_same_v<ValueCat, PlainType<ArrayValueType>>)
			{
				size += stream.Write(data.data(), dynamicSize);
			}
			else
			{
				for(const ArrayValueType& elem : data)
				{
					TResult<ssizet> res = ValueCat::ToStream(elem, stream);
					if(res.HasFailed())
						return res;
					
					size += res.GetValue();
				}
			}
			ssizet expectedSize = dynamicSize + StaticSize; 
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
			if constexpr(std::is_same_v<ValueCat, PlainType<ArrayValueType>>)
			{
				dynamicSize = elementCount * sizeof(ArrayValueType);
				size += stream.Read(data.data(), dynamicSize);
			}
			else
			{
				for(auto it = data.begin(); it != data.end(); ++it)
				{
					ArrayValueType elem;
					TResult<ssizet> res = ValueCat::FromStream(elem, stream);
					if(res.HasFailed())
						return res;
					
					(*it) = elem;
					dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
					size += res.GetValue();
				}
			}
			ssizet expectedSize = dynamicSize + StaticSize; 
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::vector>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{ buff });
				cJSON_AddItemToArray(arr, obj);
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
			SPtr<cJSON> json = CreateJSON(data, "vector"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "vector"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<ArrayValueType>>)
			{
				return sizeof(ArrayValueType) * data.size();
			}
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			data.resize(size);
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
				return data[index];
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
				data[index] = value;
		}
	};

	template<class T, class A>
	struct ContainerType<List<T, A>> : public BaseType<List<T, A>>
	{
		using Type = List<T, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::list>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const ArrayValueType& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
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
				ArrayValueType elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				(*it) = elem;
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::list>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
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
			sizet i = 0;
			for (auto& elem : data)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i);
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;

				i++;
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "list"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "list"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}
		static void SetArraySize(Type& data, sizet size)
		{
			data.resize(size);
		}
		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data)) 
			{
				sizet i = 0;
				for(auto it = data.begin(); it != data.end(); ++it, ++i)
				{
					if(i == index)
						return *it;
				}
			}
			return ArrayValueType{};
		}
		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
			{
				sizet i = 0;
				for(auto it = data.begin(); it != data.end(); ++it, ++i)
				{
					if(i == index)
					{
						(*it) = value;
						break;
					}
				}	
			}
		}
	};

	template<class T, class A>
	struct ContainerType<Deque<T, A>> : public BaseType<Deque<T, A>>
	{
		using Type = Deque<T, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::deque>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const ArrayValueType& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
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
				ArrayValueType elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				(*it) = elem;
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::deque>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
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
			SPtr<cJSON> json = CreateJSON(data, "queue"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "queue"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			data.resize(size);
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
				return data[index];
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
				data[index] = value;
		}
	};

	template<class T, class C, class A>
	struct ContainerType<Set<T, C, A>> : public BaseType<Set<T, C, A>>
	{
		using Type = Set<T, C, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::set>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const ArrayValueType& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
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
				ArrayValueType elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::set>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
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
				ArrayValueType elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "set"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "set"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			while(data.size() < size)
				data.emplace(ArrayValueType{});
			while(data.size() > size)
				data.erase(data.end());
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index));
				data.emplace(value);
			}
		}
	};

	template<class T, class C, class A>
	struct ContainerType<MultiSet<T, C, A>> : public BaseType<MultiSet<T, C, A>>
	{
		using Type = MultiSet<T, C, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::multiset>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const ArrayValueType& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
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
				ArrayValueType elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::multiset>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
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
				ArrayValueType elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "multiset"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "multiset"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			while(data.size() < size)
				data.emplace(ArrayValueType{});
			while(data.size() > size)
				data.erase(data.end());
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if (index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if (index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index));
				data.emplace(value);
			}
		}
	};

	template<class T, class H, class C, class A>
	struct ContainerType<UnorderedSet<T, H, C, A>> : public BaseType<UnorderedSet<T, H, C, A>>
	{
		using Type = UnorderedSet<T, H, C, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::unordered_set>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const ArrayValueType& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
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
				ArrayValueType elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_set>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
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
				ArrayValueType elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "unorderedset"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String {jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "unorderedset"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			while(data.size() < size)
				data.emplace(ArrayValueType{});
			while(data.size() > size)
				data.erase(data.end());
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if (index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if (index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index));
				data.emplace(value);
			}
		}
	};

	template<class T, class H, class C, class A>
	struct ContainerType<UnorderedMultiSet<T, H, C, A>> : public BaseType<UnorderedSet<T, H, C, A>>
	{
		using Type = UnorderedMultiSet<T, H, C, A>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<std::unordered_multiset>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			ssizet size = 0;
			size += stream.Write(&elementCount, sizeof(elementCount));

			auto dynamicSize = GetDynamicSize(data);

			for(const ArrayValueType& elem : data)
			{
				TResult<ssizet> res = ValueCat::ToStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
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
				ArrayValueType elem;
				TResult<ssizet> res = ValueCat::FromStream(elem, stream);
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
				dynamicSize += ValueCat::StaticSize + ValueCat::GetDynamicSize(elem);
				size += res.GetValue();
			}
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::unordered_multiset>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_AddArrayToObject(json, name.data());
			achar buff[128];
			sizet i = 0;
			for(const auto& elem : data)
			{
				snprintf(buff, ArraySize(buff), "Elem_%" PRIuPTR, i++);
				cJSON* obj = cJSON_CreateObject();
				ValueCat::ToJSON(elem, obj, StringView{buff});
				cJSON_AddItemToArray(arr, obj);
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
				ArrayValueType elem;
				EmptyResult res = ValueCat::FromJSON(elem, item, StringView{buff});
				if(res.HasFailed())
					return res;
				
				data.emplace(elem);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "unorderedmultiset"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{jsonStr.get()};
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "unorderedmultiset"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& e : data)
				size += ValueCat::StaticSize + ValueCat::GetDynamicSize(e);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(Type& data, sizet size)
		{
			while(data.size() < size)
				data.emplace(ArrayValueType{});
			while(data.size() > size)
				data.erase(data.end());
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if (index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if (index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index));
				data.emplace(value);
			}
		}
	};

	template<class K, class V, class P, class A>
	struct ContainerType<Map<K, V, P, A>> : public BaseType<Map<K, V, P, A>>
	{
		using Type = Map<K, V, P, A>;
		using ArrayValueType = typename Type::value_type;
		using KeyCat = typename TypeInfo<K>::Type;
		using ValueCat = typename TypeInfo<V>::Type;

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
			ssizet expectedSize = StaticSize + dynamicSize;
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
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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

				data.emplace(key, value);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "map"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String {jsonStr.get()};
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "map"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(UNUSED Type& data, UNUSED sizet size)
		{
			/* No-op */
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if(index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if(index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index).first);
				data.emplace(value);
			}
		}
	};

	template<class K, class V, class P, class A>
	struct ContainerType<MultiMap<K, V, P, A>> : public BaseType<MultiMap<K, V, P, A>>
	{
		using Type = MultiMap<K, V, P, A>;
		using ArrayValueType = typename Type::value_type;
		using KeyCat = typename TypeInfo<K>::Type;
		using ValueCat = typename TypeInfo<V>::Type;

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
			ssizet expectedSize = StaticSize + dynamicSize;
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
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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

				data.emplace(key, value);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "multimap"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "multimap"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(UNUSED Type& data, UNUSED sizet size)
		{
			/* No-op */
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if (index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if (index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index).first);
				data.emplace(value);
			}
		}
	};

	template<class K, class V, class H, class C, class A>
	struct ContainerType<UnorderedMap<K, V, H, C, A>> : public BaseType<UnorderedMap<K, V, H, C, A>>
	{
		using Type = UnorderedMap<K, V, H, C, A>;
		using ArrayValueType = typename Type::value_type;
		using KeyCat = typename TypeInfo<K>::Type;
		using ValueCat = typename TypeInfo<V>::Type;

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
			ssizet expectedSize = StaticSize + dynamicSize;
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
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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

				data.emplace(key, value);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "unorderedmap"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "unorderedmap"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(UNUSED Type& data, UNUSED sizet size)
		{
			/* No-op */
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if (index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if (index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index).first);
				data.emplace(value);
			}
		}
	};

	template<class K, class V, class H, class C, class A>
	struct ContainerType<UnorderedMultiMap<K, V, H, C, A>> : public BaseType<UnorderedMultiMap<K, V, H, C, A>>
	{
		using Type = UnorderedMultiMap<K, V, H, C, A>;
		using ArrayValueType = typename Type::value_type;
		using KeyCat = typename TypeInfo<K>::Type;
		using ValueCat = typename TypeInfo<V>::Type;

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
			ssizet expectedSize = StaticSize + dynamicSize;
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
			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<std::map>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
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

				data.emplace(key, value);
			}
			return Result::CreateSuccess();
		}

		static String ToString(const Type& data)
		{
			SPtr<cJSON> json = CreateJSON(data, "unorderedmultimap"sv);
			SPtr<char> jsonStr = SPtr<char>(cJSON_Print(json.get()));
			return String{ jsonStr.get() };
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			SPtr<cJSON> json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json.get(), "unorderedmultimap"sv);
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			int64 size = 0;
			for(const auto& [key, value] : data)
				size += KeyCat::StaticSize + ValueCat::StaticSize + KeyCat::GetDynamicSize(key) + ValueCat::GetDynamicSize(value);
			return size;
		}

		NODISCARD static sizet GetArraySize(const Type& data)
		{
			return data.size();
		}

		static void SetArraySize(UNUSED Type& data, UNUSED sizet size)
		{
			/* No-op */
		}

		NODISCARD static const ArrayValueType& GetArrayValue(const Type& data, sizet index)
		{
			if (index < GetArraySize(data))
			{
				sizet i = 0;
				for (const auto& elem : data)
				{
					if (i == index)
						return elem;
					++i;
				}
			}
			return ArrayValueType{};
		}

		static void SetArrayValue(Type& data, const ArrayValueType& value, sizet index)
		{
			if (index < GetArraySize(data))
			{
				data.erase(GetArrayValue(data, index).first);
				data.emplace(value);
			}
		}
	};
}

#endif /* CORE_REFLECTION_CONTAINERTYPE_H */