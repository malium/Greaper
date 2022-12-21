/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "PlainType.h"
#include "../Uuid.h"

namespace greaper::refl
	{
		template<>
		struct PlainType<Uuid> : public BaseType<Uuid>
		{
			static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain; 
			
			static TResult<ssizet> ToStream(const Uuid& data, IStream& stream)
			{ 
				return Result::CreateSuccess(stream.Write(&data, sizeof(data))); 
			}

			static TResult<ssizet> FromStream(Uuid& data, IStream& stream)
			{ 
				return Result::CreateSuccess(stream.Read(&data, sizeof(data)));
			}

			static SPtr<cJSON> CreateJSON(const Uuid& data, StringView name)
			{
				cJSON* obj = cJSON_CreateObject();
				ToJSON(data, obj, name);
				return SPtr<cJSON>(obj, cJSON_Delete);
			}

			static cJSON* ToJSON(const Uuid& data, cJSON* obj, StringView name)
			{
				auto str = data.ToString();
				return cJSON_AddStringToObject(obj, name.data(), str.c_str());
			}
			
			static EmptyResult FromJSON(Uuid& data, cJSON* json, StringView name)
			{
				cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
				if(item == nullptr)
					return Result::CreateFailure(Format("[refl::PlainType<Uuid>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));			if(cJSON_IsString(item))
				{
					data = Uuid(cJSON_GetStringValue(item));
					return Result::CreateSuccess();
				}
				return Result::CreateFailure("[refl::PlainType<Uuid>]::FromJSON Couldn't obtain the value from the json, expected: cJSON_IsString."sv);
			}

			NODISCARD static String ToString(const Uuid& data)
			{
				return data.ToString();
			}

			static EmptyResult FromString(const String& str, Uuid& data)
			{
				data.FromString(str);
				return Result::CreateSuccess();
			}

			NODISCARD static int64 GetDynamicSize(UNUSED const Uuid& data)
			{
				return 0ll; 
			}

			NODISCARD static sizet GetArraySize(UNUSED const Uuid& data)
			{
				Break("[refl::PlainType<Uuid>]::GetArraySize Trying to use a PlainType for array operations!");
				return 0ll;
			}

			static void SetArraySize(UNUSED Uuid& data, UNUSED sizet size)
			{
				Break("[refl::PlainType<Uuid>]::SetArraySize Trying to use a PlainType for array operations!");
			}

			NODISCARD static const int32& GetArrayValue(UNUSED const Uuid& data, UNUSED sizet index)
			{
				static constexpr int32 dummy = 0;
				Break("[refl::PlainType<Uuid>]::GetArrayValue Trying to use a PlainType for array operations!");
				return dummy;
			}

			static void SetArrayValue(UNUSED Uuid& data, UNUSED const int32& value, UNUSED sizet index)
			{
				Break("[refl::PlainType<Uuid>]::SetArrayValue Trying to use a PlainType for array operations!");
			}
		};
	}