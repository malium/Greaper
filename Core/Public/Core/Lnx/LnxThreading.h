/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_THREADING_H
#define CORE_LNX_THREADING_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
    using ThreadID_t = pthread_t;
	using ThreadHandle = ThreadID_t;
    
    using MutexHandle = pthread_mutex_t;

    using RecursiveMutexHandle = pthread_mutex_t;

    using RWMutexHandle = pthread_rwlock_t;

    using SignalHandle = pthread_cond_t;

    INLINE ThreadID_t CUR_THID() noexcept
	{
		return static_cast<ThreadID_t>(::pthread_self());
	}

	INLINE ThreadHandle CUR_THHND() noexcept
	{
		return CUR_THID();
	}

    INLINE void THREAD_YIELD() noexcept
	{
		::pthread_yield();
	}

    namespace Impl
    {
        struct LnxMutexImpl
        {
            static bool IsValid(const MutexHandle& handle) noexcept
            {
                return !IsMemoryCleared(handle);
            }

            static void Initialize(MutexHandle& handle) noexcept
			{
                pthread_mutex_init(&handle, nullptr);
			}

            static void Deinitialize(MutexHandle& handle) noexcept
			{
                pthread_mutex_destroy(&handle);
			}

            static void Lock(MutexHandle& handle) noexcept
			{
                pthread_mutex_lock(&handle);
			}

			static void Unlock(MutexHandle& handle) noexcept
			{
                pthread_mutex_unlock(&handle);
			}

			static bool TryLock(MutexHandle& handle) noexcept
			{
                return pthread_mutex_trylock(&handle) != 0;
			}

			static void Invalidate(MutexHandle& handle) noexcept
			{
                ClearMemory(handle);
			}
        };
        using MutexImpl = LnxMutexImpl;

        struct LnxRecursiveMutexImpl
		{
			static bool IsValid(const RecursiveMutexHandle& handle) noexcept
			{
				return !IsMemoryCleared(handle);
			}

			static void Initialize(RecursiveMutexHandle& handle) noexcept
			{
                pthread_mutexattr_t attributes;
                pthread_mutexattr_init(&attributes);
                pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(&handle, &attributes);
                pthread_mutexattr_destroy(&attributes);
			}

			static void Deinitialize(RecursiveMutexHandle& handle) noexcept
			{
                pthread_mutex_destroy(&handle);
			}

			static void Lock(RecursiveMutexHandle& handle) noexcept
			{
				pthread_mutex_lock(&handle);
			}

			static void Unlock(RecursiveMutexHandle& handle) noexcept
			{
				pthread_mutex_unlock(&handle);
			}

			static bool TryLock(RecursiveMutexHandle& handle) noexcept
			{				
				return pthread_mutex_trylock(&handle) != 0;
			}

			static void Invalidate(RecursiveMutexHandle& handle) noexcept
			{
                ClearMemory(handle);
			}
		};
		using RecursiveMutexImpl = LnxRecursiveMutexImpl;

        struct LnxRWMutexImpl
		{
			static bool IsValid(const RWMutexHandle& handle) noexcept
			{
				return !IsMemoryCleared(handle);
			}

			static void Initialize(RWMutexHandle& handle) noexcept
			{
                pthread_rwlock_init(&handle, nullptr);
			}

			static void Deinitialize(RWMutexHandle& handle) noexcept
			{
                pthread_rwlock_destroy(&handle);
			}

			static void Lock(RWMutexHandle& handle) noexcept
			{
                pthread_rwlock_wrlock(&handle);
			}

			static void LockShared(RWMutexHandle& handle) noexcept
			{
                pthread_rwlock_rdlock(&handle);
			}

			static void Unlock(RWMutexHandle& handle) noexcept
			{
                pthread_rwlock_unlock(&handle);
			}

			static void UnlockShared(RWMutexHandle& handle) noexcept
			{
                pthread_rwlock_unlock(&handle);
			}

			static bool TryLock(RWMutexHandle& handle) noexcept
			{				
				return pthread_rwlock_trywrlock(&handle) != 0;
			}

			static bool TryLockShared(RWMutexHandle& handle) noexcept
			{				
				return pthread_rwlock_tryrdlock(&handle) != 0;
			}

			static void Invalidate(RWMutexHandle& handle) noexcept
			{
				ClearMemory(handle);
			}
		};
		using RWMutexImpl = LnxRWMutexImpl;

        struct LnxSignalImpl
		{
			static bool IsValid(const SignalHandle& handle) noexcept
			{
                return !IsMemoryCleared(handle);
			}
			static void Initialize(SignalHandle& handle) noexcept
			{
                pthread_cond_init(&handle, nullptr);
			}
			static void Deinitialize(SignalHandle& handle) noexcept
			{
                pthread_cond_destroy(&handle);
			}
			static void NotifyOne(SignalHandle& handle) noexcept
			{
                pthread_cond_signal(&handle);
			}
			static void NotifyAll(SignalHandle& handle) noexcept
			{
                pthread_cond_broadcast(&handle);
			}
			static void Wait(SignalHandle& handle, MutexHandle& mutexHandle) noexcept
			{
                pthread_cond_wait(&handle, &mutexHandle);
			}
			static void WaitRW(SignalHandle& handle, RWMutexHandle& mutexHandle) noexcept
			{
                Break("ConditionVariables and RWMutex don't currently work together under Linux.");
			}
			static void WaitRecursive(SignalHandle& handle, RecursiveMutexHandle& mutexHandle) noexcept
			{
                pthread_cond_wait(&handle, &mutexHandle);
			}
			static void WaitShared(SignalHandle& handle, RWMutexHandle& mutexHandle) noexcept
			{
                Break("ConditionVariables and RWMutex don't currently work together under Linux.");
			}
			static bool WaitFor(SignalHandle& handle, MutexHandle& mutexHandle, uint32 millis) noexcept
			{
                timespec t;
                const auto timePoint = Clock_t::now();
                const auto nanos = (std::chrono::nanoseconds)std::chrono::milliseconds(millis);
                const auto abs = (timePoint.time_since_epoch() + nanos).count();
                t.tv_nsec = abs;
                const auto rc = pthread_cond_timedwait(&handle, &mutexHandle, &t);
				return rc == 0;
			}
			static bool WaitForRW(SignalHandle& handle, RWMutexHandle mutexHandle, uint32 millis) noexcept
			{
                Break("ConditionVariables and RWMutex don't currently work together under Linux.");
				return true;
			}
			static bool WaitForRecursive(SignalHandle& handle, RecursiveMutexHandle mutexHandle, uint32 millis) noexcept
			{
                timespec t;
                const auto timePoint = Clock_t::now();
                const auto nanos = (std::chrono::nanoseconds)std::chrono::milliseconds(millis);
                const auto abs = (timePoint.time_since_epoch() + nanos).count();
                t.tv_nsec = abs;
                const auto rc = pthread_cond_timedwait(&handle, &mutexHandle, &t);
				return rc == 0;
			}
			static bool WaitForShared(SignalHandle& handle, RWMutexHandle& mutexHandle, uint32 millis) noexcept
			{
                Break("ConditionVariables and RWMutex don't currently work together under Linux.");
				return true;
			}
			static void Invalidate(SignalHandle& handle) noexcept
			{
                ClearMemory(handle);
			}
		};
		using SignalImpl = LnxSignalImpl;
    }
}

#endif /* CORE_LNX_THREADING_H */