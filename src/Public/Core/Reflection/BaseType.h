/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_BASETYPE_H
#define CORE_REFLECTION_BASETYPE_H 1

#include "../CorePrerequisites.h"
#include "../Stream.h"
#include "../StringUtils.h"
#include "../Enumeration.h"
#include "../Result.h"
#define CJSON_IMPORT_SYMBOLS
#include <External/cJSON/cJSON.h>

ENUMERATION(TypeCategory, Plain, Container, Complex);

namespace greaper::refl
{
	template<class T>
	struct BaseType
	{
		static_assert(TypeInfo<T>::ID != RTI_Unknown, "[refl::BaseType<T>] instantiated with an Unknown TypeID.");
		static_assert(!std::is_same_v<TypeInfo<T>::Type, void>, "[refl::BaseType<T>] instantiated with a void type.");

		static inline constexpr uint64 ID = TypeInfo<T>::ID;
		
		static inline constexpr ssizet StaticSize = sizeof(T);

		static inline constexpr TypeCategory_t Category = TypeCategory_t::COUNT;

		static TResult<ssizet> ToStream(UNUSED const T& data, UNUSED IStream& stream)
		{
			return Result::CreateFailure<ssizet>("[refl::BaseType<T>]::ToStream Trying to use the generic refl::BaseType!"sv);
		}

		static TResult<ssizet> FromStream(UNUSED T& data, UNUSED IStream& stream)
		{
			return Result::CreateFailure<ssizet>("[refl::BaseType<T>]::FromStream Trying to use the generic refl::BaseType!"sv);
		}

		static SPtr<cJSON> ToJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(UNUSED const T& data, UNUSED cJSON* obj, UNUSED StringView name)
		{
			Break("[refl::BaseType<T>]::ToJSON Trying to use the generic refl::BaseType!");
			return nullptr;
		}
		
		static EmptyResult FromJSON(UNUSED T& data, UNUSED cJSON* json, UNUSED StringView name)
		{
			return Result::CreateFailure("[refl::BaseType<T>]::FromJSON Trying to use the generic refl::BaseType!"sv);
		}

		NODISCARD static String ToString(UNUSED const T& data)
		{
			Break("[refl::BaseType<T>]::ToString Trying to use the generic refl::BaseType!");
			return String{};
		}

		static EmptyResult FromString(UNUSED const String& str, UNUSED T& data)
		{
			return Result::CreateFailure("[refl::BaseType<T>]::FromString Trying to use the generic refl::BaseType!"sv);
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const T& data)
		{
			Break("[refl::BaseType<T>]::GetDynamicSize Trying to use the generic refl::BaseType!");
			return 0ll;
		}

		static void SetDynamicSize(UNUSED T& data, UNUSED int64 size)
		{
			Break("[refl::BaseType<T>]::SetDynamicSize Trying to use the generic refl::BaseType!");
		}
	};
}

#endif /* CORE_REFLECTION_BASETYPE_H */