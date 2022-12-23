/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_REFL_MATRIX3_H
#define MATH_REFL_MATRIX3_H 1

#include "../../Core/Reflection/PlainType.h"
#include "../Matrix3.h"

namespace greaper::refl
{
	template<class T>
	struct ContainerType<math::Matrix3Real<T>> : public BaseType<math::Matrix3Real<T>>
	{
		using Type = math::Matrix3Real<T>;
		using ArrayValueType = typename Type::value_type;
		using ValueCat = typename TypeInfo<ArrayValueType>::Type;

		static_assert(!std::is_same_v<ValueCat, void>, "[refl::ContainerType<math::Matrix3Real<T>>] Trying to use a Container with not refl value_type!");

		static inline constexpr ssizet StaticSize = ValueCat::StaticSize * Type::ComponentCount;

		static inline constexpr TypeCategory_t Category = TypeCategory_t::Container;

		static TResult<ssizet> ToStream(const Type& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, StaticSize);
			ssizet expectedSize = StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<math::Matrix3Real<T>>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIiPTR " obtained:%" PRIiPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(Type& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Read(&data, StaticSize);
			ssizet expectedSize = StaticSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<math::Matrix3Real<T>>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIiPTR " obtained:%" PRIiPTR ".", expectedSize, size));
		}

		static TResult<std::pair<Type, ssizet>> CreateFromStream(IStream& stream)
		{
			Type elem;
			TResult<ssizet> res = FromStream(elem, stream);
			if (res.HasFailed())
				return Result::CopyFailure<std::pair<Type, ssizet>, ssizet>(res);
			return Result::CreateSuccess(std::make_pair(elem, res.GetValue()));
		}

		static SPtr<cJSON> CreateJSON(const Type& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const Type& data, cJSON* json, StringView name)
		{
			cJSON* arr;
			if constexpr(std::is_same_v<ArrayValueType, float>)
			{
				arr = cJSON_CreateFloatArray((float*)&data, Type::ComponentCount);
			}
			else
			{
				arr = cJSON_CreateDoubleArray((double*)&data, Type::ComponentCount);
			}
			cJSON_AddItemToObject(json, name.data(), arr);
			return arr;
		}
		
		static EmptyResult FromJSON(Type& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ContainerType<math::Matrix3Real<T>>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));
			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ContainerType<math::Matrix3Real<T>>]::FromJSON expected an Array."sv);

			sizet arrSize = cJSON_GetArraySize(arr);
			if(arrSize != Type::ComponentCount)
				return Result::CreateFailure(Format("[refl::ContainerType<math::Matrix3Real<T>>]::FromJSON expected an Array with size '" PRIuPTR "' but obtained '" PRIuPTR "'.", Type::ComponentCount, arrSize));
			
			for(sizet i = 0; i < Type::ComponentCount; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				if(!cJSON_IsNumber(item))
					return Result::CreateFailure("[refl::ContainerType<math::Matrix3Real<T>>]::FromJSON Couldn't obtain the value from json, array item was not cJSON_IsNumber."sv);
				
				data[i] = static_cast<ArrayValueType>(cJSON_GetNumberValue(item));
			}
			return Result::CreateSuccess();
		}

		static TResult<Type> CreateFromJSON(cJSON* json, StringView name)
		{
			Type elem;
			EmptyResult res = FromJSON(elem, json, name);
			if (res.HasFailed())
				return Result::CopyFailure<Type>(res);
			return Result::CreateSuccess(elem);
		}

		static String ToString(const Type& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, Type& data)
		{
			data.FromString(str);
			return Result::CreateSuccess();
		}

		static TResult<Type> CreateFromString(const String& str)
		{
			Type elem;
			EmptyResult res = FromString(str, elem);
			if (res.HasFailed())
				return Result::CopyFailure<Type>(res);
			return Result::CreateSuccess(elem);
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const Type& data)
		{
			return 0ll;
		}

		NODISCARD static sizet GetArraySize(UNUSED const Type& data)
		{
			return Type::ComponentCount;
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
}

/*namespace greaper::refl
{
	template<class T> 
	struct ComplexType<math::Matrix3Real<T>> : public BaseType<math::Matrix3Real<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		static TResult<ssizet> ToStream(const math::Matrix3Real<T>& data, IStream& stream)
		{
			ssizet size = 0;
			size += stream.Write(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Matrix3Real>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static TResult<ssizet> FromStream(math::Matrix3Real<T>& data, IStream& stream)
		{
			ssizet size = 0; 
			size += stream.Read(&data, sizeof(data));
			if(size == sizeof(data))
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<Matrix3Real>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", sizeof(data), size));
		}

		static SPtr<cJSON> CreateJSON(const math::Matrix3Real<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const math::Matrix3Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr;
			if constexpr(std::is_same_v<T, float>)
				arr = cJSON_CreateFloatArray((const float*)&data, math::Matrix3Real<T>::ComponentCount);
			else
				arr = cJSON_CreateDoubleArray((const double*)&data, math::Matrix3Real<T>::ComponentCount);
			
			cJSON_AddItemToObject(json, name.data(), arr);
			return obj;			
		}
		
		static EmptyResult FromJSON(math::Matrix3Real<T>& data, cJSON* json, StringView name)
		{
			cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(arr == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<Matrix3Real>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			if(!cJSON_IsArray(arr))
				return Result::CreateFailure("[refl::ComplexType<Matrix3Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsArray."sv);
			
			int32 arrSize = cJSON_GetArraySize(arr);
			if(arrSize != math::Matrix3Real<T>::ComponentCount)
				return Result::CreateFailure(Format("[refl::ComplexType<Matrix3Real>]::FromJSON Couldn't obtain the value, it had different size, expected:%" PRIuPTR ", obtained:%" PRId32 ".", math::Matrix3Real<T>::ComponentCount, arr));
			
			for(decltype(math::Matrix3Real<T>::ComponentCount) i = 0; i < math::Matrix3Real<T>::ComponentCount; ++i)
			{
				cJSON* item = cJSON_GetArrayItem(arr, i);
				if(!cJSON_IsNumber(item))
					return Result::CreateFailure("[refl::ComplexType<Matrix3Real>]::FromJSON Couldn't obtain the value, it wasn't cJSON_IsNumber."sv);

				data[i] = static_cast<T>(cJSON_GetNumberValue(item));
			}
			
			return Result::CreateSuccess();
		}

		static String ToString(const math::Matrix3Real<T>& data)
		{
			return data.ToString();
		}

		static EmptyResult FromString(const String& str, math::Matrix3Real<T>& data)
		{
			data.FromString(str);
			return true;
		}

		NODISCARD static int64 GetDynamicSize(UNUSED const math::Matrix3Real<T>& data)
		{
			return 0ll;
		}

		NODISCARD static sizet GetArraySize(UNUSED const math::Matrix3Real<T>& data)
		{
			Break("[refl::ComplexType<Matrix3Real>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED math::Matrix3Real<T>& data, UNUSED sizet size)
		{
			Break("[refl::ComplexType<Matrix3Real>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const math::Matrix3Real<T>& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::ComplexType<Matrix3Real>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED math::Matrix3Real<T>& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::ComplexType<Matrix3Real>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};
}*/

#endif /* MATH_REFL_MATRIX3_H */