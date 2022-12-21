/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#ifndef CORE_REFL_CONVERSIONS_H
#define CORE_REFL_CONVERSIONS_H 1

#pragma once

#define CREATE_TYPEINFO(type, RTIType, categoryType)\
template<> struct greaper::refl::TypeInfo<type> { static constexpr ReflectedTypeID_t ID = RTIType; using Type = greaper::refl:: categoryType<type>; }

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
CREATE_TYPEINFO(greaper::RectF, greaper::refl::RTI_RectF, ComplexType);
CREATE_TYPEINFO(greaper::RectI, greaper::refl::RTI_RectI, ComplexType);
CREATE_TYPEINFO(greaper::RectU, greaper::refl::RTI_RectU, ComplexType);
CREATE_TYPEINFO(greaper::Uuid, greaper::refl::RTI_UUID, PlainType);
CREATE_TYPEINFO(greaper::IProperty, greaper::refl::RTI_Property, ComplexType);

namespace greaper::refl
{
	template<class T> struct TypeInfo<TEnum<T>> { static constexpr ReflectedTypeID_t ID = RTI_Enum; using Type = PlainType<TEnum<T>>; };
	template<typename T> struct TypeInfo<TProperty<T>> { static constexpr ReflectedTypeID_t ID = RTI_Property; using Type = ComplexType<TProperty<T>>; };
	template<> struct TypeInfo<String> { static constexpr ReflectedTypeID_t ID = RTI_String; using Type = ContainerType<String>; };
	template<> struct TypeInfo<WString> { static constexpr ReflectedTypeID_t ID = RTI_WString; using Type = ContainerType<WString>; };
	template<typename F, typename S> struct TypeInfo<std::pair<F, S>> { static constexpr ReflectedTypeID_t ID = RTI_Pair; using Type = PlainType<std::pair<F, S>>; };
	template<typename T, sizet N> struct TypeInfo<std::array<T, N>> { static constexpr ReflectedTypeID_t ID = RTI_Array; using Type = ContainerType<std::array<T, N>>; };
	template<typename T, typename A> struct TypeInfo<Vector<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_Vector; using Type = ContainerType<Vector<T, A>>; };
	template<typename T, typename A> struct TypeInfo<Deque<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_Deque; using Type = ContainerType<Deque<T, A>>; };
	template<typename T, typename A> struct TypeInfo<List<T, A>> { static constexpr ReflectedTypeID_t ID = RTI_List; using Type = ContainerType<List<T, A>>; };
	template<typename K, typename C, typename A> struct TypeInfo<Set<K, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_Set; using Type = ContainerType<Set<K, C, A>>; };
	template<typename K, typename V, typename C, typename A> struct TypeInfo<Map<K, V, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_Map; using Type = ContainerType<Map<K, V, C, A>>; };
	template<typename K, typename C, typename A> struct TypeInfo<MultiSet<K, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_MultiSet; using Type = ContainerType<MultiSet<K, C, A>>; };
	template<typename K, typename V, typename C, typename A> struct TypeInfo<MultiMap<K, V, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_MultiMap; using Type = ContainerType<MultiMap<K, V, C, A>>; };
	template<typename T, typename H, typename C, typename A> struct TypeInfo<UnorderedSet<T, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedSet; using Type = ContainerType<UnorderedSet<T, H, C, A>>; };
	template<typename K, typename V, typename H, typename C, typename A> struct TypeInfo<UnorderedMap<K, V, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMap; using Type = ContainerType<UnorderedMap<K, V, H, C, A>>; };
	template<typename K, typename V, typename H, typename C, typename A> struct TypeInfo<UnorderedMultiMap<K, V, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMultiMap; using Type = ContainerType<UnorderedMultiMap<K, V, H, C, A>>; };
	template<typename T, typename H, typename C, typename A> struct TypeInfo<UnorderedMultiSet<T, H, C, A>> { static constexpr ReflectedTypeID_t ID = RTI_UnorderedMultiSet; using Type = ContainerType<UnorderedMultiSet<T, H, C, A>>; };
}

#endif