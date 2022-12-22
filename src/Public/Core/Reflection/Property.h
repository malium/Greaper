/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../Property.h"
#include "ComplexType.h"

namespace greaper::refl
{
	template<>
	const Vector<SPtr<IField>> ComplexType<PropertyInt>::Fields = Vector<SPtr<IField>>({
		SPtr<IField>(Construct<TField<String>>("Name"sv,
		[](const void* obj) -> const void* { return &(((PropertyInt*)obj)->GetPropertyName()); })),
		SPtr<IField>(Construct<TField<ReflectedTypeID_t>>("TypeID"sv, 
		[](UNUSED const void* obj) -> const void* { return &TypeInfo<PropertyInt::value_type>::ID; })),
		SPtr<IField>(Construct<TField<PropertyInt::value_type>>("Value"sv,
		[](void* obj) -> void* { return &(((PropertyInt*)obj)->GetValueCopy()); })
		});
	/*template<>
	struct ComplexType<IProperty> : public BaseType<IProperty>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		using StringCat = ContainerType<String>;
		using ValueIDCat = PlainType<ReflectedTypeID_t>;

		static inline constexpr ssizet StaticSize = StringCat::StaticSize + ValueIDCat::StaticSize;

		static TResult<ssizet> ToStream(const IProperty& data, IStream& stream)
		{ 
			auto dynamicSize = GetDynamicSize(data);
			ssizet size = 0;
			TResult<ssizet> res = StringCat::ToStream(data.GetPropertyName(), stream);
			if(res.HasFailed())
				return res;

			size += res.GetValue();
			
			res = ValueIDCat::ToStream(data._ValueTypeID(), stream);
			if(res.HasFailed())
				return res;

			size += res.GetValue();
			res = data._ValueToStream(stream);
			if(res.HasFailed())
				return res;

			size += res.GetValue();

			ssizet expectedSize = dynamicSize + StaticSize; 
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<IProperty>]::ToStream Failure while writing to stream, not all data was written, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static TResult<ssizet> FromStream(IProperty& data, IStream& stream)
		{
			ssizet size = 0;
			ssizet dynamicSize = 0;

			String name;
			TResult<ssizet> res = StringCat::FromStream(name, stream);
			if(res.HasFailed())
				return res;
			
			if(data.GetPropertyName() != name)
			{
				return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<IProperty]>]::FromStream the property name read from stream was not matching! Wanted:'%s' Obtained:'%s'.",
					data.GetPropertyName().c_str(), name.c_str()));
			}
			size += res.GetValue();
			dynamicSize += StringCat::GetDynamicSize(name);

			ReflectedTypeID_t typeID;
			res = ValueIDCat::FromStream(typeID, stream);
			if(res.HasFailed())
				return res;

			if(data._ValueTypeID() != typeID)
			{
				return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<IProperty]>]::FromStream the property typeID read from stream was not matching! Wanted:'%" PRId64 "' Obtained:'%" PRId64 "'.",
					data._ValueTypeID(), typeID));
			}
			size += res.GetValue();
			dynamicSize += ValueIDCat::GetDynamicSize(typeID);

			res = data._ValueFromStream(stream);
			if(res.HasFailed())
				return res;
			size += res.GetValue();
			dynamicSize += data._GetDynamicSize();

			ssizet expectedSize = StaticSize + dynamicSize;
			if(size == expectedSize)
				return Result::CreateSuccess(size);
			return Result::CreateFailure<ssizet>(Format("[refl::ContainerType<IProperty>]::FromStream Failure while reading from stream, not all data was read, expected:%" PRIuPTR " obtained:%" PRIdPTR ".", expectedSize, size));
		}

		static SPtr<cJSON> CreateJSON(const IProperty& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			ToJSON(data, obj, name);
			return SPtr<cJSON>(obj, cJSON_Delete);
		}

		static cJSON* ToJSON(const IProperty& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddStringToObject(obj, "name", data.GetPropertyName().c_str());
			cJSON_AddNumberToObject(obj, "typeID", (double)data._ValueTypeID());
			data._ValueToJSON(obj, "value"sv);
			return obj;
		}

		static EmptyResult FromJSON(IProperty& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<IProperty>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			cJSON* propNameObj = cJSON_GetObjectItemCaseSensitive(item, "name");
			StringView propName = cJSON_GetStringValue(propNameObj);
			if(data.GetPropertyName() != propName)
			{
				return Result::CreateFailure(Format("[refl::ComplexType<IProperty>]::FromJSON Couldn't obtain the value from json, the property had different name, expected:'%s' obtained:'%s'.", 
					data.GetPropertyName().c_str(), propName.data()));
			}
			cJSON* typeIDObj = cJSON_GetObjectItemCaseSensitive(item, "typeID");
			auto typeID = (ReflectedTypeID_t)cJSON_GetNumberValue(typeIDObj);
			if(typeID != data._ValueTypeID())
			{
				return Result::CreateFailure(Format("[refl::ComplexType<IProperty>]::FromJSON Couldn't obtain the value from json, the property had different typeID, expected:%" PRId64 " obtained:%" PRId64 ".",
					data._ValueTypeID(), typeID));
			}

			EmptyResult res = data._ValueFromJSON(item, "value"sv);
			if(res.HasFailed())
				return res;
			
			return Result::CreateSuccess();
		}

		NODISCARD static String ToString(const IProperty& data)
		{
			return data.GetStringValueCopy();
		}

		static EmptyResult FromString(const String& str, IProperty& data)
		{
			data.SetValueFromString(str);
			return Result::CreateSuccess();
		}

		NODISCARD static int64 GetDynamicSize(const IProperty& data)
		{
			return StringCat::GetDynamicSize(data.GetPropertyName()) + ValueIDCat::GetDynamicSize(data._ValueTypeID())
				+ data._GetStaticSize() + data._GetDynamicSize();
		}

		NODISCARD static sizet GetArraySize(UNUSED const IProperty& data)
		{
			Break("[refl::ComplexType<IProperty>]::GetArraySize Trying to use a PlainType for array operations!");
			return 0ll;
		}

		static void SetArraySize(UNUSED IProperty& data, UNUSED sizet size)
		{
			Break("[refl::ComplexType<IProperty>]::SetArraySize Trying to use a PlainType for array operations!");
		}

		NODISCARD static const int32& GetArrayValue(UNUSED const IProperty& data, UNUSED sizet index)
		{
			static constexpr int32 dummy = 0;
			Break("[refl::ComplexType<IProperty>]::GetArrayValue Trying to use a PlainType for array operations!");
			return dummy;
		}

		static void SetArrayValue(UNUSED IProperty& data, UNUSED const int32& value, UNUSED sizet index)
		{
			Break("[refl::ComplexType<IProperty>]::SetArrayValue Trying to use a PlainType for array operations!");
		}
	};

	template<class T> 
	struct ComplexType<TProperty<T>> : public BaseType<TProperty<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		using StringCat = ContainerType<String>;
		using ValueIDCat = PlainType<ReflectedTypeID_t>;

		static inline constexpr ssizet StaticSize = ComplexType<IProperty>::StaticSize;

		static TResult<ssizet> ToStream(const TProperty<T>& data, IStream& stream)
		{ 
			return ComplexType<IProperty>::ToStream(data, stream);
		}
		
		static TResult<ssizet> FromStream(TProperty<T>& data, IStream& stream)
		{
			return ComplexType<IProperty>::FromStream(data, stream);
		}

		static SPtr<cJSON> CreateJSON(const TProperty<T>& data, StringView name)
		{
			return ComplexType<IProperty>::CreateJSON(data, name);
		}

		static cJSON* ToJSON(const TProperty<T>& data, cJSON* json, StringView name)
		{
			return ComplexType<IProperty>::ToJSON(data, json, name);
		}

		static EmptyResult FromJSON(TProperty<T>& data, cJSON* json, StringView name)
		{
			return ComplexType<IProperty>::FromJSON(data, json, name);
		}

		NODISCARD static String ToString(const TProperty<T>& data)
		{
			return ComplexType<IProperty>::ToString(data);
		}

		static EmptyResult FromString(const String& str, TProperty<T>& data)
		{
			return ComplexType<IProperty>::FromString(str, data);
		}
		
		NODISCARD static int64 GetDynamicSize(const TProperty<T>& data)
		{
			return ComplexType<IProperty>::GetDynamicSize(data);
		}

		NODISCARD static sizet GetArraySize(const TProperty<T>& data)
		{
			return ComplexType<IProperty>::GetArraySize(data);
		}

		static void SetArraySize(TProperty<T>& data, sizet size)
		{
			ComplexType<IProperty>::SetArraySize(data, size);
		}

		NODISCARD static const int32& GetArrayValue(const TProperty<T>& data, sizet index)
		{
			return ComplexType<IProperty>::GetArrayValue(data, index);
		}

		static void SetArrayValue(TProperty<T>& data, const int32& value, sizet index)
		{
			ComplexType<IProperty>::SetArrayValue(data, value, index);
		}
	};*/
}