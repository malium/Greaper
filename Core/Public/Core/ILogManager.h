/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_LOG_MANAGER_H
#define CORE_I_LOG_MANAGER_H 1

#include "Interface.h"
#include "Event.h"

namespace greaper
{
	namespace ELogLevel
	{
		enum Type
		{
			VERBOSE,
			INFORMATIVE,
			WARNING,
			ERROR,
			CRITICAL
		};
	}
	using LogLevel_t = ELogLevel::Type;

	struct LogData
	{
		String Message;
		Timepoint_t Time;
		LogLevel_t Level;
		StringView LibraryName;
	};

	class ILogWriter;

	class ILogManager : public TInterface<ILogManager>
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0xB05DBD1D, 0x83FE42E1, 0x90CFF1EE, 0x2434CD0D };
		static constexpr StringView InterfaceName = StringView{ "LogManager" };

		DEF_PROP(AsyncLog, bool);

		virtual ~ILogManager()noexcept = default;

		virtual WPtr<AsyncLogProp_t> GetAsyncLog()const noexcept = 0;

		virtual void AddLogWriter(SPtr<ILogWriter> writer)noexcept = 0;

		virtual void RemoveLogWriter(sizet writerID)noexcept = 0;

		virtual CRangeProtected<LogData, Mutex> GetMessages()const noexcept = 0;

		virtual void Log(LogLevel_t level, const String& message, StringView libraryName)noexcept = 0;

		virtual void _Log(const LogData& data)noexcept = 0;
	};

	using WLogManager = WPtr<ILogManager>;
	using PLogManager = SPtr<ILogManager>;
}

#include "Base/ILogWriter.h"

#endif /* CORE_I_LOG_MANAGER_H */
