/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper
{
	template<class T>
	class WeakPointer
	{
		mutable Impl::ISharedPointerControl* m_Control;

		template<class T, class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type>
		friend bool operator==(const WeakPointer<T>& left, const WeakPointer<T2>& right)noexcept;

		template<class T2>
		friend class WeakPointer;
	public:
		INLINE WeakPointer()noexcept
			:m_Control(nullptr)
		{

		}
		template<class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE WeakPointer(const SharedPointer<T2>& shared) noexcept
			:m_Control(shared.m_Control)
		{
			if (m_Control != nullptr)
				m_Control->AddWeakReference();
		}
		INLINE WeakPointer(const WeakPointer& other)noexcept
			:m_Control(other.m_Control)
		{
			if (m_Control != nullptr)
				m_Control->AddWeakReference();
		}
		template<class T2, std::enable_if<(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>) && !std::is_same_v<T2, T>, bool>::type = true>
		INLINE WeakPointer(const WeakPointer<T2>& other) noexcept
			:m_Control(other.m_Control)
		{
			if (m_Control != nullptr)
				m_Control->AddWeakReference();
		}
		INLINE WeakPointer(WeakPointer&& other) noexcept
			:m_Control(other.m_Control)
		{
			other.m_Control = nullptr;
		}
		template<class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE WeakPointer(WeakPointer<T2>&& other) noexcept
			:m_Control(other.m_Control)
		{
			other.m_Control = nullptr;
		}
		INLINE WeakPointer& operator=(const WeakPointer& other) noexcept
		{
			if (this != &other)
			{
				if (m_Control != nullptr)
					m_Control->DecWeakReference();
				
				m_Control = other.m_Control;
				if (m_Control != nullptr)
					m_Control->AddWeakReference();
			}
			return *this;
		}
		template<class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE WeakPointer& operator=(const WeakPointer<T2>& other) noexcept
		{
			if (((void*)this) != ((void*)&other))
			{
				if (m_Control != nullptr)
					m_Control->DecWeakReference();

				m_Control = other.m_Control;
				if (m_Control != nullptr)
					m_Control->AddWeakReference();
			}
			return *this;
		}
		INLINE WeakPointer& operator=(WeakPointer&& other) noexcept
		{
			if (this != &other)
			{
				if (m_Control != nullptr)
					m_Control->DecWeakReference();
				m_Control = other.m_Control;
				other.m_Control = nullptr;
			}
			return *this;
		}
		template<class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE WeakPointer& operator=(WeakPointer<T2>&& other) noexcept
		{
			if (((void*)this) != ((void*)&other))
			{
				if (m_Control != nullptr)
					m_Control->DecWeakReference();
				m_Control = other.m_Control;
				other.m_Control = nullptr;
			}
			return *this;
		}
		INLINE ~WeakPointer() noexcept
		{
			if (m_Control != nullptr)
				m_Control->DecWeakReference();
		}

		INLINE SharedPointer<T> Lock()const noexcept
		{
			SharedPointer<T> shared;

			if (Expired())
				return shared;

			m_Control->AddSharedReference();
			shared.m_Control = m_Control;
			shared.m_Value = reinterpret_cast<T*>(m_Control->GetValue());

			return shared;
		}
		INLINE bool Expired()const noexcept
		{
			return m_Control == nullptr || m_Control->SharedRefCount() <= 0;
		}
		template<class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE void Swap(WeakPointer<T2>& other)noexcept
		{
			auto* tempControl = m_Control;
			m_Control = other.m_Control;
			other.m_Control = tempControl;
		}
		INLINE void Reset()noexcept
		{
			if (m_Control != nullptr)
			{
				m_Control->DecWeakReference();
				m_Control = nullptr;
			}
		}
		INLINE uint32 SharedRefCount()const noexcept
		{
			if (m_Control != nullptr)
				return m_Control->SharedRefCount();
			return 0;
		}
		INLINE uint32 WeakRefCount()const noexcept
		{
			if (m_Control != nullptr)
				return m_Control->WeakRefCount();
			return 0;
		}
	};

	template<class T, class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
	INLINE bool operator==(const WeakPointer<T>& left, const WeakPointer<T2>& right)noexcept
	{
		return left.m_Control == right.m_Control;
	}
	template<class T, class T2, std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
	INLINE bool operator!=(const WeakPointer<T>& left, const WeakPointer<T2>& right)noexcept
	{
		return !(left == right);
	}
	template<class T>
	INLINE bool operator==(const WeakPointer<T>& left, std::nullptr_t) noexcept
	{
		return left.Expired();
	}
	template<class T>
	INLINE bool operator!=(const WeakPointer<T>&left, std::nullptr_t) noexcept
	{
		return !(left == nullptr);
	}

	template<class T>
	using WPtr = WeakPointer<T>;
}