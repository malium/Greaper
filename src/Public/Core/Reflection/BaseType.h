/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_BASETYPE_H
#define CORE_REFLECTION_BASETYPE_H 1

#include "../CorePrerequisites.h"
#include <type_traits>
#include "../Stream.h"
#include "../StringUtils.h"
#include "../Enumeration.h"
#define CJSON_IMPORT_SYMBOLS
#include <External/cJSON/cJSON.h>

ENUMERATION(TypeCategory, Plain, Container, Complex, Data);

namespace greaper::refl
{
	template<class T>
	struct BaseType
	{
		static_assert(ReflectedTypeToID<T>::ID != RTI_Unknown, "refl::BaseType instantiated with an Unknown TypeID.");

		static inline constexpr uint64 ID = ReflectedTypeToID<T>::ID;
		
		static inline constexpr ssizet StaticSize = sizeof(T);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::COUNT;

		static ssizet ToStream(UNUSED const T& data, UNUSED IStream& stream)
		{
			Break("Trying to use the generic refl::BaseType!");
			return -1ll;
		}

		static ssizet FromStream(UNUSED T& data, UNUSED IStream& stream)
		{
			Break("Trying to use the generic refl::BaseType!");
			return -1ll;
		}

		static cJSON* ToJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}

		static cJSON* ToJSON(UNUSED const T& data, UNUSED cJSON* obj, UNUSED StringView name)
		{
			Break("Trying to use the generic refl::BaseType!");
			return nullptr;
		}
		
		static bool FromJSON(UNUSED T& data, UNUSED cJSON* json, UNUSED StringView name)
		{
			Break("Trying to use the generic refl::BaseType!");
			return false;
		}

		NODISCARD static String ToString(UNUSED const T& data)
		{
			Break("Trying to use the generic refl::BaseType!");
			return String{};
		}

		static bool FromString(UNUSED const String& str, UNUSED T& data)
		{
			Break("Trying to use the generic refl::BaseType!");
			return false;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const T& data)
		{
			Break("Trying to use the generic refl::BaseType!");
			return 0ll;
		}

		static void SetDynamicSize(UNUSED T& data, UNUSED int64 size)
		{
			Break("Trying to use the generic refl::BaseType!");
		}
	};

	template<class T> class PlainType;
	template<class T> class ContainerType;
	template<class T> class ComplexType;
	template<class T> class DataType;

	template<class T>
	struct GetCategoryType
	{
		using Type =
			std::conditional_t<std::is_base_of_v<BaseType<T>, PlainType<T>>, PlainType<T>, 
			std::conditional_t<std::is_base_of_v<BaseType<T>, ContainerType<T>>, ContainerType<T>, 
			std::conditional_t<std::is_base_of_v<BaseType<T>, ComplexType<T>>, ComplexType<T>,
			std::conditional_t<std::is_base_of_v<BaseType<T>, DataType<T>>, DataType<T>, 
			void>>>>;
	};
}

#endif /* CORE_REFLECTION_BASETYPE_H */