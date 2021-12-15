/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LIBRARY_H
#define CORE_LIBRARY_H 1

#include "CorePrerequisites.h"
#include "Memory.h"

#if PLT_WINDOWS
#include "Win/WinLibrary.h"
#else
#include "Lnx/LnxLibrary.h"
#endif

namespace greaper
{
	template<class retType, class... types>
	struct FuncType
	{
		using Type = retType(*)(types...);
	};
	class Library
	{
		OSLibrary::LibraryHandle m_Handle;

	public:
		INLINE constexpr Library()noexcept
			:m_Handle(nullptr)
		{
			
		}

		INLINE Library(WStringView libraryName)noexcept
			:m_Handle(nullptr)
		{
			Open(libraryName);
		}

		INLINE Library(StringView libraryName)noexcept
			:m_Handle(nullptr)
		{
			Open(libraryName);
		}

		INLINE Library(OSLibrary::LibraryHandle handle)
			: m_Handle(handle)
		{

		}

		constexpr INLINE Library(Library&& other)noexcept
			:m_Handle(other.m_Handle)
		{
			other.m_Handle = nullptr;
		}

		constexpr INLINE Library& operator=(Library&& other)noexcept
		{
			if(this != &other)
			{
				Close();
				m_Handle = other.m_Handle;
				other.m_Handle = nullptr;
			}
			return *this;
		}

		INLINE void Open(StringView libraryName)
		{
			if (IsOpen())
				return;
			m_Handle = OSLibrary::Load(libraryName);
		}

		INLINE void Open(WStringView libraryName)
		{
			if (IsOpen())
				return;
			m_Handle = OSLibrary::Load(libraryName);
		}

		INLINE void Close()
		{
			if (!IsOpen())
				return;
			OSLibrary::Unload(m_Handle);
			m_Handle = nullptr;
		}

		INLINE constexpr bool IsOpen()const noexcept
		{
			return m_Handle != nullptr;
		}

		INLINE OSLibrary::LibraryHandle GetOSHandle()const noexcept
		{
			return m_Handle;
		}

		INLINE FuncPtr GetFunction(StringView funcName)const noexcept
		{
			if (!IsOpen())
				return nullptr;
			return OSLibrary::FuncLoad(m_Handle, funcName);
		}
		
		template<typename retType = void, class... types>
		INLINE typename FuncType<retType, types...>::Type GetFunctionT(StringView funcName)const noexcept
		{
			//if (!IsOpen())
			//	return nullptr;

			return reinterpret_cast<typename FuncType<retType, types...>::Type>(GetFunction(funcName));
		}

		Library(const Library&) = delete;
		Library& operator=(const Library&) = delete;
	};
}

#endif /* CORE_LIBRARY_H */
