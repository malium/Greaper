/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include <atomic>

namespace greaper
{
	template<class T>
	class SharedPointer;

	namespace Impl
	{
		enum class SPtrType
		{
			SingleThread,
			MultiThread
		};

		template<class T>
		using SPtrDeleterFn_t = std::function<void(T*)>;

		class ISharedPointerControl
		{
		public:
			virtual ~ISharedPointerControl()noexcept = default;

			virtual void AddSharedReference()noexcept = 0;
			virtual void DecSharedReference()noexcept = 0;
			virtual void AddWeakReference()noexcept = 0;
			virtual void DecWeakReference()noexcept = 0;
			virtual uint32 SharedRefCount()const noexcept = 0;
			virtual uint32 WeakRefCount()const noexcept = 0;
			virtual void* GetValue()const noexcept = 0;
			virtual SPtrType GetType()const noexcept = 0;
		};

		template<class T>
		class SharedPointerControlST final : public ISharedPointerControl
		{
		public:
			INLINE SharedPointerControlST(T* value, SPtrDeleterFn_t<T> deleter) noexcept
				:m_Value(value)
				, m_SharedReferences(0)
				, m_WeakReferences(0)
				, m_Deleter(std::move(deleter))
			{

			}
			INLINE void AddSharedReference() noexcept override
			{
				++m_SharedReferences;
			}
			INLINE void DecSharedReference() noexcept override
			{
				--m_SharedReferences;
				if (m_SharedReferences <= 0)
				{
					m_Deleter(m_Value);
					m_Value = nullptr;

					if (m_WeakReferences <= 0)
					{
						PlatformDealloc(this);
					}
				}
			}
			INLINE void AddWeakReference() noexcept override
			{
				++m_WeakReferences;
			}
			INLINE void DecWeakReference() noexcept override
			{
				--m_WeakReferences;
				if (m_WeakReferences <= 0 && m_SharedReferences <= 0)
				{
					PlatformDealloc(this);
				}
			}
			INLINE uint32 SharedRefCount()const noexcept override { return m_SharedReferences; }
			INLINE uint32 WeakRefCount()const noexcept override { return m_WeakReferences; }
			INLINE void* GetValue()const noexcept override { return m_Value; }
			INLINE SPtrType GetType()const noexcept override { return SPtrType::SingleThread; }

		private:
			mutable T* m_Value;
			uint32 m_SharedReferences;
			uint32 m_WeakReferences;
			SPtrDeleterFn_t<T> m_Deleter;
		};

		template<class T>
		class SharedPointerControlMT final : public ISharedPointerControl
		{
		public:
			INLINE SharedPointerControlMT(T* value, SPtrDeleterFn_t<T> deleter) noexcept
				:m_Value(value)
				, m_SharedReferences(0)
				, m_WeakReferences(0)
				, m_Deleter(std::move(deleter))
			{

			}
			INLINE void AddSharedReference() noexcept override
			{
				++m_SharedReferences;
			}
			INLINE void DecSharedReference() noexcept override
			{
				--m_SharedReferences;
				if (m_SharedReferences <= 0)
				{
					m_Deleter(m_Value);
					m_Value = nullptr;

					if (m_WeakReferences <= 0)
					{
						PlatformDealloc(this);
					}
				}
			}
			INLINE void AddWeakReference() noexcept override
			{
				++m_WeakReferences;
			}
			INLINE void DecWeakReference() noexcept override
			{
				--m_WeakReferences;
				if (m_WeakReferences <= 0 && m_SharedReferences <= 0)
				{
					PlatformDealloc(this);
				}
			}
			INLINE uint32 SharedRefCount()const noexcept override { return m_SharedReferences; }
			INLINE uint32 WeakRefCount()const noexcept override { return m_WeakReferences; }
			INLINE void* GetValue()const noexcept override { return m_Value; }
			INLINE SPtrType GetType()const noexcept override { return SPtrType::MultiThread; }

		private:
			mutable T* m_Value;
			std::atomic_uint32_t m_SharedReferences;
			std::atomic_uint32_t m_WeakReferences;
			SPtrDeleterFn_t<T> m_Deleter;
		};
	}

	template<class T>
	class SharedPointer
	{		
	private:
		void CreateControl(Impl::SPtrDeleterFn_t<T> deleter, Impl::SPtrType threading)
		{
			using namespace Impl;

			if (threading == SPtrType::MultiThread)
			{
				void* mem = PlatformAlloc(sizeof(SharedPointerControlMT<T>));// , 16);
				new(mem)SharedPointerControlMT<T>(m_Value, std::move(deleter));
				m_Control = (SharedPointerControlMT<T>*)mem;
			}
			else if (threading == SPtrType::SingleThread)
			{
				void* mem = PlatformAlloc(sizeof(SharedPointerControlST<T>));// , 16);
				new(mem)SharedPointerControlST<T>(m_Value, std::move(deleter));
				m_Control = (SharedPointerControlST<T>*)mem;
			}
			m_Control->AddSharedReference();
		}

		template<class T2>
		friend class SharedPointer;

	public:
		using Type = T;
		using Pointer = T*;

		INLINE constexpr SharedPointer() noexcept
			:m_Value(nullptr)
			,m_Control(nullptr)
		{

		}
		explicit INLINE SharedPointer(T* value, Impl::SPtrDeleterFn_t<T> deleter = &Impl::DefaultDeleter<T>, Impl::SPtrType threading = Impl::SPtrType::MultiThread)noexcept
			: m_Value(value)
			, m_Control(nullptr)
		{
			if (m_Value != nullptr)
				CreateControl(std::move(deleter), threading);
		}
		template<class T2, typename std::enable_if<(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>) && !std::is_same_v<T2, T>, bool>::type = true>
		explicit INLINE SharedPointer(T2* value, Impl::SPtrDeleterFn_t<T> deleter = &Impl::DefaultDeleter<T>, Impl::SPtrType threading = Impl::SPtrType::MultiThread)noexcept
			:m_Value((T*)value)
			,m_Control(nullptr)
		{
			if (m_Value != nullptr)
				CreateControl(std::move(deleter), threading);
		}
		INLINE SharedPointer(const SharedPointer& other)noexcept
			:m_Value(other.m_Value)
			, m_Control(other.m_Control)
		{
			if (m_Value != nullptr && m_Control != nullptr)
				m_Control->AddSharedReference();
		}
		template<class T2, typename std::enable_if<(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>) && !std::is_same_v<T2, T>, bool>::type = true>
		INLINE explicit SharedPointer(const SharedPointer<T2>& other)noexcept
			:m_Value((T*)other.m_Value)
			,m_Control(other.m_Control)
		{
			if (m_Value != nullptr && m_Control != nullptr)
				m_Control->AddSharedReference();
		}
		INLINE SharedPointer(SharedPointer&& other)noexcept
			:m_Value(other.m_Value)
			,m_Control(other.m_Control)
		{
			other.m_Value = nullptr;
			other.m_Control = nullptr;
		}
		template<class T2, typename std::enable_if<(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>) && !std::is_same_v<T2, T>, bool>::type = true>
		INLINE explicit SharedPointer(SharedPointer<T2>&& other)noexcept
			:m_Value((T*)other.m_Value)
			,m_Control(other.m_Control)
		{
			other.m_Value = nullptr;
			other.m_Control = nullptr;
		}
		INLINE ~SharedPointer()noexcept
		{
			if (m_Control != nullptr)
				m_Control->DecSharedReference();
		}
		INLINE SharedPointer& operator=(const SharedPointer& other)noexcept
		{
			if (this != &other)
			{
				if (m_Control != nullptr)
					m_Control->DecSharedReference();

				m_Control = other.m_Control;
				m_Value = other.m_Value;
				if (m_Control != nullptr)
					m_Control->AddSharedReference();
			}
			return *this;
		}
		template<class T2, typename std::enable_if<(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>) && !std::is_same_v<T2, T>, bool>::type = true>
		INLINE SharedPointer& operator=(const SharedPointer<T2>& other)noexcept
		{
			if (((void*)this) != ((void*)&other))
			{
				if (m_Control != nullptr)
					m_Control->DecSharedReference();

				m_Control = other.m_Control;
				m_Value = (T*)other.m_Value;
				if (m_Control != nullptr)
					m_Control->AddSharedReference();
			}
			return *this;
		}
		INLINE SharedPointer& operator=(SharedPointer&& other)noexcept
		{
			if (this != &other)
			{
				if (m_Control != nullptr)
					m_Control->DecSharedReference();
				m_Control = other.m_Control;
				other.m_Control = nullptr;
				m_Value = other.m_Value;
				other.m_Value = nullptr;
			}
			return *this;
		}
		template<class T2, typename std::enable_if<(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>) && !std::is_same_v<T2, T>, bool>::type = true>
		INLINE SharedPointer& operator=(SharedPointer<T2>&& other)noexcept
		{
			if (((void*)this) != ((void*)&other))
			{
				if (m_Control != nullptr)
					m_Control->DecSharedReference();
				m_Control = other.m_Control;
				other.m_Control = nullptr;
				m_Value = (T*)other.m_Value;
				other.m_Value = nullptr;
			}
			return *this;
		}
		template<class T2, typename std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE void reset(T2* value, Impl::SPtrDeleterFn_t<T> deleter = &Impl::DefaultDeleter<T>, Impl::SPtrType threading = Impl::SPtrType::MultiThread) noexcept
		{
			if (m_Control != nullptr)
			{
				m_Control->DecSharedReference();
				m_Control = nullptr;
			}
			m_Value = (T*)value;
			if (m_Value != nullptr)
				CreateControl(std::move(deleter), threading);
		}
		INLINE void reset()
		{
			if (m_Control != nullptr)
			{
				m_Control->DecSharedReference();
			}
			m_Control = nullptr;
			m_Value = nullptr;
		}
		template<class T2, typename std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE explicit operator SharedPointer<T2>()const
		{
			SharedPointer<T2> shared;
			shared.m_Control = m_Control;
			shared.m_Value = (T2*)m_Value;
			if (m_Control != nullptr)
				m_Control->AddSharedReference();
			return shared;
		}
		template<class T2, typename std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
		INLINE void swap(SharedPointer<T2>& other) noexcept
		{
			auto tempControl = m_Control;
			auto tempValue = m_Value;
			m_Control = other.m_Control;
			m_Value = other.m_Value;
			other.m_Control = tempControl;
			other.m_Value = tempValue;
		}
		INLINE uint32 RefCount()const noexcept
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
		INLINE T* get()const noexcept { return m_Value; }
		INLINE T* operator->()const noexcept
		{
			VerifyNotNull(m_Value, "SPtr: Trying to access a nullptr.");
			return m_Value;
		}
		INLINE T& operator*() const noexcept
		{
			VerifyNotNull(m_Value, "SPtr: Trying to access a nullptr.");
			return *m_Value;
		}

	private:
		mutable T* m_Value;
		mutable Impl::ISharedPointerControl* m_Control;

		template<class T2>
		friend class WeakPointer;
	};

	template<class T, class T2, typename std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
	INLINE bool operator==(const SharedPointer<T>& left, const SharedPointer<T2>& right)noexcept
	{
		return left.get() == right.get();
	}
	template<class T, class T2, typename std::enable_if<std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>, bool>::type = true>
	INLINE bool operator!=(const SharedPointer<T>& left, const SharedPointer<T2>& right)noexcept
	{
		return !(left == right);
	}
	template<class T>
	INLINE bool operator==(const SharedPointer<T>& left, std::nullptr_t)
	{
		return left.get() == nullptr;
	}
	template<class T>
	INLINE bool operator!=(const SharedPointer<T>& left, std::nullptr_t)
	{
		return !(left == nullptr);
	}

	template<class T>
	using SPtr = SharedPointer<T>;
}