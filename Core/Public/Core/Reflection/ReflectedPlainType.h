/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_REFLECTED_PLAIN_TYPE_H
#define CORE_REFLECTION_REFLECTED_PLAIN_TYPE_H 1

#include "../CorePrerequisites.h"
#include "../Stream.h"
#include "../Uuid.h"
#include <functional>

#ifndef REFLECTION_STRING_ELEMENT_SEPARATOR
#define REFLECTION_STRING_ELEMENT_SEPARATOR '|'
#endif
#ifndef REFLECTION_STRING_INNER_ELEMENT_SEPARATOR
#define REFLECTION_STRING_INNER_ELEMENT_SEPARATOR '~'
#endif

namespace greaper
{
	template<class T>
	struct ReflectedPlainType
	{
		static_assert(std::is_pod_v<T>,
			"Provided type isn't plain-old-data. You need to specialize ReflectedPlainType template in order to serialize this type. "
			" (Or call ALLOW_MEMCPY_SERIALIZATION(type, id, toString, fromString) macro if you are sure the type can be properly "
			"serialized using just memcpy.)");

		enum { ID = RTI_Unknown }; enum { HasDynamicSize = 0 };

		static ReflectedSize_t ToStream(const T& data, IStream& stream);

		static ReflectedSize_t FromStream(T& data, IStream& stream);

		static String ToString(const T& data);

		static void FromString(T& data, const String& str);

		static ReflectedSize_t GetSize(const T& data);
	};

	template<typename T>
	ReflectedSize_t ReflectedSize(const T& data);

	template<typename T>
	ReflectedSize_t ReflectedWrite(const T& data, IStream& stream);

	template<typename T>
	ReflectedSize_t ReflectedRead(T& data, IStream& stream);

	template<typename T>
	String ReflectedToString(const T& data);

	template<typename T>
	void ReflectedFromString(T& data, const String& str);

	template<class T>
	ReflectedSize_t ReflectedWriteWithSizeHeader(IStream& stream, const T& data, std::function<ReflectedSize_t()> fn);

	ReflectedSize_t ReflectedReadSizeHeader(IStream& stream, ReflectedSize_t& size);

	void ReflectedAddHeaderSize(ReflectedSize_t& size);
}

template<class T>
inline greaper::ReflectedSize_t greaper::ReflectedPlainType<T>::ToStream(const T& data, IStream& stream)
{
	return stream.Write(&data, GetSize(data));
}

template<class T>
inline greaper::ReflectedSize_t greaper::ReflectedPlainType<T>::FromStream(T& data, IStream& stream)
{
	return stream.Read(&data, GetSize(data));
}

template<class T>
inline greaper::String greaper::ReflectedPlainType<T>::ToString(const T& data) 
{
	static_assert(false, "ReflectedPlainType with no ToString specialization for type T");
	return greaper::String{};
}


template<class T>
inline void greaper::ReflectedPlainType<T>::FromString(T& data, const String& str) 
{
	static_assert(false, "ReflectedPlainType with no FromString specialization for type T");
}

template<class T>
inline greaper::ReflectedSize_t greaper::ReflectedPlainType<T>::GetSize(const T& data)
{
	return sizeof(data);
}

template<typename T>
greaper::ReflectedSize_t greaper::ReflectedSize(const T& data)
{
	return ReflectedPlainType<T>::GetSize(data);
}

template<typename T>
greaper::ReflectedSize_t greaper::ReflectedWrite(const T& data, IStream& stream)
{
	return ReflectedPlainType<T>::ToStream(data, stream);
}

template<typename T>
greaper::ReflectedSize_t greaper::ReflectedRead(T& data, IStream& stream)
{
	return ReflectedPlainType<T>::FromStream(data, stream);
}

template<typename T>
greaper::String greaper::ReflectedToString(const T& data)
{
	return ReflectedPlainType<T>::ToString(data);
}

template<typename T>
void greaper::ReflectedFromString(T& data, const String& str)
{
	ReflectedPlainType<T>::FromString(data, str);
}

template<class T>
greaper::ReflectedSize_t greaper::ReflectedWriteWithSizeHeader(IStream& stream, const T& data, std::function<ReflectedSize_t()> fn)
{
	UNUSED(data);
	const auto sizePos = stream.Tell();
	ReflectedSize_t size = 0;
	stream.Write(&size, sizeof(size));
	size = fn() + sizeof(size);
	VerifyGreater(size, 0, "Trying to write a zero length type.");
	stream.Seek(sizePos);
	stream.Write(&size, sizeof(size));
	stream.Skip(size - sizeof(size));
	return size;
}

greaper::ReflectedSize_t greaper::ReflectedReadSizeHeader(IStream& stream, ReflectedSize_t& size)
{
	const auto byteSize = stream.Read(&size, sizeof(size));
	return byteSize;
}

void greaper::ReflectedAddHeaderSize(ReflectedSize_t& size)
{
	size += sizeof(size);
}

#define ALLOW_MEMCPY_SERIALIZATION(type, id, toString, fromString)							\
namespace greaper{																    		\
	static_assert(std::is_trivially_copyable_v<type>, #type " is not trivially copyable");	\
	template<>																				\
	struct ReflectedPlainType<type>															\
	{																						\
		enum { ID = id }; enum { HasDynamicSize = 0 };										\
																							\
		static ReflectedSize_t ToStream(const type& data, IStream& stream)					\
		{																					\
			return stream.Write(&data, sizeof(data));										\
		}																					\
																							\
		static ReflectedSize_t FromStream(type& data, IStream& stream)						\
		{																					\
			return stream.Read(&data, sizeof(data));										\
		}																					\
																							\
		static String ToString(const type& data)											\
		{																					\
			return toString ;																\
		}																					\
																							\
		static void FromString(type& data, const String& str)								\
		{																					\
			fromString ;																	\
		}																					\
																							\
		static ReflectedSize_t GetSize(const type& data)									\
		{																					\
			return sizeof(data);															\
		}																					\
	};																						\
}

ALLOW_MEMCPY_SERIALIZATION(bool,			greaper::RTI_Bool, data ? "true" : "false", data = str == "true");
ALLOW_MEMCPY_SERIALIZATION(int8,			greaper::RTI_Int8, String{ std::to_string(data).data() }, data = (int8)std::strtol(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(uint8,			greaper::RTI_Uint8, String{ std::to_string(data).data() }, data = (uint8)std::strtoul(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(int16,			greaper::RTI_Int16, String{ std::to_string(data).data() }, data = (int16)std::strtol(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(uint16,			greaper::RTI_Uint16, String{ std::to_string(data).data() }, data = (uint16)std::strtoul(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(int32,			greaper::RTI_Int32, String{ std::to_string(data).data() }, data = (int32)std::strtol(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(uint32,			greaper::RTI_Uint32, String{ std::to_string(data).data() }, data = (uint32)std::strtoul(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(int64,			greaper::RTI_Int64, String{ std::to_string(data).data() }, data = (int16)std::strtoll(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(uint64,			greaper::RTI_Uint64, String{ std::to_string(data).data() }, data = (uint16)std::strtoull(str.data(), nullptr, 10));
ALLOW_MEMCPY_SERIALIZATION(float,			greaper::RTI_Float, String{ std::to_string(data).data() }, data = std::strtof(str.data(), nullptr));
ALLOW_MEMCPY_SERIALIZATION(double,			greaper::RTI_Double, String{ std::to_string(data).data() }, data = std::strtod(str.data(), nullptr));
ALLOW_MEMCPY_SERIALIZATION(greaper::Uuid, 	greaper::RTI_UUID, data.ToString(), data.FromString(str));
//ALLOW_MEMCPY_SERIALIZATION(greaper::half, greaper::RTI_Half, String{ std::to_string(data).data() }, data = (half)std::strtoul(str.data(), nullptr, 10));

#endif /* CORE_REFLECTION_REFLECTED_PLAIN_TYPE_H */