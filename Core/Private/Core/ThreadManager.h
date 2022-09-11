/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
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

		mutable RWMutex m_ThreadMutex;
		Vector<PThread> m_Threads;
		UnorderedMap<String, sizet> m_ThreadNameMap;
		UnorderedMap<ThreadID_t, sizet> m_ThreadIDMap;

		std::size_t GetThreadCount()const noexcept
		{
			SharedLock lock(m_ThreadMutex);
			return m_Threads.size();
		}
		const PThread& GetThreadIdx(std::size_t idx)const noexcept
		{
			SharedLock lock(m_ThreadMutex);
			return m_Threads[idx];
		}

	public:
		ThreadManager();

		~ThreadManager()noexcept;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(SPtr<IInterface> oldDefault)noexcept override;

		void OnDeactivation(SPtr<IInterface> newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		void InitSerialization()noexcept override;

		void DeinitSerialization()noexcept override;

		Result<WThread> GetThread(ThreadID_t id)const noexcept override;

		Result<WThread> GetThread(const String& threadName)const noexcept override;

		Result<PThread> CreateThread(const ThreadConfig& config)noexcept override;

		void DestroyThread(PThread thread)noexcept override;

		ThreadCreationEvent_t* GetThreadCreationEvent()const noexcept override { return &m_ThreadCreationEvent; }
		
		ThreadDestructionEvent_t* GetThreadDestructionEvent()const noexcept override { return &m_ThreadDestructionEvent; }

		CRange<PThread> GetThreads()const noexcept override { return CRange<PThread>(); }
	};
}

#endif /* CORE_THREAD_MANAGER_H */
