/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_TYPETRAITS_H
#define CORE_TYPETRAITS_H 1

#include "../Memory.h"
#include <type_traits>

namespace greaper
{
	template<class T> struct RemoveArray { using Type = T; };
	template<class T, std::size_t Sz> struct RemoveArray<T[Sz]> { using Type = T; };
	template<class T> struct RemoveArray<T[]> { using Type = T; };

	template<class T> using RemoveArray_t = typename RemoveArray<T>::Type;

	template<class T> struct RemoveConst { using Type = T; };
	template<class T> struct RemoveConst<const T> { using Type = T; };
	template<class T> struct RemoveConst<const T&> { using Type = T&; };
	template<class T> struct RemoveConst<const T*> { using Type = T*; };

	template<class T> using RemoveConst_t = typename RemoveConst<T>::Type;

	template<class T> struct RemoveReference { using Type = T; };
	template<class T> struct RemoveReference<T&> { using Type = T; };
	template<class T> struct RemoveReference<T&&> { using Type = T; };

	template<class T> using RemoveReference_t = typename RemoveReference<T>::Type;

	template<class T> struct RemovePointer { using Type = T; };
	template<class T> struct RemovePointer<T*> { using Type = T; };
	template<class T> struct RemovePointer<T**> { using Type = T; };

	template<class T> using RemovePointer_t = typename RemovePointer<T>::Type;

	template<class T> struct RemoveSmartPointer { using Type = T; };
	template<class T> struct RemoveSmartPointer<UPtr<T>> { using Type = T; };
	template<class T> struct RemoveSmartPointer<SPtr<T>> { using Type = T; };
	template<class T> struct RemoveSmartPointer<WPtr<T>> { using Type = T; };

	template<class T> using RemoveSmartPointer_t = typename RemoveSmartPointer<T>::Type;

	template<class T> using RemoveEverything_t = RemoveConst_t<RemoveReference_t<RemovePointer_t<RemoveArray_t<RemoveSmartPointer_t<T>>>>>;
}

#endif /* CORE_TYPETRAITS_H */