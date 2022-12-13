/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_PLAINTYPE_H
#define CORE_REFLECTION_PLAINTYPE_H 1

#include "BaseType.h"
#include "../Uuid.h"

#define CREATE_MEMCPY_PLAINTYPE(type, toString, fromString)\
namespace greaper::refl { \
	template<> struct PlainType<type> : public BaseType<type> {\
		static inline constexpr TypeCategory_t Category = TypeCategory_t::Plain; \
		static ssizet ToStream(const type& data, IStream& stream){ \
			return stream.Write(&data, sizeof(data)); \
		}\
		static ssizet FromStream(type& data, IStream& stream){ \
			return stream.Read(&data, sizeof(data));\
		}\
		NODISCARD static String ToString(const type& data){\
			return toString ;\
		}\
		static bool FromString(const String& str, type& data){\
			fromString ; \
		}\
		NODISCARD static ssizet GetDynamicSize(UNUSED const type& data){\
			return 0ll; \
		}\
		static void SetDynamicSize(UNUSED type& data, UNUSED ssizet size){}\
	};\
}

CREATE_MEMCPY_PLAINTYPE(bool, 			data ? "true" : "false", 				data = StringUtils::ToLower(str) == "true");
CREATE_MEMCPY_PLAINTYPE(int8,			String{ std::to_string(data).data() }, 	data = (int8)std::strtol(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(uint8,			String{ std::to_string(data).data() }, 	data = (uint8)std::strtoul(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(int16,			String{ std::to_string(data).data() }, 	data = (int16)std::strtol(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(uint16,			String{ std::to_string(data).data() }, 	data = (uint16)std::strtoul(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(int32,			String{ std::to_string(data).data() }, 	data = (int32)std::strtol(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(uint32,			String{ std::to_string(data).data() }, 	data = (uint32)std::strtoul(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(int64,			String{ std::to_string(data).data() }, 	data = (int16)std::strtoll(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(uint64,			String{ std::to_string(data).data() }, 	data = (uint16)std::strtoull(str.data(), nullptr, 10));
CREATE_MEMCPY_PLAINTYPE(float,			String{ std::to_string(data).data() }, 	data = std::strtof(str.data(), nullptr));
CREATE_MEMCPY_PLAINTYPE(double,			String{ std::to_string(data).data() }, 	data = std::strtod(str.data(), nullptr));
CREATE_MEMCPY_PLAINTYPE(greaper::Uuid,	data.ToString(),						data.FromString(str));

#endif /* CORE_REFLECTION_PLAINTYPE_H */