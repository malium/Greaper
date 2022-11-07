/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LOG_WRITER_FILE_H
#define CORE_LOG_WRITER_FILE_H 1

#include "../ILogManager.h"
#include "../FileStream.h"

namespace greaper
{
	class LogWriterFile : ILogWriter
	{
		SPtr<FileStream> m_Stream;

		static constexpr const achar* gLevelName[] =
		{
			"VERB",
			"INFO",
			"WARN",
			"ERRO",
			"CRIT"
		};

	public:
		INLINE explicit LogWriterFile(const std::filesystem::path& directory = std::filesystem::current_path() / "Logs", uint32 maxLogs = 20)noexcept
		{
			// Create directory if not exists
			std::filesystem::create_directories(directory);

			Vector<std::filesystem::directory_entry> entries;

			for (const auto& entry : std::filesystem::directory_iterator(directory))
			{
				if (!entry.is_regular_file())
					continue;
				entries.push_back(entry);
			}
			
			if (entries.size() >= maxLogs)
			{
				std::sort(entries.begin(), entries.end(), [](const std::filesystem::directory_entry& left, const std::filesystem::directory_entry& right) -> bool
					{
						return left.last_write_time() > right.last_write_time();
					});

				while (entries.size() >= maxLogs)
				{
					const auto& entry = entries.back();
					std::filesystem::remove(entry.path());
					entries.pop_back();
				}
			}

			auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			String name;
			name.resize(48, '\0');
			const auto ret = std::strftime(name.data(), name.size(), "%Y.%m.%d_%H.%M.%S", std::localtime(&now));
			if (ret > 0)
				name.resize(ret);
			name.append(".log");

			m_Stream = SPtr<FileStream>(Construct<FileStream>(directory / name, (uint16)(FileStream::AccessMode::WRITE | FileStream::AccessMode::READ)));
		}
		INLINE bool WritePreviousMessages()const noexcept override { return true; }

		INLINE void WriteLog(const LogData& logData)noexcept override
		{
			static achar gTimeBuff[64];

			auto time = std::chrono::system_clock::to_time_t(logData.Time);
			std::strftime(gTimeBuff, ArraySize(gTimeBuff), "%H:%M:%S", std::localtime(&time));

			auto message = Format("[%s][%s][%s]: %s\r\n", logData.LibraryName.data(), gLevelName[(sizet)logData.Level], gTimeBuff, logData.Message.c_str());
			m_Stream->Write(message.c_str(), message.length() - 1);
		}
	};
}

#endif /* CORE_LOG_WRITER_FILE_H */