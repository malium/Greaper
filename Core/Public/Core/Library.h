/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
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

		inline EmptyResult Open(StringView libraryName)
		{
			if (!IsOpen())
			{
				auto res = OSLibrary::Load(libraryName);
				if (res.IsOk())
				{
					m_Handle = res.GetValue();
					return Result::CreateSuccess();
				}
				m_Handle = nullptr;
				return Result::CopyFailure(res);
			}
			return Result::CreateSuccess();
		}

		inline EmptyResult Open(WStringView libraryName)
		{
			if (!IsOpen())
			{
				auto res = OSLibrary::Load(libraryName);
				if (res.IsOk())
				{
					m_Handle = res.GetValue();
					return Result::CreateSuccess();
				}
				m_Handle = nullptr;
				return Result::CopyFailure(res);
			}
			return Result::CreateSuccess();
		}

		inline EmptyResult Close()
		{
			if (IsOpen())
			{
				auto res = OSLibrary::Unload(m_Handle);
				m_Handle = nullptr;
				return res;
			}
			return Result::CreateSuccess();
		}

		INLINE constexpr bool IsOpen()const noexcept
		{
			return m_Handle != nullptr;
		}

		INLINE OSLibrary::LibraryHandle GetOSHandle()const noexcept
		{
			return m_Handle;
		}

		inline TResult<FuncPtr> GetFunction(StringView funcName)const noexcept
		{
			if (IsOpen())
				return OSLibrary::FuncLoad(m_Handle, std::move(funcName));

			return Result::CreateFailure<FuncPtr>(Format("Couldn't obtain the function '%s', the library was closed.", funcName.data()));
		}
		
		template<typename retType = void, class... types>
		inline TResult<typename FuncType<retType, types...>::Type> GetFunctionT(StringView funcName)const noexcept
		{
			using FuncType = typename FuncType<retType, types...>::Type;
			auto res = GetFunction(std::move(funcName));
			if (res.IsOk())
				return Result::CreateSuccess(reinterpret_cast<FuncType>(res.GetValue()));
			return Result::CopyFailure<FuncType>(res);
		}

		Library(const Library&) = delete;
		Library& operator=(const Library&) = delete;
	};

	using WLibrary = WPtr<Library>;
	using PLibrary = SPtr<Library>;
}

#endif /* CORE_LIBRARY_H */
