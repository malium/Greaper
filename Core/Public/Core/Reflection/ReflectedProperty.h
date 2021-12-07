/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_REFLECTED_PROPERTY_H
#define CORE_REFLECTION_REFLECTED_PROPERTY_H 1

#include "../CorePrerequisites.h"
#include "ReflectedPlainType.h"
#include "../Property.h"

namespace greaper
{
	template<>
	struct ReflectedPlainType<IProperty>
	{
		enum { ID = RTI_Property }; enum { HasDynamicSize = 1 };

		static ReflectedSize_t ToStream(const IProperty& data, IStream& stream)
		{
			return ReflectedWriteWithSizeHeader(stream, data, [&data, &stream]()
				{
					const auto& name = data.GetPropertyName();
					const auto& value = data.GetStringValue();
					const sizet nameLength = name.size();
					const sizet valueLength = value.size();
					ReflectedSize_t size = (nameLength + valueLength) * sizeof(String::value_type) + sizeof(nameLength) + sizeof(valueLength);
					stream.Write(&nameLength, sizeof(nameLength));
					stream.Write(&valueLength, sizeof(valueLength));
					stream.Write(name.data(), nameLength * sizeof(String::value_type));
					stream.Write(value.data(), valueLength * sizeof(String::value_type));
					return size;
				});
		}

		static ReflectedSize_t FromStream(IProperty& data, IStream& stream)
		{
			ReflectedSize_t size = 0;

			ReflectedReadSizeHeader(stream, size);

			const auto propertySize = size - sizeof(ReflectedSize_t);
			sizet nameLength = 0, valueLength = 0;
			stream.Read(&nameLength, sizeof(nameLength));
			stream.Read(&valueLength, sizeof(valueLength));

			auto name = String(nameLength, (achar)0);
			auto value = String(valueLength, (achar)0);

			stream.Read(name.data(), nameLength * sizeof(String::value_type));
			stream.Read(value.data(), valueLength * sizeof(String::value_type));

			VerifyEqual(name, data.GetPropertyName(), "Trying to deserialize a Property with different name.");

			data.SetValueFromString(value);

			return size;
		}

		static String ToString(const IProperty& data)
		{
			return data.GetPropertyName() + REFLECTION_STRING_INNER_ELEMENT_SEPARATOR + data.GetStringValue();
		}

		static void FromString(IProperty& data, const String& str)
		{
			const auto sep = StringUtils::Tokenize(str, REFLECTION_STRING_INNER_ELEMENT_SEPARATOR);

			VerifyEqual(sep.size(), 2, "Trying to deserialize a property but has more elements than needed");
			VerifyEqual(data.GetPropertyName(), sep[0], "Trying to deserialize a property with inequal name");

			data.SetValueFromString(sep[1]);
		}

		static ReflectedSize_t GetSize(const IProperty& data)
		{
			const auto& name = data.GetPropertyName();
			const auto& value = data.GetStringValue();
			const sizet nameLength = name.size();
			const sizet valueLength = value.size();
			ReflectedSize_t dataSize = (nameLength + valueLength) * sizeof(String::value_type) + sizeof(nameLength) + sizeof(valueLength);
			ReflectedAddHeaderSize(dataSize); // Size Header

			return dataSize;
		}
	};
	template<class T>
	struct ReflectedPlainType<TProperty<T>>
	{
		enum { ID = RTI_Property }; enum { HasDynamicSize = 1 };

		static ReflectedSize_t ToStream(const TProperty<T>& data, IStream& stream)
		{
			return ReflectedPlainType<IProperty>::ToStream(data, stream);
		}

		static ReflectedSize_t FromStream(TProperty<T>& data, IStream& stream)
		{
			return ReflectedPlainType<IProperty>::FromStream(data, stream);
		}

		static String ToString(const TProperty<T>& data)
		{
			return ReflectedPlainType<IProperty>::ToString(data);
		}

		static void FromString(TProperty<T>& data, const String& str)
		{
			ReflectedPlainType<IProperty>::FromString(data, str);
		}

		static ReflectedSize_t GetSize(const TProperty<T>& data)
		{
			return ReflectedPlainType<IProperty>::GetSize(data);
		}
	};
}

#endif /* CORE_REFLECTION_REFLECTED_PROPERTY_H */