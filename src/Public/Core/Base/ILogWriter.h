/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_LOG_WRITER_H
#define CORE_I_LOG_WRITER_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
	struct LogData;
	class ILogManager;
	class ILogWriter
	{
		WLogManager m_LogManager;
		sizet m_WriterID;

	public:
		virtual ~ILogWriter()noexcept = default;

		virtual void WriteLog(const LogData& logData)noexcept = 0;

		virtual bool WritePreviousMessages()const noexcept { return false; }

		INLINE void _Connect(WLogManager logManager, sizet writerID)noexcept
		{
			m_LogManager = std::move(logManager);
			m_WriterID = writerID;
		}
	
		INLINE void Disconnect()noexcept
		{
			if (m_LogManager.expired())
				return;

			auto logMgr = m_LogManager.lock();
			logMgr->RemoveLogWriter(m_WriterID);
		}

		INLINE NODISCARD sizet GetWriterID()const noexcept { return m_WriterID; }
	};
}

#endif /* CORE_I_LOG_WRITER_H */