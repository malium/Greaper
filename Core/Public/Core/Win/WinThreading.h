/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_THREADING_H
#define CORE_WIN_THREADING_H 1

#include "../CorePrerequisites.h"

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

	ThreadID_t CUR_THID() noexcept
	{
		return static_cast<ThreadID_t>(::GetCurrentThreadId());
	}

	ThreadHandle CUR_THHND() noexcept
	{
		return static_cast<ThreadHandle>(::GetCurrentThread());
	}

	void THREAD_YIELD() noexcept
	{
		::SwitchToThread();
	}

	namespace Impl
	{
		struct WinMutexImpl
		{
			static bool IsValid(const MutexHandle& handle) noexcept
			{
				return handle.Ptr != reinterpret_cast<PVOID>(-1);
			}

			static void Initialize(MutexHandle& handle) noexcept
			{
				InitializeSRWLock(&handle);
			}

			static void Deinitialize(MutexHandle& handle) noexcept
			{
				UNUSED(handle);
			}

			static void Lock(MutexHandle& handle) noexcept
			{
				AcquireSRWLockExclusive(&handle);
			}

			static void Unlock(MutexHandle& handle) noexcept
			{
				ReleaseSRWLockExclusive(&handle);
			}

			static bool TryLock(MutexHandle& handle) noexcept
			{				
				return TryAcquireSRWLockExclusive(&handle) != FALSE;
			}

			static void Invalidate(MutexHandle& handle) noexcept
			{
				handle.Ptr = reinterpret_cast<PVOID>(-1);
			}
		};
		using MutexImpl = WinMutexImpl;

		struct WinRecursiveMutexImpl
		{
			static bool IsValid(const RecursiveMutexHandle& handle) noexcept
			{
				return handle.LockSemaphore != INVALID_HANDLE_VALUE;
			}

			static void Initialize(RecursiveMutexHandle& handle) noexcept
			{
				InitializeCriticalSection(&handle);
			}

			static void Deinitialize(RecursiveMutexHandle& handle) noexcept
			{
				DeleteCriticalSection(&handle);
			}

			static void Lock(RecursiveMutexHandle& handle) noexcept
			{
				EnterCriticalSection(&handle);
			}

			static void Unlock(RecursiveMutexHandle& handle) noexcept
			{
				LeaveCriticalSection(&handle);
			}

			static bool TryLock(RecursiveMutexHandle& handle) noexcept
			{				
				return TryEnterCriticalSection(&handle) != FALSE;
			}

			static void Invalidate(RecursiveMutexHandle& handle) noexcept
			{
				handle.LockSemaphore = INVALID_HANDLE_VALUE;
			}
		};
		using RecursiveMutexImpl = WinRecursiveMutexImpl;

		struct WinRWMutexImpl
		{
			static bool IsValid(const RWMutexHandle& handle) noexcept
			{
				return handle.Ptr != reinterpret_cast<PVOID>(-1);
			}

			static void Initialize(RWMutexHandle& handle) noexcept
			{
				InitializeSRWLock(&handle);
			}

			static void Deinitialize(RWMutexHandle& handle) noexcept
			{
				UNUSED(handle);
			}

			static void Lock(RWMutexHandle& handle) noexcept
			{
				AcquireSRWLockExclusive(&handle);
			}

			static void LockShared(RWMutexHandle& handle) noexcept
			{
				AcquireSRWLockShared(&handle);
			}

			static void Unlock(RWMutexHandle& handle) noexcept
			{
				ReleaseSRWLockExclusive(&handle);
			}

			static void UnlockShared(RWMutexHandle& handle) noexcept
			{
				ReleaseSRWLockShared(&handle);
			}

			static bool TryLock(RWMutexHandle& handle) noexcept
			{				
				return TryAcquireSRWLockExclusive(&handle) != FALSE;
			}

			static bool TryLockShared(RWMutexHandle& handle) noexcept
			{				
				return TryAcquireSRWLockShared(&handle) != FALSE;
			}

			static void Invalidate(RWMutexHandle& handle) noexcept
			{
				handle.Ptr = reinterpret_cast<PVOID>(-1);
			}
		};
		using RWMutexImpl = WinRWMutexImpl;

		struct WinSignalImpl
		{
			static bool IsValid(const SignalHandle& handle) noexcept
			{
				return handle.Ptr != reinterpret_cast<PVOID>(-1);
			}
			static void Initialize(SignalHandle& handle) noexcept
			{
				InitializeConditionVariable(&handle);
			}
			static void Deinitialize(SignalHandle& handle) noexcept
			{
				UNUSED(handle);
			}
			static void NotifyOne(SignalHandle& handle) noexcept
			{
				WakeConditionVariable(&handle);
			}
			static void NotifyAll(SignalHandle& handle) noexcept
			{
				WakeAllConditionVariable(&handle);
			}
			static void Wait(SignalHandle& handle, MutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableSRW(&handle, &mutexHandle, INFINITE, 0);
			}
			static void WaitRW(SignalHandle& handle, RWMutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableSRW(&handle, &mutexHandle, INFINITE, 0);
			}
			static void WaitRecursive(SignalHandle& handle, RecursiveMutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableCS(&handle, &mutexHandle, INFINITE);
			}
			static void WaitShared(SignalHandle& handle, RWMutexHandle& mutexHandle) noexcept
			{
				SleepConditionVariableSRW(&handle, &mutexHandle, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
			}
			static bool WaitFor(SignalHandle& handle, MutexHandle& mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableSRW(&handle, &mutexHandle, millis, 0);
			}
			static bool WaitForRW(SignalHandle& handle, RWMutexHandle mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableSRW(&handle, &mutexHandle, millis, 0);
			}
			static bool WaitForRecursive(SignalHandle& handle, RecursiveMutexHandle mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableCS(&handle, &mutexHandle, millis);
			}
			static bool WaitForShared(SignalHandle& handle, RWMutexHandle& mutexHandle, uint32 millis) noexcept
			{
				return SleepConditionVariableSRW(&handle, &mutexHandle, millis, CONDITION_VARIABLE_LOCKMODE_SHARED);
			}
			static void Invalidate(SignalHandle& handle) noexcept
			{
				handle.Ptr = reinterpret_cast<PVOID>(-1);
			}
		};
		using SignalImpl = WinSignalImpl;
	}
}

#endif /* CORE_WIN_THREADING_H */