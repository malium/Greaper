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
#include <Core/Base/TaskScheduler.h>

namespace greaper::core
{
	class LogManager final : public ILogManager
	{
		enum PropertiesIndices
		{
			AsyncProp,

			COUNT
		};

		WGreaperLib m_Library;
		bool m_IsActive;
		bool m_IsInitialized;
		mutable InitializationEvt_t m_OnInitialization;
		mutable ActivationEvt_t m_OnActivation;
		mutable ChangingDefaultEvt_t m_OnChangingDefault;
		mutable LogEvent_t m_OnLogMessage;
		AsyncLogProp_t::ModificationEventHandler_t m_OnAsyncProp;
		Vector<WIProperty> m_Properties;

		Vector<LogData> m_QueuedMessages;
		std::atomic_bool m_Threaded;
		//TaskScheduler m_Scheduler;

		void OnAsyncChanged(IProperty* prop);
		void RunFn();

	public:
		const Uuid& GetInterfaceUUID()const noexcept override { return InterfaceUUID; }

		const StringView& GetInterfaceName()const noexcept override { return InterfaceName; }

		WGreaperLib GetLibrary()const noexcept override { return m_Library; }

		void Initialize(WPtr<IGreaperLibrary> library)noexcept override;

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

		LogManager();

		~LogManager()noexcept;

		LogEvent_t* GetLogEvent()const noexcept override { return &m_OnLogMessage; }

		WPtr<AsyncLogProp_t> GetAsyncLog()const noexcept override { return m_Properties[(sizet)AsyncProp]; }

		CRange<WPtr<IProperty>> GetProperties()const noexcept override { return CreateRange(m_Properties); }

		void Log(LogLevel_t level, const String& message)noexcept override;

		void _Log(const LogData& data)noexcept override;
	};
}

#endif /* CORE_LOG_MANAGER_H */
