/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_COMPLEXTYPE_H
#define CORE_REFLECTION_COMPLEXTYPE_H 1

#include "ContainerType.h"
#include "IField.h"

namespace greaper::refl
{
	template<class T>
	struct ComplexType : public BaseType<T>
	{
		static const Vector<SPtr<IField>> Fields;

		using ArrayValueType = int32;

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static inline constexpr ssizet StaticSize = 0;

		static TResult<ssizet> ToStream(const T& data, IStream& stream)
		{
			ssizet totalSize = 0;
			for (const auto& field : Fields)
			{
				auto val = field->GetValue(&data);
				auto res = field->ToStream(val, stream);
				if (res.HasFailed())
					return res;

				ssizet size = res.GetValue();
				ssizet expectedSize = field->GetStaticSize() + field->GetDynamicSize(val);
				totalSize += size;
				if (size != expectedSize)
					return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<T>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIiPTR" obtained:%" PRIiPTR ".", expectedSize, size));
			}
			return Result::CreateSuccess(totalSize);
		}

		static TResult<ssizet> FromStream(T& data, IStream& stream)
		{
			ssizet totalSize = 0;
			for (const auto& field : Fields)
			{
				auto res = field->CreateFromStream(stream);
				if (res.HasFailed())
					return res;

				auto lck = Lock<RecursiveMutex>(*std::get<2>(res.GetValue()), AdoptLock{});

				ssizet size = std::get<1>(res.GetValue());
				ssizet expectedSize = field->GetStaticSize() + field->GetDynamicSize(val);
				totalSize += size;
				if (size != expectedSize)
					return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<T>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIiPTR" obtained:%" PRIiPTR ".", expectedSize, size));
				
				field->SetValue(&data, std::get<0>(res.GetValue()));
			}
			return Result::CreateSuccess(totalSize);
		}

		static SPtr<cJSON> CreateJSON(const T& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const T& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			for (const auto& field : Fields)
			{
				auto value = field->GetValue(&data);
				field->ToJSON(value, json);
			}
			return obj;
		}

		static EmptyResult FromJSON(T& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if (item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<T>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			for (const auto& field : Fields)
			{
				auto res = field->CreateFromJSON(json);
				if (res.HasFailed())
					return res;

				field->SetValue(&data, std::get<0>(res.GetValue()));
				std::get<1>(res.GetValue())->unlock();
			}
			return Result::CreateSuccess();
		}

		static String ToString(const T& data)
		{
			auto json = CreateJSON(data, TypeInfo_t<T>::Name);
			auto str = SPtr<char>(cJSON_Print(json.get()));
			return String{ str.get() };
		}

		static EmptyResult FromString(const String& str, T& data)
		{
			auto json = SPtr<cJSON>(cJSON_Parse(str.c_str()), cJSON_Delete);
			return FromJSON(data, json, TypeInfo_t<T>::Name);
		}

		NODISCARD static int64 GetDynamicSize(const T& data)
		{
			int64 size = 0;
			for (const auto& field : Fields)
			{
				auto value = field->GetCValue();
				size += field->GetStaticSize() + field->GetDynamicSize(value);
			}
			return size;
		}

		NODISCARD static sizet GetArraySize(UNUSED const T& data)
		{
			Break("[refl::ComplexType<T>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED T& data, UNUSED sizet size)
		{
			Break("[refl::ComplexType<T>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const ArrayValueType& GetArrayValue(UNUSED const T& data, UNUSED sizet index)
		{
			static constexpr ArrayValueType dummy = 0;
			Break("[refl::ComplexType<T>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED T& data, UNUSED const ArrayValueType& value, UNUSED sizet index)
		{
			Break("[refl::ComplexType<T>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};

	constexpr auto x = ComplexType<float>::StaticSize;
}

#endif /* CORE_REFLECTION_COMPLEXTYPE_H */
