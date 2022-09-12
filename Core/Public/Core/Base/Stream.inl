/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

//#include "../Stream.h"

namespace greaper
{
	template<typename T>
	INLINE const IStream& IStream::operator>>(T& val)const noexcept
	{
		const auto read = Read((void*)&val, sizeof(val));
		VerifyEqual(read, sizeof(val), "Couldn't read the whole value from the stream, Size:%d Read:%d.", sizeof(val), read);
		return *this;
	}

	template<typename T>
	INLINE IStream& IStream::operator>>(T& val)noexcept
	{
		const auto read = Read((void*)&val, sizeof(val));
		VerifyEqual(read, sizeof(val), "Couldn't read the whole value from the stream, Size:%d Read:%d.", sizeof(val), read);
		return *this;
	}

	template<typename T>
	INLINE IStream& IStream::operator<<(const T& val)noexcept
	{
		const auto written = Write(&val, sizeof(val));
		VerifyEqual(written, sizeof(val), "Couldn't write the whole value to the stream, Size:%d Written:%d.", sizeof(val), written);
		return *this;
	}

	INLINE IStream::IStream(const uint16 accessMode)noexcept
		:m_Access(accessMode)
		,m_Size(0)
	{

	}

	INLINE IStream::IStream(StringView name, const uint16 accessMode)noexcept
		:m_Name(name)
		,m_Access(accessMode)
		,m_Size(0)
	{

	}

	INLINE void IStream::Align(uint32 count)noexcept
	{
		if (count <= 1)
			return;

		const auto alignOffset = (count - (Tell() & (count - 1))) & (count - 1);
		Skip(alignOffset);
	}
}