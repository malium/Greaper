/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

//#include "../Uuid.h"

namespace greaper
{
	namespace Impl
	{
		constexpr achar HEX_TO_LITERAL[16] =
		{
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
		};
		constexpr uint8 LITERAL_TO_HEX[256] =
		{
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			// 0 through 9 translate to 0  though 9
			0x00,  0x01,  0x02,  0x03,  0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			// A through F translate to 10 though 15
			0xFF,  0x0A,  0x0B,  0x0C,  0x0D,  0x0E,  0x0F,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			// a through f translate to 10 though 15
			0xFF,  0x0A,  0x0B,  0x0C,  0x0D,  0x0E,  0x0F,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
			0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF
		};

		INLINE constexpr void ViewToUUID(const StringView& view, uint32 data[4]) noexcept
		{
			if (view.size() < 36)
				return;

			uint32 idx = 0;

			for (int32 i = 7; i >= 0; --i)
			{
				const auto charVal = view[idx++];
				const auto hexVal = (uint32)LITERAL_TO_HEX[static_cast<size_t>(charVal)];
				data[0] |= hexVal << (i * 4);
			}

			++idx;

			for (int32 i = 7; i >= 4; --i)
			{
				const auto charVal = view[idx++];
				const auto hexVal = (uint32)LITERAL_TO_HEX[static_cast<size_t>(charVal)];
				data[1] |= hexVal << (i * 4);
			}

			++idx;

			for (int32 i = 3; i >= 0; --i)
			{
				const auto charVal = view[idx++];
				const auto hexVal = (uint32)LITERAL_TO_HEX[static_cast<size_t>(charVal)];
				data[1] |= hexVal << (i * 4);
			}

			++idx;

			for (int32 i = 7; i >= 4; --i)
			{
				const auto charVal = view[idx++];
				const auto hexVal = (uint32)LITERAL_TO_HEX[static_cast<size_t>(charVal)];
				data[2] |= hexVal << (i * 4);
			}

			++idx;

			for (int32 i = 3; i >= 0; --i)
			{
				const auto charVal = view[idx++];
				const auto hexVal = (uint32)LITERAL_TO_HEX[static_cast<size_t>(charVal)];
				data[2] |= hexVal << (i * 4);
			}


			for (int32 i = 7; i >= 0; --i)
			{
				const auto charVal = view[idx++];
				const auto hexVal = (uint32)LITERAL_TO_HEX[static_cast<size_t>(charVal)];
				data[3] |= hexVal << (i * 4);
			}
		}

		INLINE void UUIDToString(const uint32 data[4], String& uuid) noexcept
		{
			uint8 output[36] = {};
			uint32 idx = 0;

			for (int32 i = 7; i >= 0; --i)
			{
				const auto hexVal = (data[0] >> (i * 4)) & 0xF;
				output[idx++] = HEX_TO_LITERAL[hexVal];
			}

			output[idx++] = '-';

			for (int32 i = 7; i >= 4; --i)
			{
				const auto hexVal = (data[1] >> (i * 4)) & 0xF;
				output[idx++] = HEX_TO_LITERAL[hexVal];
			}

			output[idx++] = '-';

			for (int32 i = 3; i >= 0; --i)
			{
				const auto hexVal = (data[1] >> (i * 4)) & 0xF;
				output[idx++] = HEX_TO_LITERAL[hexVal];
			}

			output[idx++] = '-';

			for (int32 i = 7; i >= 4; --i)
			{
				const auto hexVal = (data[2] >> (i * 4)) & 0xF;
				output[idx++] = HEX_TO_LITERAL[hexVal];
			}

			output[idx++] = '-';

			for (int32 i = 3; i >= 0; --i)
			{
				const auto hexVal = (data[2] >> (i * 4)) & 0xF;
				output[idx++] = HEX_TO_LITERAL[hexVal];
			}

			for (int32 i = 7; i >= 0; --i)
			{
				const auto hexVal = (data[3] >> (i * 4)) & 0xF;
				output[idx++] = HEX_TO_LITERAL[hexVal];
			}
			uuid.assign(reinterpret_cast<const achar*>(output), 36);
		}
	}

	/*INLINE constexpr Uuid::Uuid() noexcept
		:m_Data{ 0, 0, 0, 0 }
	{

	}*/

	INLINE constexpr Uuid::Uuid(const uint32 data0, const uint32 data1, const uint32 data2, const uint32 data3) noexcept
		:m_Data{ data0, data1, data2, data3 }
	{

	}

	INLINE constexpr Uuid::Uuid(const StringView& view) noexcept
		:m_Data{ 0, 0, 0, 0 }
	{
		Impl::ViewToUUID(view, m_Data);
	}

	/*INLINE constexpr Uuid::Uuid(const Uuid& other)noexcept
		:m_Data{ other.m_Data[0], other.m_Data[1], other.m_Data[2], other.m_Data[3] }
	{

	}
	INLINE constexpr Uuid::Uuid(Uuid&& other)noexcept
		:m_Data{ other.m_Data[0], other.m_Data[1], other.m_Data[2], other.m_Data[3] }
	{
		for (int i = 0; i < 4; ++i)
			other.m_Data[i] = 0;
	}
	INLINE Uuid& Uuid::operator=(const Uuid& other)noexcept
	{
		for (int i = 0; i < 4; ++i)
			m_Data[i] = other.m_Data[i];
		return *this;
	}
	INLINE Uuid& Uuid::operator=(Uuid&& other)noexcept
	{
		if (this == &other)
			return *this;
		for (int i = 0; i < 4; ++i)
		{
			m_Data[i] = other.m_Data[i];
			other.m_Data[i] = 0;
		}
	}*/

	INLINE Uuid& Uuid::operator=(const StringView& view) noexcept
	{
		Impl::ViewToUUID(view, m_Data);
		return *this;
	}
	INLINE Uuid& Uuid::operator=(const String& str) noexcept
	{
		Impl::ViewToUUID(str, m_Data);
		return *this;
	}

	[[nodiscard]] INLINE String Uuid::ToString()const noexcept
	{
		String uuid;
		Impl::UUIDToString(m_Data, uuid);
		return uuid;
	}

	INLINE void Uuid::FromString(const String& str) noexcept
	{
		Impl::ViewToUUID(StringView{str.data(), str.size()}, m_Data);
	}

	INLINE Uuid Uuid::GenerateRandom() noexcept
	{
#if PLT_WINDOWS
		UUID uuid;
		UuidCreate(&uuid);
		const auto data0 = uuid.Data1;
		const auto data1 = uuid.Data2 | (uuid.Data3 << 16);
		const auto data2 = uuid.Data3 | (uuid.Data4[0] << 16) | (uuid.Data4[1] << 24);
		const auto data3 = uuid.Data4[2] | (uuid.Data4[3] << 8) | (uuid.Data4[4] << 16) | (uuid.Data4[5] << 24);
		return Uuid(data0, data1, data2, data3);
#elif PLT_LINUX
		uuid_t nativeUUID;
		uuid_generate(nativeUUID);

		return Uuid(
			*(uint32*)&nativeUUID[0],
			*(uint32*)&nativeUUID[4],
			*(uint32*)&nativeUUID[8],
			*(uint32*)&nativeUUID[12]);
#endif
	}

	INLINE constexpr bool Uuid::IsEmpty()const noexcept
	{
		return m_Data[0] == 0 && m_Data[1] == 0 && m_Data[2] == 0 && m_Data[3] == 0;
	}

	INLINE constexpr const uint32* Uuid::GetData() const noexcept
	{
		return &(m_Data[0]);
	}

	INLINE constexpr Uuid Uuid::Empty()noexcept
	{
		return Uuid{};
	}

	INLINE constexpr bool operator!=(const Uuid& left, const Uuid& right)noexcept
	{
		return !(left == right);
	}

	INLINE constexpr bool operator>(const Uuid& left, const Uuid& right)noexcept
	{
		return right < left;
	}

	INLINE constexpr bool operator<=(const Uuid& left, const Uuid& right)noexcept
	{
		return !(left > right);
	}

	INLINE constexpr bool operator>=(const Uuid& left, const Uuid& right)noexcept
	{
		return !(left < right);
	}
}

INLINE constexpr bool greaper::operator==(const greaper::Uuid& left, const greaper::Uuid& right)noexcept
{
	return left.m_Data[0] == right.m_Data[0] && left.m_Data[1] == right.m_Data[1]
		&& left.m_Data[2] == right.m_Data[2] && left.m_Data[3] == right.m_Data[3];
}

INLINE constexpr bool greaper::operator<(const greaper::Uuid& left, const greaper::Uuid& right)noexcept
{
	if (left.m_Data[0] < right.m_Data[0])
		return true;
	else if (left.m_Data[0] > right.m_Data[0])
		return false;

	if (left.m_Data[1] < right.m_Data[1])
		return true;
	else if (left.m_Data[1] > right.m_Data[1])
		return false;

	if (left.m_Data[2] < right.m_Data[2])
		return true;
	else if (left.m_Data[2] > right.m_Data[2])
		return false;

	if (left.m_Data[3] < right.m_Data[3])
		return true;
	else if (left.m_Data[3] > right.m_Data[3])
		return false;

	return false; // Equal
}

namespace std
{
	template<>
	struct hash<greaper::Uuid>
	{
		size_t operator()(const greaper::Uuid& val)const noexcept
		{
			return ComputeHash(val.m_Data[0], val.m_Data[1], val.m_Data[2], val.m_Data[3]);
		}
	};
}