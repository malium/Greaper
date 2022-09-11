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

		mutable LogEvent_t m_OnLogMessage;
		AsyncLogProp_t::ModificationEventHandler_t m_OnAsyncProp;

		Vector<LogData> m_QueuedMessages;
		std::atomic_bool m_Threaded;

		void OnAsyncChanged(IProperty* prop);
		void RunFn();

	public:
		LogManager();

		~LogManager()noexcept;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(SPtr<IInterface> oldDefault)noexcept override;

		void OnDeactivation(SPtr<IInterface> newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		void InitSerialization()noexcept override;

		void DeinitSerialization()noexcept override;

		LogEvent_t* GetLogEvent()const noexcept override { return &m_OnLogMessage; }

		WPtr<AsyncLogProp_t> GetAsyncLog()const noexcept override { return m_Properties[(sizet)AsyncProp]; }

		void Log(LogLevel_t level, const String& message)noexcept override;

		void _Log(const LogData& data)noexcept override;
	};
}

#endif /* CORE_LOG_MANAGER_H */
