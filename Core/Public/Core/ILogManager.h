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
			LL_ERROR,
			CRITICAL
		};
	}
	using LogLevel_t = ELogLevel::Type;

	struct LogData
	{
		String Message;
		Timepoint_t Time;
		LogLevel_t Level;
	};

	struct LogManagerConfig
	{
		bool CreateDefaultLogger = true;
		bool AsyncronousLogging = true;
	};

	class ILogManager : public TInterface<ILogManager, LogManagerConfig>
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0xB05DBD1D, 0x83FE42E1, 0x90CFF1EE, 0x2434CD0D };
		static constexpr StringView InterfaceName = StringView{ "LogManager" };

		using LogEvent_t = Event<const LogData&>;
		using LogEventHandler_t = LogEvent_t::HandlerType;
		using LogEventFunction_t = LogEvent_t::HandlerFunction;

		virtual ~ILogManager()noexcept = default;

		virtual LogEvent_t*const GetLogEvent() = 0;

		virtual void Log(LogLevel_t level, const String& message) = 0;

		virtual void _Log(const LogData& data) = 0;
	};
}

#endif /* CORE_I_LOG_MANAGER_H */
