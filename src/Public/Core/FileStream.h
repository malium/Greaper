/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_FILE_STREAM_H
#define CORE_FILE_STREAM_H 1

#include "Stream.h"
#include <filesystem>
#include <fstream>
#include <utility>

namespace greaper
{
	class FileStream : public IStream
	{
	public:
		explicit FileStream(std::filesystem::path  filePath, uint16 accessMode = READ, bool freeOnClose = true)noexcept;
		~FileStream()noexcept override;

		INLINE bool IsFile()const noexcept override { return true; }

		ssizet Read(void* buf, ssizet count)const noexcept override;

		ssizet Write(const void* buf, ssizet count)noexcept override;

		void Skip(ssizet count)noexcept override;

		void Seek(ssizet pos)noexcept override;

		ssizet Tell()const noexcept override;

		bool Eof()const noexcept override;

		NODISCARD SPtr<IStream> Clone(bool copyData = true)const noexcept override;

		void Close()noexcept override;

		const std::filesystem::path& GetPath()const noexcept;

	protected:
		std::filesystem::path m_Path;
		SPtr<std::fstream> m_Stream;
		bool m_FreeOnClose;
	};
}

#include "Base/FileStream.inl"

#endif /* CORE_FILE_STREAM_H */