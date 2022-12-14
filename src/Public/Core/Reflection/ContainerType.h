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

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			auto dynamicSize = GetDynamicSize(data);
			auto size = stream.Write(&dynamicSize, sizeof(dynamicSize));
			size += stream.Write(data.data(), dynamicSize);
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 dynamicSize;
			auto size = stream.Read(&dynamicSize, sizeof(dynamicSize));
			SetDynamicSize(data, dynamicSize);
			size += stream.Read(data.data(), dynamicSize);
			return size;
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
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(cJSON_IsString(item))
			{
				data.assign(cJSON_GetStringValue(item));
				return true;
			}
			return false;
		}

		static String ToString(const Type& data)
		{
			return data;
		}

		static bool FromString(const String& str, Type& data)
		{
			data.assign(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			return data.size() * sizeof(Type::value_type);
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			data.resize(size / sizeof(Type::value_type), Type::value_type(0));
		}
	};

	template<>
	struct ContainerType<WString> : public BaseType<WString>
	{
		using Type = WString;

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 dynamicSize = GetDynamicSize(data);
			auto size = stream.Write(&dynamicSize, sizeof(dynamicSize));
			size += stream.Write(data.data(), dynamicSize);
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 dynamicSize;
			auto size = stream.Read(&dynamicSize, sizeof(dynamicSize));
			SetDynamicSize(data, dynamicSize);
			size += stream.Read(data.data(), dynamicSize);
			return size;
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
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(cJSON_IsString(item))
			{
				// TODO: Convert from UTF8 instead of ANSI
				data = StringUtils::ToWIDE(cJSON_GetStringValue(item));
				return true;
			}
			return false;
		}

		static String ToString(const Type& data)
		{
			return StringUtils::FromWIDE(data);
		}

		static bool FromString(const String& str, Type& data)
		{
			data = StringUtils::ToWIDE(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			return data.size() * sizeof(Type::value_type);
		}

		static void SetDynamicSize(Type& data, int64 size)
		{
			data.resize(size / sizeof(Type::value_type), Type::value_type(0));
		}
	};

	template<class T, sizet N>
	struct ContainerType<std::array<T, N>> : public BaseType<std::array<T, N>>
	{
		using Type = std::array<T, N>;
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = ValueCat::StaticSize * N;

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				return stream.Write(data.data(), StaticSize);
			}
			else
			{
				ssizet size = 0;
				for(const T& elem : data)
					size += ValueCat::ToStream(elem, stream);
				return size;
			}
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				return stream.Read(data.data(), StaticSize);
			}
			else
			{
				ssizet size = 0;
				for(T& elem : data)
					size += ValueCat::FromStream(elem, stream);
				return size;
			}
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr) || cJSON_GetArraySize(arr) != N)
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				bool ok = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "array"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "array"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				size += stream.Write(data.data(), elementCount * sizeof(T));
				return size;
			}
			else
			{
				for(const T& elem : data)
					size += ValueCat::ToStream(elem, stream);
				return size;
			}
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			data.resize(elementCount);
			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				size += stream.Read(&data.data(), elementCount * sizeof(T));
				return size;
			}
			else
			{
				for(auto it = data.begin(); it != data.end(); ++it)
				{
					T elem;
					size += ValueCat::FromStream(elem, stream);
					(*it) = elem;
				}
				return size;
			}
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			data.resize(N);
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				bool ok = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "vector"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "vector"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				size += stream.Write(data.data(), elementCount * sizeof(T));
				return size;
			}
			else
			{
				for(const T& elem : data)
					size += ValueCat::ToStream(elem, stream);
				return size;
			}
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			data.resize(elementCount);
			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				size += stream.Read(&data.data(), elementCount * sizeof(T));
				return size;
			}
			else
			{
				for(auto it = data.begin(); it != data.end(); ++it)
				{
					T elem;
					size += ValueCat::FromStream(elem, stream);
					(*it) = elem;
				}
				return size;
			}
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			data.resize(N);
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				bool ok = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "list"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "list"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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
	struct ContainerType<Deque<T, A>> : public BaseType<Deque<T, A>>
	{
		using Type = Deque<T, A>;
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				size += stream.Write(data.data(), elementCount * sizeof(T));
				return size;
			}
			else
			{
				for(const T& elem : data)
					size += ValueCat::ToStream(elem, stream);
				return size;
			}
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			data.resize(elementCount);
			if constexpr(std::is_same_v<ValueCat, PlainType<T>)
			{
				size += stream.Read(&data.data(), elementCount * sizeof(T));
				return size;
			}
			else
			{
				for(auto it = data.begin(); it != data.end(); ++it)
				{
					T elem;
					size += ValueCat::FromStream(elem, stream);
					(*it) = elem;
				}
				return size;
			}
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			data.resize(N);
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				bool ok = ValueCat::FromJSON(data[i], item, StringView{buff});
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "queue"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "queue"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class T, class C, class A>
	struct ContainerType<Set<T, C, A>> : public BaseType<Set<T, C, A>>
	{
		using Type = Set<T, C, A>;
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const T& elem : data)
				size += ValueCat::ToStream(elem, stream);
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				size += ValueCat::FromStream(elem, stream);
				data.emplace(elem);
			}
			return size;
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				T elem;
				bool ok = ValueCat::FromJSON(elem, item, StringView{buff});
				data.emplace(elem);
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "set"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "set"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class T, class C, class A>
	struct ContainerType<MultiSet<T, C, A>> : public BaseType<Multiset<T, C, A>>
	{
		using Type = MultiSet<T, C, A>;
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const T& elem : data)
				size += ValueCat::ToStream(elem, stream);
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				size += ValueCat::FromStream(elem, stream);
				data.emplace(elem);
			}
			return size;
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				T elem;
				bool ok = ValueCat::FromJSON(elem, item, StringView{buff});
				data.emplace(elem);
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "multiset"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "multiset"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class T, class H, class C, class A>
	struct ContainerType<UnorderedSet<T, H, C, A>> : public BaseType<UnorderedSet<T, H, C, A>>
	{
		using Type = UnorderedSet<T, H, C, A>;
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const T& elem : data)
				size += ValueCat::ToStream(elem, stream);
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				size += ValueCat::FromStream(elem, stream);
				data.emplace(elem);
			}
			return size;
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				T elem;
				bool ok = ValueCat::FromJSON(elem, item, StringView{buff});
				data.emplace(elem);
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "unorderedset"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "unorderedset"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class T, class H, class C, class A>
	struct ContainerType<UnorderedMultiSet<T, H, C, A>> : public BaseType<UnorderedSet<T, H, C, A>>
	{
		using Type = UnorderedMultiSet<T, H, C, A>;
		using ValueCat = GetCategoryType<T>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const T& elem : data)
				size += ValueCat::ToStream(elem, stream);
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				T elem;
				size += ValueCat::FromStream(elem, stream);
				data.emplace(elem);
			}
			return size;
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
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				cJSON* obj = ValueCat::ToJSON(data[i], StringView{buff});
				cJSON_AddItemToArray(arr, obj);
			}
			return arr;
		}
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				snprintf(buff, ArraySize(buff), "Elem_%" PRIu64, i);
				T elem;
				bool ok = ValueCat::FromJSON(elem, item, StringView{buff});
				data.emplace(elem);
				if(!ok)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "unorderedmultiset"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "unorderedmultiset"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class K, class V, class P, class A>
	struct ContainerType<Map<K, V, P, A>> : public BaseType<Map<K, V, P, A>>
	{
		using Type = Map<K, V, P, A>;
		using KeyCat = GetCategoryType<K>::Type;
		using ValueCat = GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const auto& [key, value] : data)
			{
				size += KeyCat::ToStream(key, stream);
				size += ValueCat::ToStream(value, stream);
			}

			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				V value;
				size += KeyCat::FromStream(key, stream);
				size += ValueCat::FromStream(value, stream);
				data.emplace(key, value);
			}
			return size;
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
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				V value;
				bool keyOK = KeyCat::FromJSON(key, item, "key"sv);
				bool valueOK = ValueCat::FromJSON(value, item, "value"sv);
				data.emplace(key, value);
				if(!keyOK || !valueOK)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "map"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "map"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class K, class V, class P, class A>
	struct ContainerType<MultiMap<K, V, P, A>> : public BaseType<MultiMap<K, V, P, A>>
	{
		using Type = MultiMap<K, V, P, A>;
		using KeyCat = GetCategoryType<K>::Type;
		using ValueCat = GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const auto& [key, value] : data)
			{
				size += KeyCat::ToStream(key, stream);
				size += ValueCat::ToStream(value, stream);
			}

			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				V value;
				size += KeyCat::FromStream(key, stream);
				size += ValueCat::FromStream(value, stream);
				data.emplace(key, value);
			}
			return size;
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
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				V value;
				bool keyOK = KeyCat::FromJSON(key, item, "key"sv);
				bool valueOK = ValueCat::FromJSON(value, item, "value"sv);
				data.emplace(key, value);
				if(!keyOK || !valueOK)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "multimap"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "multimap"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class K, class V, class H, class C, class A>
	struct ContainerType<UnorderedMap<K, V, H, C, A>> : public BaseType<UnorderedMap<K, V, H, C, A>>
	{
		using Type = UnorderedMap<K, V, H, C, A>;
		using KeyCat = GetCategoryType<K>::Type;
		using ValueCat = GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const auto& [key, value] : data)
			{
				size += KeyCat::ToStream(key, stream);
				size += ValueCat::ToStream(value, stream);
			}

			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				V value;
				size += KeyCat::FromStream(key, stream);
				size += ValueCat::FromStream(value, stream);
				data.emplace(key, value);
			}
			return size;
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
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				V value;
				bool keyOK = KeyCat::FromJSON(key, item, "key"sv);
				bool valueOK = ValueCat::FromJSON(value, item, "value"sv);
				data.emplace(key, value);
				if(!keyOK || !valueOK)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "unorderedmap"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "unorderedmap"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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

	template<class K, class V, class H, class C, class A>
	struct ContainerType<UnorderedMultiMap<K, V, H, C, A>> : public BaseType<UnorderedMultiMap<K, V, H, C, A>>
	{
		using Type = UnorderedMultiMap<K, V, H, C, A>;
		using KeyCat = GetCategoryType<K>::Type;
		using ValueCat = GetCategoryType<V>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = sizeof(int64);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			int64 elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));

			for(const auto& [key, value] : data)
			{
				size += KeyCat::ToStream(key, stream);
				size += ValueCat::ToStream(value, stream);
			}

			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			int64 elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));

			data.clear();
			for(decltype(elementCount) i = 0; i < elementCount; ++i)
			{
				K key;
				V value;
				size += KeyCat::FromStream(key, stream);
				size += ValueCat::FromStream(value, stream);
				data.emplace(key, value);
			}
			return size;
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
		
		static bool FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(!cJSON_IsArray(arr))
				return false; 
			
			achar buff[128];
			bool parseOK = true;
			sizet N = cJSON_GetArraySize(arr);
			data.clear();
			for(sizet i = 0; i < N; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				K key;
				V value;
				bool keyOK = KeyCat::FromJSON(key, item, "key"sv);
				bool valueOK = ValueCat::FromJSON(value, item, "value"sv);
				data.emplace(key, value);
				if(!keyOK || !valueOK)
					parseOK = false;
			}
			return parseOK;
		}

		static String ToString(const Type& data)
		{
			cJSON* json = FromJSON(data, "unorderedmultimap"sv);
			char* jsonStr = cJSON_Print(json);
			String str{jsonStr};
			cJSON_Delete(json);
			Dealloc(jsonStr);
			return str;
		}

		static bool FromString(const String& str, Type& data)
		{
			cJSON* json = cJSON_Parse(str.c_str());
			bool ok = FromJSON(data, json, "unorderedmultimap"sv);
			cJSON_Delete(json);
			return ok;
		}

		NODISCARD static int64 GetDynamicSize(const Type& data)
		{
			if constexpr (std::is_same_v<ValueCat, PlainType<T>)
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
}

#endif /* CORE_REFLECTION_CONTAINERTYPE_H */