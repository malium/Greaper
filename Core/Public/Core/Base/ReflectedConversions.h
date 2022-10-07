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
	template<> struct ReflectedTypeToID<String> { static constexpr ReflectedTypeID_t ID = RTI_String; };
	template<> struct ReflectedTypeToID<WString> { static constexpr ReflectedTypeID_t ID = RTI_WString; };
	template<typename T, sizet N> struct ReflectedTypeToID<std::array<T, N>> { static constexpr ReflectedTypeID_t ID = RTI_Array; };
	template<typename T, typename A> struct ReflectedTypeToID<Vector<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_Vector; };
	template<typename T, typename A> struct ReflectedTypeToID<List<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_List; };
	template<typename K, typename P, typename A> struct ReflectedTypeToID<Set<K, P, A>> { static constexpr ReflectedTypeID_t ID = RTI_Set; };
	template<typename K, typename V, typename P, typename A> struct ReflectedTypeToID<Map<K, V, P, A>> { static constexpr ReflectedTypeID_t ID = RTI_Map; };
	template<typename K, typename P, typename A> struct ReflectedTypeToID<MultiSet<K, P, A>> { static constexpr ReflectedTypeID_t ID = RTI_MultiSet; };
	template<typename K, typename V, typename P, typename A> struct ReflectedTypeToID<MultiMap<K, V, P, A>> { static constexpr ReflectedTypeID_t ID = RTI_MultiMap; };
	template<typename T, typename H, typename C, typename A> struct ReflectedTypeToID<UnorderedSet<T, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedSet; };
	template<typename K, typename V, typename H, typename C, typename A> struct ReflectedTypeToID<UnorderedMap<K, V, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMap; };
	template<typename K, typename V, typename H, typename C, typename A> struct ReflectedTypeToID<UnorderedMultiMap<K, V, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMultiMap; };
	template<typename T, typename H, typename C, typename A> struct ReflectedTypeToID<UnorderedMultiSet<T, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMultiSet; };
}