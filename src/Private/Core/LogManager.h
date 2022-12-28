/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LOG_MANAGER_H
#define CORE_LOG_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Core/Reflection/PlainType.h>
#include <Core/Reflection/Property.h>
#include <Core/ILogManager.h>
#include <Core/Property.h>
#include <Core/Concurrency.h>
#include <Core/IThreadManager.h>
#include <Core/MPMCTaskScheduler.h>

#define LOGMANAGER_USE_MPMC 1

namespace greaper::core
{
	class LogManager final : public ILogManager
	{
		enum PropertiesIndices
		{
			AsyncProp,

			COUNT
		};

		AsyncLogProp_t::ModificationEventHandler_t m_OnAsyncProp;

#if !LOGMANAGER_USE_MPMC
		Vector<LogData> m_QueuedMessages;
		Mutex m_QueueMutex;
		Signal m_QueueSignal;
#endif
		bool m_Threaded;
		Mutex m_WriterMutex;
		Vector<SPtr<ILogWriter>> m_Writers;
		PThread m_AsyncThread;
#if LOGMANAGER_USE_MPMC
		SPtr<MPMCTaskScheduler> m_Scheduler;
#endif

		Vector<LogData> m_Messages;
		mutable Mutex m_MessagesMutex;

		void OnAsyncChanged(IProperty* prop);
		void StartThreadMode();
		void StopThreadMode();
		void RunFn();
		void LogToWriters(const LogData& data);

	public:
		LogManager();

		~LogManager()noexcept;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(const PInterface& oldDefault)noexcept override;

		void OnDeactivation(UNUSED const PInterface& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		WPtr<AsyncLogProp_t> GetAsyncLog()const noexcept override { return (WPtr<AsyncLogProp_t>)m_Properties[(sizet)AsyncProp]; }

		void AddLogWriter(SPtr<ILogWriter> writer)noexcept override;

		void RemoveLogWriter(sizet writerID)noexcept override;

		INLINE void AccessMessages(const std::function<void(CSpan<LogData>)>& accessFn)const noexcept override
		{
			auto lck = Lock(m_MessagesMutex);
			accessFn(CreateSpan(m_Messages));
		}

		void Log(LogLevel_t level, const String& message, StringView libraryName)noexcept override;

		void _Log(const LogData& data)noexcept override;
	};
}

#endif /* CORE_LOG_MANAGER_H */
