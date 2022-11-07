/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_RELFECTED_PLAIN_ENUM_H
#define CORE_REFLECTION_RELFECTED_PLAIN_ENUM_H 1

#include "ReflectedPlainType.h"
#include "../Enumeration.h"

namespace greaper
{
	template<class T>
	struct ReflectedPlainType<TEnum<T>>
	{
		enum { ID = RTI_Enum }; enum { HasDynamicSize = 0 };

		static_assert(HasEnumReflection<T>::value, "Trying to serialize an non-Greaper enum as such");

		static ReflectedSize_t ToStream(const T& data, IStream& stream)
		{
			const auto d = (int32)data;
			return stream.Write(&d, sizeof(d));
		}

		static ReflectedSize_t FromStream(T& data, IStream& stream)
		{
			int32 d;
			const auto ret = stream.Read(&d, sizeof(d));
			data = (T)d;
			return ret;
		}

		static String ToString(const T& data)
		{
			return String{ TEnum<T>::ToString(data); }
		}

		static void FromString(T& data, const String& str)
		{
			data = TEnum<T>::FromString(str);
		}

		static ReflectedSize_t GetSize(const T& data)
		{
			return sizeof(int32);
		}
	};
}

#endif /* CORE_REFLECTION_RELFECTED_PLAIN_ENUM_H */