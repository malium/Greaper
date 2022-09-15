/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <utility>

namespace greaper
{
	template<class T> struct ReflectedTypeToID { static constexpr ReflectedTypeID_t ID = RTI_Unknown; };
	template<> struct ReflectedTypeToID<bool> { static constexpr ReflectedTypeID_t ID = RTI_Bool; };
	template<> struct ReflectedTypeToID<int8> { static constexpr ReflectedTypeID_t ID = RTI_Int8; };
	template<> struct ReflectedTypeToID<uint8> { static constexpr ReflectedTypeID_t ID = RTI_Uint8; };
	template<> struct ReflectedTypeToID<int16> { static constexpr ReflectedTypeID_t ID = RTI_Int16; };
	template<> struct ReflectedTypeToID<uint16> { static constexpr ReflectedTypeID_t ID = RTI_Uint16; };
	template<> struct ReflectedTypeToID<int32> { static constexpr ReflectedTypeID_t ID = RTI_Int32; };
	template<> struct ReflectedTypeToID<uint32> { static constexpr ReflectedTypeID_t ID = RTI_Uint32; };
	template<> struct ReflectedTypeToID<int64> { static constexpr ReflectedTypeID_t ID = RTI_Int64; };
	template<> struct ReflectedTypeToID<uint64> { static constexpr ReflectedTypeID_t ID = RTI_Uint64; };
	template<> struct ReflectedTypeToID<float> { static constexpr ReflectedTypeID_t ID = RTI_Float; };
	template<> struct ReflectedTypeToID<double> { static constexpr ReflectedTypeID_t ID = RTI_Double; };
	template<typename F, typename S> struct ReflectedTypeToID<std::pair<F, S>> { static constexpr ReflectedTypeID_t ID = RTI_Pair; };
}