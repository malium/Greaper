/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PREREQUISITES_H
#define CORE_PREREQUISITES_H 1

#include "PHAL.h"
#include "Memory.h"
#include <chrono>
#include "Base/TypeTraits.h"

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

#if PLT_WINDOWS
	class WinThreadImpl; using Thread = WinThreadImpl;
#elif PLT_LINUX
	class LnxThreadImpl; using Thread = LnxThreadImpl;
#endif
	using WThread = WPtr<Thread>; using PThread = SPtr<Thread>;

	struct ThreadConfig;
	class ICommandManager; using PCommandManager = SPtr<ICommandManager>; using WCommandManager = WPtr<ICommandManager>;
	class ICommand; using PCommand = SPtr<ICommand>; using WCommand = WPtr<ICommand>;
	class IConsole; using PConsole = SPtr<IConsole>; using WConsole = WPtr<IConsole>;
	class Library; using PLibrary = SPtr<Library>; using WLibrary = WPtr<Library>;
	class IGreaperLibrary; using PGreaperLib = SPtr<IGreaperLibrary>; using WGreaperLib = WPtr<IGreaperLibrary>;
	class IInterface; using PInterface = SPtr<IInterface>; using WInterface = WPtr<IInterface>;
	class IProperty; using PIProperty = SPtr<IProperty>; using WIProperty = WPtr<IProperty>;
	template<class T> class TProperty; template<class T> using PProperty = SPtr<TProperty<T>>; template<class T> using WProperty = WPtr<TProperty<T>>;
	template<class T> class TPropertyValidator;
	template<class... Args> class Event;
	template<class T> class RectT;
	using RectF = RectT<float>;
	using RectD = RectT<double>;
	using RectI = RectT<int32>;
	using RectU = RectT<uint32>;
	
	class IStream;
	class Uuid;
	class Result;
	namespace Impl
	{
		template<class T> class TResult;
		struct EmptyStruc {  };
	}
	template<class T> using TResult = Impl::TResult<T>;
	using EmptyResult = Impl::TResult<Impl::EmptyStruc>;

	// Greaper Core specialization
	using PropertyBool = TProperty<bool>;
	using PropertyInt = TProperty<int32>;
	using PropertyFloat = TProperty<float>;
	using PropertyDouble = TProperty<double>;
	using PropertyString = TProperty<String>;
	using PropertyRectF = TProperty<RectF>;
	using PropertyRectI = TProperty<RectI>;
	using PropertyRectU = TProperty<RectU>;
	using PropertyStringVec = TProperty<StringVec>;

	// types
	using half = int16;

	using ReflectedFieldID_t = int16;
	using ReflectedTypeID_t = int64;
	using ReflectedSize_t = int64;

	// Reflection ID
	namespace refl
	{
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
			RTI_RectF,
			RTI_RectD,
			RTI_RectI,
			RTI_RectU,

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
			RTI_UUID = 100,
			RTI_Property,

		};

		template<class T> class PlainType {  };
		template<class T> class ContainerType {  };
		template<class T> class ComplexType;

		template<class T> struct TypeInfo { static constexpr ReflectedTypeID_t ID = RTI_Unknown; using Type = void; static constexpr StringView Name = "Unknown"sv; };
		template<class T> using TypeInfo_t = TypeInfo<RemoveEverything_t<T>>;
	}
}
#include "Base/ReflectedConversions.h"

#endif /* CORE_PREREQUISITES_H */