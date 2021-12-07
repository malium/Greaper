/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_STREAM_H
#define CORE_STREAM_H 1

#include "Memory.h"

namespace greaper
{
	class IStream
	{
	public:
		enum AccessMode
		{
			READ = 1,
			WRITE
		};

	protected:
		static constexpr uint32 StreamTempSize = 128;
		String m_Name;
		ssizet m_Size;
		uint16 m_Access;

	public:
		IStream(uint16 accessMode = READ);
		IStream(StringView name, uint16 accessMode = READ);

		virtual ~IStream() = default;

		const String& GetName()const noexcept { return m_Name; }

		uint16 GetAccessMode()const noexcept { return m_Access; }
		
		virtual bool IsReadable()const noexcept { return (m_Access & READ) != 0; }

		virtual bool IsWritable()const noexcept { return (m_Access & WRITE) != 0; }

		virtual bool IsFile()const noexcept = 0;

		template<typename T>
		IStream& operator>>(T& val)const;

		template<typename T>
		IStream& operator<<(const T& val);

		virtual ssizet Read(void* buff, ssizet count)const = 0;
		
		virtual ssizet Write(const void* buff, ssizet count) = 0;
		
		virtual void Skip(ssizet count) = 0;

		virtual void Seek(ssizet pos) = 0;

		virtual ssizet Tell()const = 0;

		virtual void Align(uint32 count = 1);

		virtual bool Eof()const = 0;

		ssizet Size()const noexcept { return m_Size; }

		virtual SPtr<IStream> Clone(bool copyData = true)const = 0;

		virtual void Close() = 0;
	};
}

#include "Base/Stream.inl"

#endif /* CORE_STREAM_H */