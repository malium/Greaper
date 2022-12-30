/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_RESULT_H
#define CORE_RESULT_H 1

#include "CorePrerequisites.h"
#include "Memory.h"

namespace greaper
{
	class Result;
	class Return;
	namespace Impl
	{
		template<class T>
		class TResult
		{
			String m_FailMessage;
			T m_Value;
			bool m_Failure;

		public:
			TResult()noexcept = default;

			NODISCARD INLINE bool IsOk()const noexcept { return !m_Failure; }

			NODISCARD INLINE bool HasFailed()const noexcept { return m_Failure; }

			NODISCARD INLINE const String& GetFailMessage()const noexcept { return m_FailMessage; }

			NODISCARD INLINE T& GetValue() noexcept
			{
				Verify(IsOk(), "Trying to optain a failed result, msg: '%s'.", m_FailMessage.c_str());
				return m_Value;
			}

			NODISCARD INLINE const T& GetValue()const noexcept
			{
				Verify(IsOk(), "Trying to optain a failed result, msg: '%s'.", m_FailMessage.c_str());
				return m_Value;
			}
			friend class greaper::Result;
		};

		template<>
		class TResult<EmptyStruc>
		{
			String m_FailMessage;
			bool m_Failure;

			static EmptyStruc gRtn;

		public:
			INLINE TResult()noexcept = default;

			NODISCARD INLINE bool IsOk()const noexcept { return !m_Failure; }

			NODISCARD INLINE bool HasFailed()const noexcept { return m_Failure; }

			NODISCARD INLINE const String& GetFailMessage()const noexcept { return m_FailMessage; }

			NODISCARD INLINE EmptyStruc& GetValue() noexcept
			{
				static_assert("should not reach here");
				return gRtn;
			}

			NODISCARD INLINE const EmptyStruc& GetValue()const noexcept
			{
				static_assert("should not reach here");
				return gRtn;
			}
			friend class greaper::Result;
		};

		template<class T>
		class TReturn
		{
			T m_Value{};
			bool m_Failure{};

		public:
			INLINE constexpr TReturn()noexcept : m_Failure(true) {  }
			INLINE constexpr TReturn(T value)noexcept : m_Value(value), m_Failure(false) {  }

			NODISCARD INLINE constexpr bool IsOk()const noexcept { return !m_Failure; }

			NODISCARD INLINE constexpr bool HasFailed()const noexcept { return m_Failure; }

			NODISCARD INLINE constexpr const T& GetValue()const noexcept
			{
				Verify(IsOk(), "Trying to optain a failed return");
				return m_Value;
			}
			friend class greaper::Return;
		};
	}

	class Result
	{
	public:
		template<class T>
		NODISCARD INLINE static TResult<T> CreateSuccess(T value) noexcept
		{
			TResult<T> res;
			res.m_Failure = false;
			res.m_Value = std::move(value);
			return res;
		}

		NODISCARD INLINE static TResult<Impl::EmptyStruc> CreateSuccess() noexcept
		{
			TResult<Impl::EmptyStruc> res;
			res.m_Failure = false;
			return res;
		}

		NODISCARD INLINE static TResult<Impl::EmptyStruc> CreateFailure(String errorMessage) noexcept
		{
			TResult<Impl::EmptyStruc> res;
			res.m_Failure = true;
			res.m_FailMessage = std::move(errorMessage);
			return res;
		}

		NODISCARD INLINE static TResult<Impl::EmptyStruc> CreateFailure(StringView errorMessage) noexcept
		{
			TResult<Impl::EmptyStruc> res;
			res.m_Failure = true;
			res.m_FailMessage.assign(errorMessage);
			return res;
		}

		template<class T>
		NODISCARD INLINE static TResult<T> CreateFailure(String errorMessage) noexcept
		{
			TResult<T> res;
			res.m_Failure = true;
			res.m_FailMessage = std::move(errorMessage);
			return res;
		}

		template<class T>
		NODISCARD INLINE static TResult<T> CreateFailure(StringView errorMessage) noexcept
		{
			TResult<T> res;
			res.m_Failure = true;
			res.m_FailMessage.assign(errorMessage);
			return res;
		}

		template<class T, class T2>
		NODISCARD INLINE static TResult<T> CopyFailure(const TResult<T2>& other)noexcept
		{
			TResult<T> res;
			res.m_FailMessage.assign(other.GetFailMessage());
			res.m_Failure = true;
			return res;
		}

		template<class T>
		NODISCARD INLINE static TResult<Impl::EmptyStruc> CopyFailure(const TResult<T>& other)noexcept
		{
			TResult<Impl::EmptyStruc> res;
			res.m_FailMessage.assign(other.GetFailMessage());
			res.m_Failure = true;
			return res;
		}
	};
	class Return
	{
	public:
		template<class T>
		NODISCARD INLINE constexpr static TReturn<T> CreateSuccess(T value)noexcept
		{
			return TReturn<T>(value);
		}
		template<class T>
		NODISCARD INLINE constexpr static TReturn<T> CreateFailure()noexcept
		{
			return TReturn<T>{};
		}
		template<class T, class T2>
		NODISCARD INLINE constexpr static TReturn<T> CopyFailure(const TReturn<T2>& other)noexcept
		{
			return TReturn<T>{};
		}
	};
}

#endif /* CORE_RESULT_H */