/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_STREAM_H
#define CORE_STREAM_H 1

#include "CorePrerequisites.h"

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
		explicit IStream(uint16 accessMode = READ)noexcept;
		explicit IStream(StringView name, uint16 accessMode = READ)noexcept;

		virtual ~IStream()noexcept = default;

		const String& GetName()const noexcept { return m_Name; }

		uint16 GetAccessMode()const noexcept { return m_Access; }
		
		virtual bool IsReadable()const noexcept { return (m_Access & READ) != 0; }

		virtual bool IsWritable()const noexcept { return (m_Access & WRITE) != 0; }

		virtual bool IsFile()const noexcept = 0;

		template<typename T>
		const IStream& operator>>(T& val)const noexcept;

		template<typename T>
		IStream& operator>>(T& val)noexcept;

		template<typename T>
		IStream& operator<<(const T& val)noexcept;

		virtual ssizet Read(void* buff, ssizet count)const noexcept = 0;
		
		virtual ssizet Write(const void* buff, ssizet count)noexcept = 0;
		
		virtual void Skip(ssizet count)noexcept = 0;

		virtual void Seek(ssizet pos)noexcept = 0;

		virtual ssizet Tell()const noexcept = 0;

		virtual void Align(uint32 count = 1)noexcept;

		virtual bool Eof()const noexcept = 0;

		ssizet Size()const noexcept { return m_Size; }

		virtual SPtr<IStream> Clone(bool copyData = true)const noexcept = 0;

		virtual void Close()noexcept = 0;
	};
}

#include "Base/Stream.inl"

#endif /* CORE_STREAM_H */