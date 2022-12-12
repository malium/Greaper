/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

//#include "../FileStream.h"

namespace greaper
{
	INLINE FileStream::FileStream(std::filesystem::path  filePath, uint16 accessMode, bool freeOnClose) noexcept
		:IStream(accessMode)
		,m_Path(std::move(filePath))
		,m_FreeOnClose(freeOnClose)
	{
		std::ios::openmode openMode = std::ios::binary;

		if ((m_Access & READ) != 0)
			openMode |= std::ios::in;

		m_Stream = SPtr<std::fstream>(Construct<std::fstream>());

		if ((m_Access & WRITE) != 0)
		{
			openMode |= std::ios::out;
			openMode |= std::ios::app;
		}

		m_Stream->open(m_Path, openMode);

		if (m_Stream->fail())
		{
			return;
		}

		m_Stream->seekg(0, std::ios_base::end);
		m_Size = (ssizet)m_Stream->tellg();
		m_Stream->seekg(0, std::ios_base::beg);
	}

	INLINE FileStream::~FileStream() noexcept
	{
		Close();
	}

	INLINE ssizet FileStream::Read(void* buf, ssizet count) const noexcept
	{
		m_Stream->read((char*)buf, static_cast<std::streamsize>(count));
		return (ssizet)m_Stream->gcount();
	}

	INLINE ssizet FileStream::Write(const void* buf, ssizet count)noexcept
	{
		ssizet written = 0;
		if (IsWritable())
		{
			m_Stream->write(static_cast<const char*>(buf), static_cast<std::streamsize>(count));
			written = count;
		}
		return written;
	}

	INLINE void FileStream::Skip(ssizet count)noexcept
	{
		m_Stream->clear();
		if ((m_Access & WRITE) != 0)
			m_Stream->seekp(static_cast<std::ofstream::pos_type>(count), std::ios::cur);
		else
			m_Stream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
	}

	INLINE void FileStream::Seek(ssizet pos)noexcept
	{
		m_Stream->clear();
		if ((m_Access & WRITE) != 0)
			m_Stream->seekp(static_cast<std::ofstream::pos_type>(pos), std::ios::beg);
		else
			m_Stream->seekg(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
	}

	INLINE ssizet FileStream::Tell() const noexcept
	{
		m_Stream->clear();
		return (ssizet)m_Stream->tellg();
	}

	INLINE bool FileStream::Eof() const noexcept
	{
		return m_Stream->eof();
	}

	INLINE SPtr<IStream> FileStream::Clone(UNUSED bool copyData) const noexcept
	{
		return SPtr<IStream>(Construct<FileStream>(m_Path, GetAccessMode(), m_FreeOnClose));
	}

	INLINE void FileStream::Close()noexcept
	{
		if (m_Stream == nullptr)
			return;

		if ((m_Access & WRITE) != 0)
			m_Stream->flush();

		m_Stream->close();
		m_Stream.reset();
	}

	INLINE const std::filesystem::path& FileStream::GetPath() const noexcept { return m_Path; }
}