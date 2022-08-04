/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LOG_MANAGER_H
#define CORE_LOG_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Core/ILogManager.h>
#include <Core/Property.h>
#include <Core/Concurrency.h>
#include <Core/IThreadManager.h>

namespace greaper::core
{
	class LogManager final : public ILogManager
	{
		IGreaperLibrary* m_Library;
		bool m_IsActive;
		bool m_IsInitialized;
		InitializationEvt_t m_OnInitialization;
		ActivationEvt_t m_OnActivation;
		ChangingDefaultEvt_t m_OnChangingDefault;
		LogEvent_t m_OnLogMessage;
		AsyncLogProp_t::ModificationEventHandler_t m_OnAsyncProp;
		AsyncLogProp_t* m_AsyncProp;

		Vector<LogData> m_QueuedMessages;
		std::atomic_bool m_Threaded;
		Mutex m_Mutex;
		Signal m_MessageSignal;
		IThread* m_AsyncThread;

		void OnAsyncChanged(IProperty* prop);
		void RunFn();

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

		LogManager();

		~LogManager()noexcept;

		LogEvent_t* const GetLogEvent()override { return &m_OnLogMessage; }

		AsyncLogProp_t* GetAsyncLog()override { return m_AsyncProp; }

		void Log(LogLevel_t level, const String& message)override;

		void _Log(const LogData& data)override;
	};
}

#endif /* CORE_LOG_MANAGER_H */
