/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_THREAD_MANAGER_H
#define CORE_THREAD_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Core/Concurrency.h>
#include <Core/IThreadManager.h>

namespace greaper::core
{
	class ThreadManager final : public IThreadManager
	{
		mutable ThreadCreationEvent_t m_ThreadCreationEvent;
		mutable ThreadDestructionEvent_t m_ThreadDestructionEvent;

		ThreadDestructionEvent_t::HandlerType m_DestructionEventHnd;

		mutable RecursiveMutex m_ThreadMutex;
		Vector<PThread> m_Threads;
		UnorderedMap<String, sizet> m_ThreadNameMap;
		UnorderedMap<ThreadID_t, sizet> m_ThreadIDMap;

		void OnThreadDestruction(const PThread& thread)noexcept;

	public:
		ThreadManager();

		~ThreadManager()noexcept;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(const SPtr<IInterface>& oldDefault)noexcept override;

		void OnDeactivation(const SPtr<IInterface>& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		void InitSerialization()noexcept override;

		void DeinitSerialization()noexcept override;

		TResult<WThread> GetThread(ThreadID_t id)const noexcept override;

		TResult<WThread> GetThread(const String& threadName)const noexcept override;

		TResult<PThread> CreateThread(const ThreadConfig& config)noexcept override;

		INLINE ThreadCreationEvent_t* GetThreadCreationEvent()const noexcept override { return &m_ThreadCreationEvent; }
		
		INLINE ThreadDestructionEvent_t* GetThreadDestructionEvent()const noexcept override { return &m_ThreadDestructionEvent; }

		INLINE std::tuple<CSpan<PThread>, RecursiveMutex&> GetThreads()const noexcept override
		{
			return { CreateSpan(m_Threads), (RecursiveMutex&)m_ThreadMutex };
		}
	};
}

#endif /* CORE_THREAD_MANAGER_H */
