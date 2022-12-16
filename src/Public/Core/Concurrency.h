/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_CONCURRENCY_H
#define CORE_CONCURRENCY_H 1

#include "CorePrerequisites.h"

#if PLT_WINDOWS
#include "Win/WinThreading.h"
#elif PLT_LINUX
#include "Lnx/LnxThreading.h"
#endif
#include <atomic>
#include <any>

/*** Cross-platform concurrency primitives and utilites
*	
*	Greaper requires some concurrency primities to work and that those work as similar as possible
*	on each platform Greaper is supported
*	
*	Currently, Linux platform does not support RWMutex interaction with Signals
*/
namespace greaper
{
	/*** Typical Mutex
	*
	*	To be added:
	*	In debug builds or activated by configuration, information of
	*	which thread owns the mutex, so when a deadlock occurs we can 
	*	see which threads are colliding with their info
	*/
	class Mutex
	{
		MutexHandle m_Handle;
	public:
		INLINE Mutex() noexcept
		{
			Impl::MutexImpl::Initialize(m_Handle);
		}

		Mutex(const Mutex&) = delete;
		Mutex& operator=(const Mutex&) = delete;
		INLINE Mutex(Mutex&& other) noexcept
		{

			DuplicateMemory(other.m_Handle, m_Handle);
			Impl::MutexImpl::Invalidate(other.m_Handle);
		}
		INLINE Mutex& operator=(Mutex&& other)noexcept
		{
			if (this != &other)
			{
				if (Impl::MutexImpl::IsValid(m_Handle))
					Impl::MutexImpl::Deinitialize(m_Handle);
				DuplicateMemory(other.m_Handle, m_Handle);
				Impl::MutexImpl::Invalidate(other.m_Handle);
			}
			return *this;
		}
		INLINE ~Mutex()
		{
			if (Impl::MutexImpl::IsValid(m_Handle))
				Impl::MutexImpl::Deinitialize(m_Handle);
		}

		INLINE void lock() noexcept
		{
			Verify(Impl::MutexImpl::IsValid(m_Handle), "Trying to lock an invalid mutex.");
			Impl::MutexImpl::Lock(m_Handle);
		}

		INLINE bool try_lock() noexcept
		{
			if (!Impl::MutexImpl::IsValid(m_Handle))
				return false;
			return Impl::MutexImpl::TryLock(m_Handle);
		}

		INLINE void unlock() noexcept
		{
			Verify(Impl::MutexImpl::IsValid(m_Handle), "Trying to unlock an invalid mutex.");
			return Impl::MutexImpl::Unlock(m_Handle);
		}

		NODISCARD INLINE const MutexHandle* GetHandle()const noexcept
		{
			return &m_Handle;
		}
		NODISCARD INLINE MutexHandle* GetHandle() noexcept
		{
			return &m_Handle;
		}
	};

	/*** Mutex that allows recursion
	*	Multiple locking by the same thread -> expects the same unlockings afterwards
	*
	*	To be added:
	*	In debug builds or activated by configuration, information of
	*	which thread owns the mutex, so when a deadlock occurs we can
	*	see which threads are colliding with their info. Also a counter
	*	that tells how many lockings have done the lock thread.
	*/
	class RecursiveMutex
	{
		RecursiveMutexHandle m_Handle;

	public:
		INLINE RecursiveMutex() noexcept
		{
			Impl::RecursiveMutexImpl::Initialize(m_Handle);
		}
		RecursiveMutex(const RecursiveMutex&) = delete;
		RecursiveMutex& operator=(const RecursiveMutex&) = delete;
		INLINE RecursiveMutex(RecursiveMutex&& other) noexcept
		{
			DuplicateMemory(other.m_Handle, m_Handle);
			Impl::RecursiveMutexImpl::Invalidate(other.m_Handle);
		}
		INLINE RecursiveMutex& operator=(RecursiveMutex&& other) noexcept
		{
			if (this != &other)
			{
				if (Impl::RecursiveMutexImpl::IsValid(m_Handle))
					Impl::RecursiveMutexImpl::Deinitialize(m_Handle);
				DuplicateMemory(other.m_Handle, m_Handle);
				Impl::RecursiveMutexImpl::Invalidate(other.m_Handle);
			}
			return *this;
		}
		INLINE ~RecursiveMutex()
		{
			if (Impl::RecursiveMutexImpl::IsValid(m_Handle))
				Impl::RecursiveMutexImpl::Deinitialize(m_Handle);
		}

		INLINE void lock() noexcept
		{
			Verify(Impl::RecursiveMutexImpl::IsValid(m_Handle), "Trying to lock an invalid recursive mutex");
			Impl::RecursiveMutexImpl::Lock(m_Handle);
		}

		INLINE bool try_lock() noexcept
		{
			if (!Impl::RecursiveMutexImpl::IsValid(m_Handle))
				return false;
			return Impl::RecursiveMutexImpl::TryLock(m_Handle);
		}

		INLINE void unlock() noexcept
		{
			Verify(Impl::RecursiveMutexImpl::IsValid(m_Handle), "Trying to unlock an invalid recursive mutex");
			Impl::RecursiveMutexImpl::Unlock(m_Handle);
		}

		NODISCARD INLINE const RecursiveMutexHandle* GetHandle()const noexcept
		{
			return &m_Handle;
		}

		NODISCARD INLINE RecursiveMutexHandle* GetHandle() noexcept
		{
			return &m_Handle;
		}
	};

	/*** Read-Write Mutex
	*	A lightweight mutex to avoid locking when we are just reading
	*
	*	To be added:
	*	In debug builds or activated by configuration, information of
	*	which thread owns the mutex, so when a deadlock occurs we can
	*	see which threads are colliding with their info
	*/
	class RWMutex
	{
		RWMutexHandle m_Handle;

	public:
		INLINE RWMutex() noexcept
		{
			Impl::RWMutexImpl::Initialize(m_Handle);
		}
		RWMutex(const RWMutex&) = delete;
		RWMutex& operator=(const RWMutex&) = delete;
		INLINE RWMutex(RWMutex&& other) noexcept
		{
			DuplicateMemory(other.m_Handle, m_Handle);
			Impl::RWMutexImpl::Invalidate(other.m_Handle);
		}
		INLINE RWMutex& operator=(RWMutex&& other) noexcept
		{
			if (this != &other)
			{
				if (Impl::RWMutexImpl::IsValid(m_Handle))
					Impl::RWMutexImpl::Deinitialize(m_Handle);
				DuplicateMemory(other.m_Handle, m_Handle);
				Impl::RWMutexImpl::Invalidate(other.m_Handle);
			}
			return *this;
		}
		INLINE ~RWMutex()
		{
			if (Impl::RWMutexImpl::IsValid(m_Handle))
				Impl::RWMutexImpl::Deinitialize(m_Handle);
		}

		INLINE void lock() noexcept
		{
			Verify(Impl::RWMutexImpl::IsValid(m_Handle), "Trying to lock an invalid read-write mutex");
			Impl::RWMutexImpl::Lock(m_Handle);
		}

		INLINE void lock_shared() noexcept
		{
			Verify(Impl::RWMutexImpl::IsValid(m_Handle), "Trying to lock shared an invalid read-write mutex");
			Impl::RWMutexImpl::LockShared(m_Handle);
		}

		INLINE void unlock() noexcept
		{
			Verify(Impl::RWMutexImpl::IsValid(m_Handle), "Trying to unlock an invalid read-write mutex");
			Impl::RWMutexImpl::Unlock(m_Handle);
		}

		INLINE void unlock_shared() noexcept
		{
			Verify(Impl::RWMutexImpl::IsValid(m_Handle), "Trying to unlock shared an invalid read-write mutex");
			Impl::RWMutexImpl::UnlockShared(m_Handle);
		}

		INLINE bool try_lock() noexcept
		{
			if (!Impl::RWMutexImpl::IsValid(m_Handle))
				return false;
			return Impl::RWMutexImpl::TryLock(m_Handle);
		}

		INLINE bool try_lock_shared() noexcept
		{
			if (!Impl::RWMutexImpl::IsValid(m_Handle))
				return false;
			return Impl::RWMutexImpl::TryLockShared(m_Handle);
		}

		NODISCARD INLINE const RWMutexHandle* GetHandle()const noexcept
		{
			return &m_Handle;
		}

		NODISCARD INLINE RWMutexHandle* GetHandle() noexcept
		{
			return &m_Handle;
		}
	};

	/*** The lightweightest Mutex
	*	It's the mutex that guaranties the lowest latency, but in exchange of huge
	*	CPU cost if its waiting on big tasks. Better be used to protect small and 
	*	fast tasks.
	*
	*	To be added:
	*	In debug builds or activated by configuration, information of
	*	which thread owns the mutex, so when a deadlock occurs we can
	*	see which threads are colliding with their info
	*/
	class SpinLock
	{
		static constexpr uint32 SpinCount = 4000;
		std::atomic_flag m_Lock;

	public:
		SpinLock() noexcept = default;
		SpinLock(const SpinLock&) = delete;
		SpinLock& operator=(const SpinLock&) = delete;
		~SpinLock() = default;

		INLINE void lock() noexcept
		{
			while (true)
			{
				for (uint32 i = 0; i < SpinCount; ++i)
				{
					if (try_lock())
						return;
				}
				THREAD_YIELD();
			}
		}

		INLINE bool try_lock() noexcept
		{
			const auto res = !m_Lock.test_and_set(std::memory_order_acquire);
			return res;
		}

		INLINE void unlock() noexcept
		{
			m_Lock.clear(std::memory_order_release);
		}
	};

	struct AdoptLock { constexpr explicit AdoptLock()noexcept = default; };
	struct DeferLock { constexpr explicit DeferLock()noexcept = default; };
	struct TryToLock { constexpr explicit TryToLock()noexcept = default; };

	/*** Locks a given mutex and unlocks it when out of scope */
	template<class Mtx>
	class Lock
	{
		Mtx& m_Mutex;

	public:
		using mutex_type = Mtx;

		INLINE explicit Lock(Mtx& mtx)
			:m_Mutex(mtx)
		{
			m_Mutex.lock();
		}

		INLINE Lock(Mtx& mtx, AdoptLock)
			:m_Mutex(mtx)
		{

		}

		INLINE ~Lock()
		{
			m_Mutex.unlock();
		}

		NODISCARD INLINE Mtx* mutex()noexcept { return &m_Mutex; }

		Lock(const Lock&) = delete;
		Lock& operator=(const Lock&) = delete;
	};

#define LOCK(MUTEX) greaper::Lock<decltype(MUTEX)> lck(MUTEX)

	/*** Locks a mutex and unlocks it when out of scope, but ensures mutex uniqueness by allowing mutex ownership */
	template<class Mtx>
	class UniqueLock
	{
	public:
		constexpr UniqueLock()noexcept = default;

		INLINE explicit UniqueLock(Mtx& mtx)noexcept
			:m_Mutex(std::addressof(mtx))
			,m_Owns(false)
		{
			m_Mutex->lock();
			m_Owns = true;
		}
		INLINE UniqueLock(Mtx& mtx, AdoptLock)noexcept
			:m_Mutex(std::addressof(mtx))
			,m_Owns(true)
		{

		}
		INLINE UniqueLock(Mtx& mtx, DeferLock)noexcept
			:m_Mutex(std::addressof(mtx))
			,m_Owns(false)
		{

		}
		INLINE UniqueLock(Mtx& mtx, TryToLock)noexcept
			:m_Mutex(std::addressof(mtx))
			,m_Owns(m_Mutex->try_lock())
		{

		}
		INLINE UniqueLock(UniqueLock&& other)noexcept
			:m_Mutex(other.m_Mutex)
			,m_Owns(other.m_Owns)
		{
			other.m_Mutex = nullptr;
			other.m_Owns = false;
		}
		INLINE UniqueLock& operator=(UniqueLock&& other)noexcept
		{
			if (this != std::addressof(other))
			{
				if (m_Owns)
					m_Mutex->unlock();

				m_Mutex = other.m_Mutex;
				m_Owns = other.m_Owns;
				other.m_Mutex = nullptr;
				other.m_Owns = false;
			}
			return *this;
		}
		INLINE ~UniqueLock()noexcept
		{
			if (m_Owns)
				m_Mutex->unlock();
		}

		UniqueLock(const UniqueLock&) = delete;
		UniqueLock& operator=(const UniqueLock&) = delete;

		INLINE void lock()noexcept
		{
			m_Mutex->lock();
			m_Owns = true;
		}

		INLINE bool try_lock()noexcept
		{
			m_Owns = m_Mutex->try_lock();
			return m_Owns;
		}

		INLINE void unlock()noexcept
		{
			if (!m_Mutex || !m_Owns)
				Break("Trying to unlock an invalid UniqueLock.");

			m_Mutex->unlock();
			m_Owns = false;
		}

		INLINE void swap(UniqueLock& other)noexcept
		{
			std::swap(m_Mutex, other.m_Mutex);
			std::swap(m_Owns, other.m_Owns);
		}

		NODISCARD INLINE Mtx* release()noexcept
		{
			auto* mtx = m_Mutex;
			m_Mutex = nullptr;
			m_Owns = false;
			return mtx;
		}

		INLINE bool owns_lock()const noexcept { return m_Owns; }

		INLINE explicit operator bool()const noexcept { return m_Owns; }

		NODISCARD INLINE Mtx* mutex()const noexcept { return m_Mutex; }

	private:
		Mtx* m_Mutex = nullptr;
		bool m_Owns = false;
	};

	/*** Locks a given read-write mutex on its shared configuration and unlocks it when out of scope */
	class SharedLock
	{
		RWMutex& m_Mutex;

	public:
		using mutex_type = RWMutex;

		INLINE explicit SharedLock(RWMutex& mutex) noexcept
			:m_Mutex(mutex)
		{
			m_Mutex.lock_shared();
		}

		INLINE SharedLock(RWMutex& mutex, AdoptLock) noexcept
			:m_Mutex(mutex)
		{

		}

		SharedLock(const SharedLock&) = delete;
		SharedLock& operator=(const SharedLock&) = delete;
		SharedLock(SharedLock&&)noexcept = default;
		SharedLock& operator=(SharedLock&&)noexcept = default;

		NODISCARD INLINE RWMutex* mutex() noexcept
		{
			return &m_Mutex;
		}

		INLINE ~SharedLock()
		{
			m_Mutex.unlock_shared();
		}
	};

#define SHAREDLOCK(MUTEX) SharedLock lck(MUTEX);

	/*** Also called ConditionVariable
	*	Handles syncronization between multiple threads by signaling when
	*	the protected resource is available.
	*	Signals work only with Greaper primitives and uses OS primitives
	* 
	*	To be added:
	*	In debug builds or activated by configuration, information of
	*	the threads that are waiting on the signal
	*/
	class Signal
	{
		SignalHandle m_Handle;

		INLINE void Wait(const UniqueLock<Mutex>& lock) noexcept
		{
			Impl::SignalImpl::Wait(m_Handle, *lock.mutex()->GetHandle());
		}
		INLINE void Wait(const UniqueLock<RWMutex>& lock) noexcept
		{
			Impl::SignalImpl::WaitRW(m_Handle, *lock.mutex()->GetHandle());
		}
		INLINE void Wait(const UniqueLock<RecursiveMutex>& lock) noexcept
		{
			Impl::SignalImpl::WaitRecursive(m_Handle, *lock.mutex()->GetHandle());
		}
		INLINE void WaitShared(const UniqueLock<RWMutex>& lock) noexcept
		{
			Impl::SignalImpl::WaitShared(m_Handle, *lock.mutex()->GetHandle());
		}
		INLINE bool WaitFor(const UniqueLock<Mutex>& lock, const uint32 millis) noexcept
		{
			return Impl::SignalImpl::WaitFor(m_Handle, *lock.mutex()->GetHandle(), millis);
		}
		INLINE bool WaitFor(const UniqueLock<RWMutex>& lock, const uint32 millis) noexcept
		{
			return Impl::SignalImpl::WaitForRW(m_Handle, *lock.mutex()->GetHandle(), millis);
		}
		INLINE bool WaitFor(const UniqueLock<RecursiveMutex>& lock, const uint32 millis) noexcept
		{
			return Impl::SignalImpl::WaitForRecursive(m_Handle, *lock.mutex()->GetHandle(), millis);
		}
		INLINE bool WaitForShared(const UniqueLock<RWMutex>& lock, const uint32 millis) noexcept
		{
			return Impl::SignalImpl::WaitForShared(m_Handle, *lock.mutex()->GetHandle(), millis);
		}

	public:
		Signal() noexcept
		{
			Impl::SignalImpl::Initialize(m_Handle);
		}
		Signal(const Signal&) = delete;
		Signal& operator=(const Signal&) = delete;
		Signal(Signal&& other) noexcept
		{
			DuplicateMemory(other.m_Handle, m_Handle);
			Impl::SignalImpl::Invalidate(other.m_Handle);
		}
		Signal& operator=(Signal&& other)noexcept
		{
			if (this != &other)
			{
				if (Impl::SignalImpl::IsValid(m_Handle))
					Impl::SignalImpl::Deinitialize(m_Handle);
				DuplicateMemory(other.m_Handle, m_Handle);
				Impl::SignalImpl::Invalidate(other.m_Handle);
			}
			return *this;
		}
		~Signal()
		{
			if (Impl::SignalImpl::IsValid(m_Handle))
				Impl::SignalImpl::Deinitialize(m_Handle);
		}

		INLINE void notify_one() noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			Impl::SignalImpl::NotifyOne(m_Handle);
		}

		INLINE void notify_all() noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			Impl::SignalImpl::NotifyAll(m_Handle);
		}

		template<class Mtx>
		INLINE void wait(const UniqueLock<Mtx>& lock) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			Wait(lock);
		}

		INLINE void wait_shared(const UniqueLock<RWMutex>& lock) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			WaitShared(lock);
		}

		template<class Mtx, class Pred>
		INLINE void wait(const UniqueLock<Mtx>& lock, Pred pred) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			while (!pred())
				Wait(lock);
		}

		template<class Pred>
		INLINE void wait_shared(const UniqueLock<RWMutex>& lock, Pred pred) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			while (!pred())
				WaitShared(lock);
		}

		template<class Mtx, class Rep, class Period>
		INLINE bool wait_for(const UniqueLock<Mtx>& lock, const std::chrono::duration<Rep, Period>& relativeTime) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			return WaitFor(lock, millis);
		}

		template<class Rep, class Period>
		INLINE bool wait_for_shared(const UniqueLock<RWMutex>& lock, const std::chrono::duration<Rep, Period>& relativeTime) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			return WaitForShared(lock, millis);
		}

		template<class Mtx, class Rep, class Period, class Pred>
		INLINE bool wait_for(const UniqueLock<Mtx>& lock, const std::chrono::duration<Rep, Period>& relativeTime, Pred pred) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			while (!pred())
			{
				if (!WaitFor(lock, millis))
					return pred();
			}
			return true;
		}

		template<class Rep, class Period, class Pred>
		INLINE bool wait_for_shared(const UniqueLock<RWMutex>& lock, const std::chrono::duration<Rep, Period>& relativeTime, Pred pred) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			while (!pred())
			{
				if (!WaitForShared(lock, millis))
					return pred();
			}
			return true;
		}

		template<class Mtx, class _Clock, class _Duration>
		INLINE bool wait_for(const UniqueLock<Mtx>& lock, const std::chrono::time_point<_Clock, _Duration>& absoluteTime) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto relativeTime = absoluteTime - _Clock::now();
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			return WaitFor(lock, millis);
		}

		template<class _Clock, class _Duration>
		INLINE bool wait_for_shared(const UniqueLock<RWMutex>& lock, const std::chrono::time_point<_Clock, _Duration>& absoluteTime) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto relativeTime = absoluteTime - _Clock::now();
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			return WaitForShared(lock, millis);
		}

		template<class Mtx, class _Clock, class _Duration, class Pred>
		INLINE bool wait_for(const UniqueLock<Mtx>& lock, const std::chrono::time_point<_Clock, _Duration>& absoluteTime, Pred pred) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto relativeTime = absoluteTime - _Clock::now();
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			while (!pred())
			{
				if (!WaitFor(lock, millis))
					return pred();
			}
			return true;
		}

		template<class _Clock, class _Duration, class Pred>
		INLINE bool wait_for(const UniqueLock<RWMutex>& lock, const std::chrono::time_point<_Clock, _Duration>& absoluteTime, Pred pred) noexcept
		{
			Verify(Impl::SignalImpl::IsValid(m_Handle), "Trying to use an invalid SignalHandle.");
			const auto relativeTime = absoluteTime - _Clock::now();
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(relativeTime).count();
			while (!pred())
			{
				if (!WaitForShared(lock, millis))
					return pred();
			}
			return true;
		}

		NODISCARD INLINE const SignalHandle* GetHandle()const noexcept
		{
			return &m_Handle;
		}

		NODISCARD INLINE SignalHandle* GetHandle() noexcept
		{
			return &m_Handle;
		}
	};

	/*** Allows syncronization for multiple resources by different threads */
	class Semaphore
	{
		Mutex m_Mutex;
		Signal m_Signal;
		sizet m_Count;
		const sizet m_MaxCount;

	public:
		INLINE explicit Semaphore(sizet maxCount = 0) noexcept
			:m_Count(maxCount)
			,m_MaxCount(maxCount)
		{

		}
		Semaphore(const Semaphore&) = delete;
		Semaphore(Semaphore&& other) = default;
		Semaphore& operator=(const Semaphore&) = delete;
		Semaphore& operator=(Semaphore&& other)noexcept = default;
		~Semaphore() = default;

		INLINE void notify()noexcept
		{
			auto lck = UniqueLock<Mutex>(m_Mutex); // can throw
			++m_Count;
			m_Signal.notify_one();
		}

		INLINE void wait()noexcept
		{
			UniqueLock<Mutex> lck(m_Mutex); // can throw
			m_Signal.wait(lck, [&]{return m_Count > 0;});
			--m_Count;
		}

		INLINE bool try_wait()noexcept
		{
			Lock<Mutex> lck(m_Mutex); // can throw
			if(m_Count > 0)
			{
				--m_Count;
				return true;
			}
			return false;
		}

		NODISCARD INLINE sizet GetMaxCount()const noexcept { return m_MaxCount; }

		NODISCARD INLINE const Signal& GetSignal()const noexcept { return m_Signal; }
		NODISCARD INLINE Signal& GetSignal() noexcept { return m_Signal; }
		NODISCARD INLINE const Mutex& GetMutex()const noexcept { return m_Mutex; }
		NODISCARD INLINE Mutex& GetMutex() noexcept { return m_Mutex; }
	};

	/*** Sets a place where the threads will wait until all of them have reached that place
	*	Was adapted from: https://stackoverflow.com/a/27118537
	*/
	class Barrier
	{
		Mutex m_Mutex;
		Signal m_Signal;
		const sizet m_MaxCount;
		sizet m_Count;
		sizet m_Generation;

	public:
		INLINE explicit Barrier(sizet maxCount = 0)noexcept
			:m_MaxCount(maxCount)
			,m_Count(maxCount)
			,m_Generation(0)
		{

		}

		INLINE void sync()noexcept
		{
			UniqueLock<Mutex> lck(m_Mutex);
			const auto gen = m_Generation;
			if(!--m_Count)
			{
				++m_Generation;
				m_Count = m_MaxCount;
				m_Signal.notify_all();
			}
			else
			{
				m_Signal.wait(lck, [this, gen] { return gen != m_Generation; });
			}
		}

		NODISCARD INLINE sizet GetMaxCount()const noexcept { return m_MaxCount; }

		NODISCARD INLINE const Signal& GetSignal()const noexcept { return m_Signal; }
		NODISCARD INLINE Signal& GetSignal()noexcept { return m_Signal; }
		NODISCARD INLINE const Mutex& GetMutex()const noexcept { return m_Mutex; }
		NODISCARD INLINE Mutex& GetMutex()noexcept { return m_Mutex; }
	};

	struct AsyncOpSyncData
	{
		Mutex Mtx;
		Signal Condition;
	};

	struct AsyncOpEmpty {  };

	/*** Allows intant return on functions that have to wait a resource to be processed
	*	this class will guarantee that you get that resource and if you want wait on it
	*	be processed.
	*/
	class IAsyncOp
	{
		struct OpData
		{
			std::any RetValue;
			std::atomic_bool IsCompleted = false;
		};

	protected:
		SPtr<OpData> m_Data;
		SPtr<AsyncOpSyncData> m_SyncData;

		void CompleteCheck()const
		{
			Verify(HasCompleted(), "Trying to get AsyncOp return value but the operation hasn't completed yet.");
		}

	public:
		IAsyncOp()
			:m_Data(Construct<OpData>())
		{

		}

		explicit IAsyncOp(UNUSED AsyncOpEmpty empty)
		{

		}

		explicit IAsyncOp(const SPtr<AsyncOpSyncData>& syncData)
			:m_Data(Construct<OpData>())
			,m_SyncData(syncData)
		{

		}

		IAsyncOp(UNUSED AsyncOpEmpty empty, const SPtr<AsyncOpSyncData>& syncData)
			:m_SyncData(syncData)
		{

		}

		bool HasCompleted()const
		{
			return m_Data->IsCompleted.load(std::memory_order_acquire);
		}

		void BlockUntilComplete()const
		{
			if(m_SyncData == nullptr)
			{
				// "No sync data is available, AsyncOp must be complete to be able to block."
				return;
			}
			UniqueLock<Mutex> lock(m_SyncData->Mtx);
			while(!HasCompleted())
				m_SyncData->Condition.wait(lock);
		}

		template<typename T>
		NODISCARD T GetReturnValue()const
		{
			return std::any_cast<T>(GetGenericReturnValue());
		}

		NODISCARD std::any GetGenericReturnValue()const
		{
			CompleteCheck();

			return m_Data->RetValue;
		}
	};

	template<class RetType>
	class TAsyncOp : public IAsyncOp
	{
		template<class RetType2>
		friend bool operator==(const TAsyncOp<RetType2>&, std::nullptr_t);
	public:
		using ReturnType = RetType;

		TAsyncOp() = default;
		
		explicit TAsyncOp(AsyncOpEmpty empty)
			:IAsyncOp(empty)
		{

		}

		explicit TAsyncOp(const SPtr<AsyncOpSyncData>& syncData)
			:IAsyncOp(syncData)
		{

		}

		TAsyncOp(AsyncOpEmpty empty, const SPtr<AsyncOpSyncData>& syncData)
			:IAsyncOp(empty, syncData)
		{

		}

		NODISCARD RetType GetReturnValue()const
		{
			this->CompleteCheck();

			return std::any_cast<RetType>(m_Data->RetValue);
		}

		void _CompleteOperation()
		{
			m_Data->IsCompleted.store(true, std::memory_order_release);

			if (m_SyncData != nullptr)
				m_SyncData->Condition.notify_all();
		}

		void _CompleteOperation(const RetType& retVal)
		{
			m_Data->RetValue = retVal;
			_CompleteOperation();
		}
	};

	template<class RetType>
	NODISCARD INLINE bool operator==(const TAsyncOp<RetType>& left, std::nullptr_t right)
	{
		return left.m_Data == nullptr;
	}

	template<class RetType>
	NODISCARD INLINE bool operator!=(const TAsyncOp<RetType>& left, std::nullptr_t right)
	{
		return !(left == nullptr);
	}

	using AsyncOp = TAsyncOp<std::any>;
}

#endif /* CORE_CONCURRENCY_H */
