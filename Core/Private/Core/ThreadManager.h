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
		IGreaperLibrary* m_Library;
		bool m_IsActive;
		bool m_IsInitialized;
		InitializationEvt_t m_OnInitialization;
		ActivationEvt_t m_OnActivation;
		ChangingDefaultEvt_t m_OnChangingDefault;
		ThreadEvent_t m_ThreadEvent;
		ThreadPoolEvent_t m_ThreadPoolEvent;

	public:
		const Uuid& GetInterfaceUUID()const override { return InterfaceUUID; }

		const StringView& GetInterfaceName()const override { return InterfaceName; }

		IGreaperLibrary* GetLibrary()const override { return m_Library; }

		void Initialize(IGreaperLibrary* library)override;

		void Deinitialize()override;

		void OnActivate()override;

		void OnDeactivate()override;

		void InitProperties()override;

		void DeinitProperties()override;

		bool IsActive()const override { return m_IsActive; }

		bool IsInitialized()const override { return m_IsInitialized; }

		InitializationEvt_t* const GetInitializationEvent()override { return &m_OnInitialization; }

		ActivationEvt_t* const GetActivationEvent()override { return &m_OnActivation; }

		void OnChangingDefault(IInterface* newDefault)override;

		ChangingDefaultEvt_t* const GetChangingDefaultEvent() { return &m_OnChangingDefault; }

		ThreadManager();

		~ThreadManager()noexcept;

		Result<IThread*> GetThread(ThreadID_t id)const override;

		Result<IThread*> GetThread(const String& threadName)const override;

		Result<IThread*> CreateThread(const ThreadConfig& config)override;

		void DestroyThread(IThread* thread)override;

		ThreadEvent_t* const GetThreadEvent()override { return &m_ThreadEvent; }

		Result<IThreadPool*> GetThreadPool(const String& poolName)const override;

		Result<IThreadPool*> CreateThreadPool(const ThreadPoolConfig& config)override;

		void DestroyThreadPool(IThreadPool* pool)override;

		ThreadPoolEvent_t* const GetThreadPoolEvent()override { return &m_ThreadPoolEvent; }
	};
}

#endif /* CORE_THREAD_MANAGER_H */
