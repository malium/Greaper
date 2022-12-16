/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper
{
	template<class T>
	class UPtr
	{
	public:
		using Type = T;
		using Pointer = T*;
		using DeleteFN = std::function<void(T*)>;

		INLINE constexpr UPtr() noexcept
			:m_Value(nullptr)
			,m_Deleter(nullptr)
		{

		}
		INLINE explicit UPtr(T* value, DeleteFN deleteFn = &Impl::DefaultDeleter<T>) noexcept
			:m_Value(value)
			,m_Deleter(std::move(deleteFn))
		{

		}
		INLINE UPtr(UPtr&& other) noexcept
			:m_Value(nullptr)
			,m_Deleter(nullptr)
		{
			Swap(other);
		}
		INLINE UPtr& operator=(UPtr&& other) noexcept
		{
			if (this != &other)
			{
				Swap(other);
			}
			return *this;
		}
		INLINE ~UPtr() noexcept
		{
			Reset(nullptr);
		}
		INLINE void Swap(UPtr& other) noexcept
		{
			auto tempVal = m_Value;
			DeleteFN&& tempDel = std::move(m_Deleter);
			m_Value = other.m_Value;
			m_Deleter = std::move(other.m_Deleter);
			other.m_Value = tempVal;
			other.m_Deleter = std::move(tempDel);
		}
		NODISCARD INLINE T* Release() noexcept
		{
			T* temp = m_Value;
			m_Value = nullptr;
			return temp;
		}
		INLINE void Reset(T* value, DeleteFN deleteFn = &Impl::DefaultDeleter<T>) noexcept
		{
			if (m_Value != nullptr)
			{
				m_Deleter(m_Value);
				m_Value = nullptr;
			}
			m_Value = value;
			m_Deleter = std::move(deleteFn);
		}
		NODISCARD INLINE T* Get()const noexcept { return m_Value; }
		INLINE  T* operator->() const noexcept
		{
			VerifyNotNull(m_Value, "UPtr: Trying to access a nullptr.");
			return m_Value;
		}
		INLINE T& operator*() const noexcept
		{
			VerifyNotNull(m_Value, "UPtr: Trying to access a nullptr.");
			return *m_Value;
		}

		UPtr(const UPtr&) = delete;
		UPtr& operator=(const UPtr&) = delete;

	private:
		mutable T* m_Value;
		DeleteFN m_Deleter;
	};

	template<class T>
	INLINE bool operator==(const UPtr<T>& left, const UPtr<T>& right) noexcept { return left.Get() == right.Get(); }
	template<class T>
	INLINE bool operator!=(const UPtr<T>& left, const UPtr<T>& right) noexcept { return !(left == right); }
	template<class T>
	INLINE bool operator==(const UPtr<T>& left, std::nullptr_t) { return left.Get() == nullptr; }
	template<class T>
	INLINE bool operator!=(const UPtr<T>& left, std::nullptr_t) { return !(left == nullptr); }
}