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

		AsyncLogProp_t::ModificationEventHandler_t m_OnAsyncProp;

		Vector<LogData> m_QueuedMessages;
		Mutex m_QueueMutex;
		Signal m_QueueSignal;
		bool m_Threaded;
		Mutex m_WriterMutex;
		Vector<SPtr<ILogWriter>> m_Writers;
		PThread m_AsyncThread;

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

		void OnActivation(const SPtr<IInterface>& oldDefault)noexcept override;

		void OnDeactivation(const SPtr<IInterface>& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		void InitSerialization()noexcept override;

		void DeinitSerialization()noexcept override;

		WPtr<AsyncLogProp_t> GetAsyncLog()const noexcept override { return (WPtr<AsyncLogProp_t>)m_Properties[(sizet)AsyncProp]; }

		void AddLogWriter(SPtr<ILogWriter> writer)noexcept override;

		void RemoveLogWriter(sizet writerID)noexcept override;

		std::tuple<std::span<const LogData>, Mutex&> GetMessages()const noexcept override;

		void Log(LogLevel_t level, const String& message, StringView libraryName)noexcept override;

		void _Log(const LogData& data)noexcept override;
	};
}

#endif /* CORE_LOG_MANAGER_H */
