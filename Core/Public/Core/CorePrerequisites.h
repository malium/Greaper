/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PREREQUISITES_H
#define CORE_PREREQUISITES_H 1

#include "PHAL.h"
#include "Memory.h"
#include <chrono>

#ifdef GREAPER_CORE_VERSION
#undef GREAPER_CORE_VERSION
#endif
#define GREAPER_CORE_VERSION VERSION_SETTER(0, 3, 0, 0)

#define DEF_PROP(name, type) static constexpr StringView name##Name = #name##sv; using name##Prop_t = TProperty<type>

template<class T> class TEnum;
namespace greaper
{
	using Clock_t = std::chrono::high_resolution_clock;
	using Timepoint_t = Clock_t::time_point;
	using Duration_t = Clock_t::duration;

	// fwd
	class GenericAllocator;
	class IApplication; using PApplication = SPtr<IApplication>; using WApplication = WPtr<IApplication>;
	class ILogManager; using PLogManager = SPtr<ILogManager>; using WLogManager = WPtr<ILogManager>;
	class IThreadManager; using PThreadManager = SPtr<IThreadManager>; using WThreadManager = WPtr<IThreadManager>;
	class IThread; using PThread = SPtr<IThread>; using WThread = WPtr<IThread>;
	struct ThreadConfig;
	class IGreaperLibrary; using PGreaperLib = SPtr<IGreaperLibrary>; using WGreaperLib = WPtr<IGreaperLibrary>;
	class IProperty; using PIProperty = SPtr<IProperty>; using WIProperty = WPtr<IProperty>;
	template<class T> class TProperty; template<class T> using PProperty = SPtr<TProperty<T>>; template<class T> using WProperty = WPtr<TProperty<T>>;
	template<class T> class TPropertyValidator;
	template<class... Args> class Event;
	class IStream;
	class Uuid;
	template<class T> class Result;

	// types
	using half = int16;

	using ReflectedFieldID_t = int16;
	using ReflectedTypeID_t = int64;
	using ReflectedSize_t = int64;
	template<class T> struct ReflectedPlainType;

	// Reflection ID
	enum CoreReflectedTypeID : ReflectedTypeID_t
	{
		RTI_Unknown = 0,

		// Base types
		RTI_Bool = 1,
		RTI_Int8,
		RTI_Uint8,
		RTI_Int16,
		RTI_Uint16,
		RTI_Int32,
		RTI_Uint32,
		RTI_Int64,
		RTI_Uint64,
		RTI_Float,
		RTI_Double,
		RTI_Half,
		RTI_Enum,
		RTI_Rect,

		// Base Containers
		RTI_String = 25,
		RTI_WString,
		RTI_Array,
		RTI_Vector,
		RTI_List,
		RTI_Deque,
		RTI_Map,
		RTI_UnorderedMap,
		RTI_MultiMap,
		RTI_UnorderedMultiMap,
		RTI_Pair,
		RTI_Set,
		RTI_UnorderedSet,
		RTI_MultiSet,
		RTI_UnorderedMultiSet,

		// Classes
		RTI_UUID = 50,
		RTI_Property,

	};
}
#include "Base/ReflectedConversions.h"

#endif /* CORE_PREREQUISITES_H */