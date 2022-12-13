/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_CONTAINERTYPE_H
#define CORE_REFLECTION_CONTAINERTYPE_H 1

#include "BaseType.h"

namespace greaper::refl
{
	template<>
	struct ContainerType<String> : BaseType<String>
	{
		using Type = String;

		static inline constexpr ssizet StaticSize = sizeof(sizet);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			sizet elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));
			size += stream.Write(data.data(), elementCount * sizeof(Type::value_type));
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			sizet elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));
			const auto dynSize = elementCount * sizeof(Type::value_type);
			SetDynamicSize(data, dynSize);
			size += stream.Read(data.data(), dynSize);
			return size;
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

		NODISCARD static ssizet GetDynamicSize(const Type& data)
		{
			return data.size() * sizeof(Type::value_type);
		}

		static void SetDynamicSize(Type& data, ssizet size)
		{
			data.resize(size / sizeof(Type::value_type), Type::value_type(0));
		}
	};

	template<>
	struct ContainerType<WString> : BaseType<WString>
	{
		using Type = WString;

		static inline constexpr ssizet StaticSize = sizeof(sizet);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static ssizet ToStream(const Type& data, IStream& stream)
		{
			sizet elementCount = data.size();
			auto size = stream.Write(&elementCount, sizeof(elementCount));
			size += stream.Write(data.data(), elementCount * sizeof(Type::value_type));
			return size;
		}

		static ssizet FromStream(Type& data, IStream& stream)
		{
			sizet elementCount;
			auto size = stream.Read(&elementCount, sizeof(elementCount));
			const auto dynSize = elementCount * sizeof(Type::value_type);
			SetDynamicSize(data, dynSize);
			size += stream.Read(data.data(), dynSize);
			return size;
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

		NODISCARD static ssizet GetDynamicSize(const Type& data)
		{
			return data.size() * sizeof(Type::value_type);
		}

		static void SetDynamicSize(Type& data, ssizet size)
		{
			data.resize(size / sizeof(Type::value_type), Type::value_type(0));
		}
	};

	template<class T, sizet N>
	struct ContainerType<std::array<T, N>> : BaseType<std::array<T, N>>
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

		static String ToString(const Type& data)
		{
			// Todo add JSON or YAML
			return {};
		}

		static bool FromString(const String& str, Type& data)
		{
			// Todo add JSON or YAML
			return false;
		}

		NODISCARD static ssizet GetDynamicSize(UNUSED const Type& data)
		{
			return 0ll;
		}

		static void SetDynamicSize(UNUSED Type& data, UNUSED ssizet size)
		{
			/* No-op */
		}
	};
}

#endif /* CORE_REFLECTION_CONTAINERTYPE_H */