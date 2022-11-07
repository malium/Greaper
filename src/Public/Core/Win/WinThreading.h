/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_THREADING_H
#define CORE_WIN_THREADING_H 1

#include "../CorePrerequisites.h"
#include "Win32Concurrency.h"

namespace greaper
{
	using ThreadID_t = uint32;
	constexpr ThreadID_t InvalidThreadID = static_cast<ThreadID_t>(-1);
	using ThreadHandle = HANDLE;
	inline const ThreadHandle InvalidThreadHandle = static_cast<ThreadHandle>(INVALID_HANDLE_VALUE);

	using MutexHandle = SRWLOCK;
	constexpr MutexHandle* InvalidMutexHandle = nullptr;

	using RecursiveMutexHandle = CRITICAL_SECTION;
	constexpr RecursiveMutexHandle* InvalidRecursiveMutexHandle = nullptr;

	using RWMutexHandle = SRWLOCK;
	constexpr RWMutexHandle* InvalidRWMutexHandle = nullptr;

	using SignalHandle = CONDITION_VARIABLE;
	constexpr SignalHandle* InvalidSignalHandle = nullptr;

	INLINE ThreadID_t CUR_THID() noexcept
	{
		return static_cast<ThreadID_t>(::GetCurrentThreadId());
	}

	INLINE ThreadHandle CUR_THHND() noexcept
	{
		return static_cast<ThreadHandle>(::GetCurrentThread());
	}

	INLINE void THREAD_YIELD() noexcept
	{
		::SwitchToThread();
	}

	namespace Impl
	{
		struct WinMutexImpl
		{
			INLINE static bool IsValid(const MutexHandle& handle) noexcept
			{
				return handle.Ptr != reinterpret_cast<PVOID>(-1);
			}

			INLINE static void Initialize(MutexHandle& handle) noexcept
			{
				InitializeSRWLock(&handle);
			}

			INLINE static void Deinitialize(UNUSED MutexHandle& handle) noexcept
			{

			}

			INLINE static void Lock(MutexHandle& handle) noexcept
			{
				AcquireSRWLockExclusive(&handle);
			}

			INLINE static void Unlock(MutexHandle& handle) noexcept
			{
				ReleaseSRWLockExclusive(&handle);
			}

			INLINE static bool TryLock(MutexHandle& handle) noexcept
			{				
				return TryAcquireSRWLockExclusive(&handle) != FALSE;
			}

			INLINE static void Invalidate(MutexHandle& handle) noexcept
			{
				handle.Ptr = reinterpret_cast<PVOID>(-1);
			}
		};
		using MutexImpl = WinMutexImpl;

		struct WinRecursiveMutexImpl
		{
			INLINE static bool IsValid(const RecursiveMutexHandle& handle) noexcept
			{
				return handle.LockSemaphore != INVALID_HANDLE_VALUE;
			}

			INLINE static void Initialize(RecursiveMutexHandle& handle) noexcept
			{
				InitializeCriticalSection(&handle);
			}

			INLINE static void Deinitialize(RecursiveMutexHandle& handle) noexcept
			{
				DeleteCriticalSection(&handle);
			}

			INLINE static void Lock(RecursiveMutexHandle& handle) noexcept
			{
				EnterCriticalSection(&handle);
			}

			INLINE static void Unlock(RecursiveMutexHandle& handle) noexcept
			{
				LeaveCriticalSection(&handle);
			}

			INLINE static bool TryLock(RecursiveMutexHandle& handle) noexcept
			{				
				return TryEnterCriticalSection(&handle) != FALSE;
			}

			INLINE static void Invalidate(RecursiveMutexHandle& handle) noexcept
			{
				handle.LockSemaphore = INVALID_HANDLE_VALUE;
			}
		};
		using RecursiveMutexImpl = WinRecursiveMutexImpl;

		struct WinRWMutexImpl
		{
			INLINE static bool IsValid(const RWMutexHandle& handle) noexcept
			{
				return handle.Ptr != reinterpret_cast<PVOID>(-1);
			}

			INLINE static void Initialize(RWMutexHandle& handle) noexcept
			{
				InitializeSRWLock(&handle);
			}

			INLINE static void Deinitialize(UNUSED RWMutexHandle& handle) noexcept
			{

			}

			INLINE static void Lock(RWMutexHandle& handle) noexcept
			{
				AcquireSRWLockExclusive(&handle);
			}

			INLINE static void LockShared(RWMutexHandle& handle) noexcept
			{
				AcquireSRWLockShared(&handle);
			}

			INLINE static void Unlock(RWMutexHandle& handle) noexcept
			{
				ReleaseSRWLockExclusive(&handle);
			}

			INLINE static void UnlockShared(RWMutexHandle& handle) noexcept
			{
				ReleaseSRWLockShared(&handle);
			}

			INLINE static bool TryLock(RWMutexHandle& handle) noexcept
			{				
				return TryAcquireSRWLockExclusive(&handle) != FALSE;
			}

			INLINE static bool TryLockShared(RWMutexHandle& handle) noexcept
			{				
				return TryAcquireSRWLockShared(&handle) != FALSE;
			}

			INLINE static void Invalidate(RWMutexHandle& handle) noexcept
			{
				handle.Ptr = reinterpret_cast<PVOID>(-1);
			}
		};
		using RWMutexImpl = WinRWMutexImpl;

		struct WinSignalImpl
		{
			INLINE static bool IsValid(const SignalHandle& handle) noexcept
			{
				return handle.Ptr != reinterpret_cast<PVOID>(-1);
			}
			INLINE static void Initialize(SignalHandle& handle) noexcept
			{
				InitializeConditionVariable(&handle);
			}
			INLINE static void Deinitialize(UNUSED SignalHandle& handle) noexcept
			{

			}
			INLINE static void NotifyOne(SignalHandle& handle) noexcept
			{
				WakeConditionVariable(&handle);
			}
			INLINE static void NotifyAll(SignalHandle& handle) noexcept
			{
				WakeAllConditionVariable(&handle);
			}
			INLINE static void Wait(SignalHandle& handle, MutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableSRW(&handle, &mutexHandle, INFINITE, 0);
			}
			INLINE static void WaitRW(SignalHandle& handle, RWMutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableSRW(&handle, &mutexHandle, INFINITE, 0);
			}
			INLINE static void WaitRecursive(SignalHandle& handle, RecursiveMutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableCS(&handle, &mutexHandle, INFINITE);
			}
			INLINE static void WaitShared(SignalHandle& handle, RWMutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableSRW(&handle, &mutexHandle, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
			}
			INLINE static bool WaitFor(SignalHandle& handle, MutexHandle& mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableSRW(&handle, &mutexHandle, millis, 0);
			}
			INLINE static bool WaitForRW(SignalHandle& handle, RWMutexHandle mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableSRW(&handle, &mutexHandle, millis, 0);
			}
			INLINE static bool WaitForRecursive(SignalHandle& handle, RecursiveMutexHandle mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableCS(&handle, &mutexHandle, millis);
			}
			INLINE static bool WaitForShared(SignalHandle& handle, RWMutexHandle& mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableSRW(&handle, &mutexHandle, millis, CONDITION_VARIABLE_LOCKMODE_SHARED);
			}
			INLINE static void Invalidate(SignalHandle& handle) noexcept
			{
				handle.Ptr = reinterpret_cast<PVOID>(-1);
			}
		};
		using SignalImpl = WinSignalImpl;
	}
}

#endif /* CORE_WIN_THREADING_H */