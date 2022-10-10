/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_MEMORY_STREAM_H
#define CORE_MEMORY_STREAM_H 1

#include "Stream.h"

namespace greaper
{
	class MemoryStream : public IStream
	{
	protected:
		uint8* m_Data;
		mutable uint8* m_Cursor;
		uint8* m_End;
		bool m_OwnsMemory;

		void Realloc(sizet bytes)noexcept;

	public:
		MemoryStream()noexcept;

		explicit MemoryStream(sizet capacity)noexcept;

		MemoryStream(void* memory, sizet size)noexcept;

		MemoryStream(const MemoryStream& other)noexcept;
		MemoryStream& operator=(const MemoryStream& other)noexcept;
		MemoryStream(MemoryStream&& other) noexcept;
		MemoryStream& operator=(MemoryStream&& other) noexcept;

		~MemoryStream()noexcept override;

		INLINE bool IsFile()const noexcept override { return false; }

		INLINE uint8* GetData()const noexcept { return m_Data; }

		INLINE uint8* GetCursor()const noexcept { return m_Cursor; }

		ssizet Read(void* buf, ssizet count)const noexcept override;

		ssizet Write(const void* buf, ssizet count)noexcept override;

		void Skip(ssizet count)noexcept override;

		void Seek(ssizet pos)noexcept override;

		INLINE ssizet Tell()const noexcept override { return m_Cursor - m_Data; }

		INLINE bool Eof()const noexcept override { return m_Cursor >= m_End; }

		SPtr<IStream> Clone(bool copyData = true)const noexcept override;

		void Close()noexcept override;

		uint8* DisownMemory()noexcept;
	};
}

#include "Base/MemoryStream.inl"

#endif /* CORE_MEMORY_STREAM_H */