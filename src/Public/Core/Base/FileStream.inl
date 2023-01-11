#include "FileStream.h"
/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
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
			m_Stream.reset();
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
		ssizet read = 0;
		if(IsReadable())
		{
			m_Stream->read((char*)buf, static_cast<std::streamsize>(count));
			read = (ssizet)m_Stream->gcount();
		}
		return read;
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
		if (IsWritable())
		{
			m_Stream->clear();
			m_Stream->seekp(static_cast<std::ofstream::pos_type>(count), std::ios::cur);
		}
		else if(IsReadable())
		{
			m_Stream->clear();
			m_Stream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
		}
	}

	INLINE void FileStream::Seek(ssizet pos)noexcept
	{
		if(IsWritable())
		{
			m_Stream->clear();
			m_Stream->seekp(static_cast<std::ofstream::pos_type>(pos), std::ios::beg);
		}
		else if(IsReadable())
		{
			m_Stream->clear();
			m_Stream->seekg(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
		}
	}

	INLINE ssizet FileStream::Tell() const noexcept
	{
		if(m_Stream != nullptr)
		{
			m_Stream->clear();
			return (ssizet)m_Stream->tellg();
		}
		return -1;
	}

	INLINE bool FileStream::Eof() const noexcept
	{
		if(m_Stream != nullptr)
			return m_Stream->eof();
		return true;
	}

	INLINE bool FileStream::IsReadable() const noexcept
	{
		if(m_Stream != nullptr)
			return IStream::IsReadable();
		return false;
	}

	INLINE bool FileStream::IsWritable() const noexcept
	{
		if(m_Stream != nullptr)
			return IStream::IsWritable();
		return false;
	}

	NODISCARD INLINE SPtr<IStream> FileStream::Clone(UNUSED bool copyData) const noexcept
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