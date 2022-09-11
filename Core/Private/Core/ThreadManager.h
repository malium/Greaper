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
		WGreaperLib m_Library;
		bool m_IsActive;
		bool m_IsInitialized;
		mutable InitializationEvt_t m_OnInitialization;
		mutable ActivationEvt_t m_OnActivation;
		mutable ChangingDefaultEvt_t m_OnChangingDefault;
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
		const Uuid& GetInterfaceUUID()const noexcept override { return InterfaceUUID; }

		const StringView& GetInterfaceName()const noexcept override { return InterfaceName; }

		WGreaperLib GetLibrary()const noexcept override { return m_Library; }

		void Initialize(WGreaperLib library)noexcept override;

		void Deinitialize()noexcept override;

		void OnActivate()noexcept override;

		void OnDeactivate()noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		bool IsActive()const noexcept override { return m_IsActive; }

		bool IsInitialized()const noexcept override { return m_IsInitialized; }

		InitializationEvt_t* GetInitializationEvent()const noexcept override { return &m_OnInitialization; }

		ActivationEvt_t* GetActivationEvent()const noexcept override { return &m_OnActivation; }

		void OnChangingDefault(WInterface newDefault)noexcept override;

		ChangingDefaultEvt_t* GetChangingDefaultEvent()const noexcept { return &m_OnChangingDefault; }

		ThreadManager();

		~ThreadManager()noexcept;

		Result<WThread> GetThread(ThreadID_t id)const noexcept override;

		Result<WThread> GetThread(const String& threadName)const noexcept override;

		Result<PThread> CreateThread(const ThreadConfig& config)noexcept override;

		void DestroyThread(PThread thread)noexcept override;

		ThreadCreationEvent_t* GetThreadCreationEvent()const noexcept override { return &m_ThreadCreationEvent; }
		
		ThreadDestructionEvent_t* GetThreadDestructionEvent()const noexcept override { return &m_ThreadDestructionEvent; }

		CRange<WPtr<IProperty>> GetProperties()const noexcept override { return CRange<WPtr<IProperty>>(); }

		CRange<PThread> GetThreads()const noexcept override { return CRange<PThread>(); }

		/*Result<IThreadPool*> GetThreadPool(const String& poolName)const override;

		Result<IThreadPool*> CreateThreadPool(const ThreadPoolConfig& config)override;

		void DestroyThreadPool(IThreadPool* pool)override;

		ThreadPoolEvent_t* const GetThreadPoolEvent()override { return &m_ThreadPoolEvent; }*/
	};
}

#endif /* CORE_THREAD_MANAGER_H */
