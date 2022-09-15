/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_RESULT_H
#define CORE_RESULT_H 1

#include "CorePrerequisites.h"
#include "Memory.h"

namespace greaper
{
	template<class T> class Result;
	struct EmptyStruct {  };
	template<class T>
	class Result
	{
		String m_FailMessage;
		T m_Value;
		bool m_Failure = false;

		template<class _T_> friend Result<_T_> CreateResult(_T_)noexcept;
		template<class _T_> friend Result<_T_> CreateFailure(StringView)noexcept;
		template<class _T_> friend Result<_T_> CreateFailure(const String&)noexcept;
		template<class _T_, class _U_> friend Result<_T_> CopyFailure(Result<_U_>)noexcept;

		Result() = default;

	public:
		bool IsOk()const noexcept { return !m_Failure; }

		bool HasFailed()const noexcept { return m_Failure; }

		const String& GetFailMessage()const noexcept { return m_FailMessage; }

		T& GetValue() noexcept;

		const T& GetValue()const noexcept;
	};

	using EmptyResult = Result<EmptyStruct>;

	template<class T>
	INLINE Result<T> CreateResult(T value) noexcept
	{
		Result<T> res;
		res.m_Value = std::move(value);
		res.m_Failure = false;
		return res;
	}

	INLINE Result<EmptyStruct> CreateEmptyResult() noexcept
	{
		return CreateResult<EmptyStruct>(EmptyStruct{});
	}

	template<class T>
	INLINE Result<T> CreateFailure(StringView errorMessage) noexcept
	{
		Result<T> res;
		res.m_FailMessage.assign(errorMessage);
		res.m_Failure = true;
		return res;
		//return std::move(res);
	}

	template<class T>
	INLINE Result<T> CreateFailure(const String& errorMessage) noexcept
	{
		Result<T> res;
		res.m_FailMessage = errorMessage;
		res.m_Failure = true;
		return res;
		//return std::move(res);
	}

	INLINE Result<EmptyStruct> CreateEmptyFailure(StringView errorMessage) noexcept
	{
		return CreateFailure<EmptyStruct>(errorMessage);
	}

	template<class T, class U = T>
	INLINE Result<T> CopyFailure(Result<U> res) noexcept
	{
		Result<T> r;
		r.m_Failure = true;
		r.m_FailMessage = std::move(res.m_FailMessage);
		return std::move(r);
	}

	template<class T>
	INLINE T& greaper::Result<T>::GetValue() noexcept
	{
		Verify(IsOk(), "Trying to optain a failed result, msg: '%s'.", m_FailMessage.c_str());
		return m_Value;
	}
	
	template<class T>
	INLINE const T& greaper::Result<T>::GetValue() const noexcept
	{
		Verify(IsOk(), "Trying to optain a failed result, msg: '%s'.", m_FailMessage.c_str());
		return m_Value;
	}
}

#endif /* CORE_RESULT_H */