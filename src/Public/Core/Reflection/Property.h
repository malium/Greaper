/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "ComplexType.h"
//#include "../Property.h"

namespace greaper::refl
{
	template<class T> 
	struct ComplexType<TProperty<T>> : public BaseType<TProperty<T>>
	{
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Complex;

		using StringCat = ContainerType<String>;
		using ValueIDCat = PlainType<ReflectedTypeID_t>;
		using ValueCat = GetCategoryType<T>::Type;

		static inline constexpr ssizet StaticSize = StringCat::StaticSize + ValueIDCat::StaticSize + ValueCat::StaticSize;

		static TResult<ssizet> ToStream(const TProperty<T>& data, IStream& stream)
		{ 
			ssizet size = 0;
			TResult<ssizet> res = StringCat::ToStream(data.GetPropertyName(), stream);
			if(res.HasFailed())
				return res;

			size += res.GetValue();
			
			res = ValueIDCat::ToStream(ReflectedTypeToID<T>::ID, stream);
			if(res.HasFailed())
				return res;

			size += res.GetValue();

			//auto accessFn = [&stream, &res](const T& val){ res = ValueCat::ToStream(val, stream); };
			//data.AccessValue(accessFn);
			res = data._ValueToStream(stream);
			if(res.HasFailed())
				return res;

			size += res.GetValue();

			return size;
		}
		
		static TResult<ssizet> FromStream(TProperty<T>& data, IStream& stream)
		{
			ssizet size = 0;

			String name;
			TResult<ssizet> res = StringCat::FromStream(name, stream);
			if(res.HasFailed())
				return res;
			
			if(data.GetPropertyName() != name)
			{
				return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<TProperty]>]::FromStream the property name read from stream was not matching! Wanted:'%s' Obtained:'%s'.",
					data.GetPropertyName().c_str(), name.c_str()));
			}
			size += res.GetValue();

			ReflectedTypeID_t typeID;
			res = ValueIDCat::FromStream(typeID, stream);
			if(res.HasFailed())
				return res;

			if(ReflectedTypeToID<T>::ID != typeID)
			{
				return Result::CreateFailure<ssizet>(Format("[refl::ComplexType<TProperty]>]::FromStream the property typeID read from stream was not matching! Wanted:'%"PRId64"' Obtained:'%"PRId64"'.",
					ReflectedTypeToID<T>::ID, typeID));
			}
			size += res.GetValue();

			//T value;
			//res = ValueCat::FromStream(value, stream);
			res = data._ValueFromStream(stream);
			if(res.HasFailed())
				return res;
			size += res.GetValue();

			//data.SetValue(value, false, true);

			return size;
		}

		static cJSON* ToJSON(const TProperty<T>& data, StringView name)
		{
			cJSON* obj = cJSON_CreateObject();
			return ToJSON(data, obj, name);
		}
		static cJSON* ToJSON(const TProperty<T>& data, cJSON* json, StringView name)
		{
			cJSON* obj = cJSON_AddObjectToObject(json, name.data());
			cJSON_AddStringToObject(obj, "name", data.GetPropertyName().c_str());
			cJSON_AddNumberToObject(obj, "typeID", ReflectedTypeToID<T>::ID);
			//auto accessFn = [obj](const T& value) { ValueCat::ToJSON(value, obj, "value"); };
			//data.AccessValue(accessFn);
			data._ValueToJSON(obj, "value"sv);
			return obj;
		}
		static EmptyResult FromJSON(TProperty<T>& data, cJSON* json, StringView name)
		{
			cJSON* item = cJSON_GetObjectItemCaseSensitive(json, name.data());
			if(item == nullptr)
				return Result::CreateFailure(Format("[refl::ComplexType<TProperty>]::FromJSON Couldn't obtain the value from json, the item with name '%s' was not found.", name.data()));

			cJSON* propNameObj = cJSON_GetObjectItemCaseSensitive(item, "name");
			StringView propName = cJSON_GetStringValue(propNameObj);
			if(data.GetPropertyName() != propName)
			{
				return Result::CreateFailure(Format("[refl::ComplexType<TProperty>]::FromJSON Couldn't obtain the value from json, the property had different name, expected:'%s' obtained:'%s'.", 
					data.GetPropertyName().c_str(), propName.data()));
			}
			cJSON* typeIDObj = cJSON_GetObjectItemCaseSensitive(item, "typeID");
			ReflectedTypeID_t typeID = cJSON_GetNumberValue(typeIDObj);
			if(typeID != ReflectedTypeToID<T>::ID)
			{
				return Result::CreateFailure(Format("[refl::ComplexType<TProperty>]::FromJSON Couldn't obtain the value from json, the property had different typeID, expected:%"PRId64" obtained:%"PRId64".",
					ReflectedTypeToID<T>::ID, typeID));
			}

			//T value;
			//EmptyResult res = ValueCat::FromJSON(value, item, "value"sv);
			EmptyResult res = data._ValueFromJSON(item, "value"sv);
			if(res.HasFailed())
				return res;
			
			//data.SetValue(value, false, true);
			return Result::CreateSuccess();
		}
		NODISCARD static String ToString(const TProperty<T>& data)
		{
			return data.GetStringValueCopy();
		}
		static EmptyResult FromString(const String& str, TProperty<T>& data)
		{
			data.SetValueFromString(str);
			return Result::CreateSuccess();
		}
		NODISCARD static int64 GetDynamicSize(const TProperty<T>& data)
		{
			return StringCat::GetDynamicSize(data.GetPropertyName()) + ValueIDCat::GetDynamicSize(ReflectedTypeToID<T>::ID)
				+ data._GetDynamicSize();
		}
		static void SetDynamicSize(UNUSED TProperty<T>& data, UNUSED int64 size)
		{
			/* No-op */
		}
	};
}