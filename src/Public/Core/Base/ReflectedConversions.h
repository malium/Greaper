/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#ifndef CORE_REFL_CONVERSIONS_H
#define CORE_REFL_CONVERSIONS_H 1

#pragma once

#define CREATE_TYPEINFO(type, RTIType, categoryType)\
template<> struct greaper::refl::TypeInfo<type> { static constexpr ReflectedTypeID_t ID = RTIType; using Type = greaper::refl:: categoryType<type>; static constexpr StringView Name = #type##sv ; }

#define CREATE_TYPEINFO_CNAME(type, RTIType, categoryType, name)\
template<> struct greaper::refl::TypeInfo<type> { static constexpr ReflectedTypeID_t ID = RTIType; using Type = greaper::refl:: categoryType<type>; static constexpr StringView Name = name##sv ; }

CREATE_TYPEINFO(bool, greaper::refl::RTI_Bool, PlainType);
CREATE_TYPEINFO(int8, greaper::refl::RTI_Int8, PlainType);
CREATE_TYPEINFO(uint8, greaper::refl::RTI_Uint8, PlainType);
CREATE_TYPEINFO(int16, greaper::refl::RTI_Int16, PlainType);
CREATE_TYPEINFO(uint16, greaper::refl::RTI_Uint16, PlainType);
CREATE_TYPEINFO(int32, greaper::refl::RTI_Int32, PlainType);
CREATE_TYPEINFO(uint32, greaper::refl::RTI_Uint32, PlainType);
CREATE_TYPEINFO(int64, greaper::refl::RTI_Int64, PlainType);
CREATE_TYPEINFO(uint64, greaper::refl::RTI_Uint64, PlainType);
CREATE_TYPEINFO(float, greaper::refl::RTI_Float, PlainType);
CREATE_TYPEINFO(double, greaper::refl::RTI_Double, PlainType);
CREATE_TYPEINFO_CNAME(greaper::RectF, greaper::refl::RTI_RectF, ComplexType, "RectF");
CREATE_TYPEINFO_CNAME(greaper::RectD, greaper::refl::RTI_RectD, ComplexType, "RectD");
CREATE_TYPEINFO_CNAME(greaper::RectI, greaper::refl::RTI_RectI, ComplexType, "RectI");
CREATE_TYPEINFO_CNAME(greaper::RectU, greaper::refl::RTI_RectU, ComplexType, "RectU");
CREATE_TYPEINFO_CNAME(greaper::Uuid, greaper::refl::RTI_UUID, PlainType, "UUID");
CREATE_TYPEINFO_CNAME(greaper::IProperty, greaper::refl::RTI_Property, ComplexType, "Property");
CREATE_TYPEINFO_CNAME(greaper::String, greaper::refl::RTI_String, ContainerType, "String");
CREATE_TYPEINFO_CNAME(greaper::WString, greaper::refl::RTI_WString, ContainerType, "WString");

namespace greaper::refl
{
	template<class T> struct TypeInfo<TEnum<T>> { static constexpr ReflectedTypeID_t ID = RTI_Enum; using Type = PlainType<TEnum<T>>; static constexpr StringView Name = "Enum"sv; };
	template<typename T> struct TypeInfo<TProperty<T>> { static constexpr ReflectedTypeID_t ID = RTI_Property; using Type = ComplexType<TProperty<T>>; static constexpr StringView Name = "Property"sv; };
	template<typename F, typename S> struct TypeInfo<std::pair<F, S>> { static constexpr ReflectedTypeID_t ID = RTI_Pair; using Type = PlainType<std::pair<F, S>>; static constexpr StringView Name = "pair"sv; };
	template<typename T, sizet N> struct TypeInfo<std::array<T, N>> { static constexpr ReflectedTypeID_t ID = RTI_Array; using Type = ContainerType<std::array<T, N>>; static constexpr StringView Name = "array"sv; };
	template<typename T, typename A> struct TypeInfo<Vector<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_Vector; using Type = ContainerType<Vector<T, A>>; static constexpr StringView Name = "vector"sv; };
	template<typename T, typename A> struct TypeInfo<Deque<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_Deque; using Type = ContainerType<Deque<T, A>>; static constexpr StringView Name = "deque"sv; };
	template<typename T, typename A> struct TypeInfo<List<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_List; using Type = ContainerType<List<T, A>>; static constexpr StringView Name = "list"sv; };
	template<typename K, typename C, typename A> struct TypeInfo<Set<K, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_Set; using Type = ContainerType<Set<K, C, A>>; static constexpr StringView Name = "set"sv; };
	template<typename K, typename V, typename C, typename A> struct TypeInfo<Map<K, V, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_Map; using Type = ContainerType<Map<K, V, C, A>>; static constexpr StringView Name = "map"sv; };
	template<typename K, typename C, typename A> struct TypeInfo<MultiSet<K, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_MultiSet; using Type = ContainerType<MultiSet<K, C, A>>; static constexpr StringView Name = "multiset"sv; };
	template<typename K, typename V, typename C, typename A> struct TypeInfo<MultiMap<K, V, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_MultiMap; using Type = ContainerType<MultiMap<K, V, C, A>>; static constexpr StringView Name = "multimap"sv; };
	template<typename T, typename H, typename C, typename A> struct TypeInfo<UnorderedSet<T, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedSet; using Type = ContainerType<UnorderedSet<T, H, C, A>>; static constexpr StringView Name = "unordered_set"sv; };
	template<typename K, typename V, typename H, typename C, typename A> struct TypeInfo<UnorderedMap<K, V, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMap; using Type = ContainerType<UnorderedMap<K, V, H, C, A>>; static constexpr StringView Name = "unordered_map"sv; };
	template<typename K, typename V, typename H, typename C, typename A> struct TypeInfo<UnorderedMultiMap<K, V, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMultiMap; using Type = ContainerType<UnorderedMultiMap<K, V, H, C, A>>; static constexpr StringView Name = "unordered_multimap"sv; };
	template<typename T, typename H, typename C, typename A> struct TypeInfo<UnorderedMultiSet<T, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMultiSet; using Type = ContainerType<UnorderedMultiSet<T, H, C, A>>; static constexpr StringView Name = "unordered_multiset"sv; };
}

#endif