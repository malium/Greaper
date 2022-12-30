/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_REFL_HALF_H
#define MATH_REFL_HALF_H 1

#include <Core/Reflection/PlainType.h>
#include <Math/Base/Half.h>

namespace greaper::refl
{
	template<>
	struct PlainType<math::Half> : public BaseType<math::Half>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain;
		static TResult<ssizet> ToStream(const math::Half& data, IStream& stream)
		{ 
			ssizet size = stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<Half>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIiPTR ".", sizeof(data), size));
		}
		static TResult<ssizet> FromStream(math::Half& data, IStream& stream)
		{ 
			ssizet size = stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::PlainType<Half>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIiPTR ".", sizeof(data), size));
		}
		static TResult<std::pair<math::Half, ssizet>> CreateFromStream(IStream& stream)
		{
			math::Half elem;
			TResult<ssizet> res = FromStream(elem, stream);
			if (res.HasFailed())
				return Result::CopyFailure<std::pair<math::Half, ssizet>, ssizet>(res);
			return Result::CreateSuccess(std::make_pair(elem, res.GetValue()));
		}
		static SPtr<cJSON> CreateJSON(const math::Half& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}
		static cJSON* ToJSON(const math::Half& data, cJSON* obj, StringView name)
		{
			return PlainType<float>::ToJSON(data.Get(), obj, name);
		}
		static EmptyResult FromJSON(math::Half& data, cJSON* json, StringView name)
		{
			float temp;
			EmptyResult res = PlainType<float>::FromJSON(temp, json, name);
			if(res.HasFailed())
				return res;
			data.Set(temp);
			return Result::CreateSuccess();
		}
		static TResult<math::Half> CreateFromJSON(cJSON* json, StringView name)
		{
			math::Half elem;
			EmptyResult res = FromJSON(elem, json, name);
			if (res.HasFailed())
				return Result::CopyFailure<math::Half>(res);
			return Result::CreateSuccess(elem);
		}
		NODISCARD static String ToString(const math::Half& data)
		{
			return PlainType<float>::ToString(data.Get());
		}
		static EmptyResult FromString(const String& str, math::Half& data)
		{
			float temp;
			EmptyResult res = PlainType<float>::FromString(str, temp);
			if(res.HasFailed())
				return res;
			data.Set(temp);
			return Result::CreateSuccess();
		}
		static TResult<math::Half> CreateFromString(const String& str)
		{
			math::Half elem;
			EmptyResult res = FromString(str, elem);
			if (res.HasFailed())
				return Result::CopyFailure<math::Half>(res);
			return Result::CreateSuccess(elem);
		}
		NODISCARD static int64 GetDynamicSize(UNUSED const math::Half& data)
		{
			return 0ll; 
		}

		NODISCARD static sizet GetArraySize(UNUSED const math::Half& data)
		{
			Break("[refl::PlainType<Half>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED math::Half& data, UNUSED sizet size)
		{
			Break("[refl::PlainType<Half>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const math::Half& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::PlainType<Half>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED math::Half& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::PlainType<Half>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};
}

#endif /* MATH_REFL_HALF_H */