/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_RECT_H
#define CORE_RECT_H 1

#include "../Memory.h"
#include "../Reflection/ReflectedPlainType.h"
#include "../StringUtils.h"

namespace greaper
{
	template<class T>
	class RectT
	{
	public:
		T Left = T(0);
		T Top = T(0);
		T Right = T(0);
		T Bottom = T(0);

		constexpr RectT() = default;
		constexpr RectT(T left, T top, T right, T bottom) noexcept;
		
		constexpr T GetWidth()const noexcept;
		constexpr T GetHeight()const noexcept;

		void SetSize(T widht, T height) noexcept;
		void SetOrigin(T left, T top, bool keepSize = false) noexcept;

		constexpr bool IsInside(T x, T y)const noexcept;
		constexpr bool IsInside(const RectT& other)const noexcept;

		constexpr T GetArea()const noexcept;

		String ToString()const noexcept;
		bool FromString(const String& str) noexcept;

#if PLT_WINDOWS
		INLINE void Set(const RECT& rect) noexcept
		{
			Left = (T)rect.left;
			Top = (T)rect.top;
			Right = (T)rect.right;
			Bottom = (T)rect.bottom;
		}
		INLINE constexpr RECT ToRECT()const noexcept
		{
			RECT r;
			r.left = (decltype(r.left))Left;
			r.top = (decltype(r.top))Top;
			r.right = (decltype(r.right))Right;
			r.bottom = (decltype(r.bottom))Bottom;
			return r;
		}
		INLINE constexpr bool IsInside(const POINT& p)const noexcept
		{
			return IsInside((T)p.x, (T)p.y);
		}
#endif
	};
	using RectF = RectT<float>;
	using RectI = RectT<int32>;
	using RectU = RectT<uint32>;

	template<class T> INLINE constexpr bool operator==(const RectT<T>& left, const RectT<T>& right) noexcept
	{
		return left.Left == right.Left
			&& left.Top == right.Top
			&& left.Right == right.Right
			&& left.Top == right.Top;
	}
	template<class T> INLINE constexpr bool operator!=(const RectT<T>& left, const RectT<T>& right) noexcept
	{
		return !(left == right);
	}

	template<class T>
	INLINE constexpr RectT<T>::RectT(T left, T top, T width, T height) noexcept
		:Left(left)
		,Top(top)
		,Right(left + width)
		,Bottom(top + height)
	{
		
	}

	template<class T>
	INLINE constexpr T RectT<T>::GetWidth()const noexcept
	{
		return Abs(Right - Left);
	}

	template<class T>
	INLINE constexpr T RectT<T>::GetHeight()const noexcept
	{
		return Abs(Bottom - Top);
	}

	template<class T>
	INLINE void RectT<T>::SetSize(T width, T height) noexcept
	{
		width = Abs(width);
		height = Abs(height);
		Right = Left + width;
		Bottom = Top + height;
	}

	template<class T>
	INLINE void RectT<T>::SetOrigin(T left, T top, bool keepSize) noexcept
	{
		if(!keepSize)
		{
			Left = left;
			Top = top;
		}
		else
		{
			const T width = GetWidth();
			const T height = GetHeight();
			Left = left;
			Top = top;
			SetSize(width, height);
		}
	}

	template<class T>
	INLINE constexpr bool RectT<T>::IsInside(T x, T y)const noexcept
	{
		return Left <= x && Right >= x
			&& Top <= y && Bottom >= y;
	}

	template<class T>
	INLINE constexpr bool RectT<T>::IsInside(const RectT<T>& other)const noexcept
	{
		return Left <= other.Left && Right >= other.Right
			&& Top <= other.Top && Bottom >= other.Bottom;
	}

	template<class T>
	INLINE constexpr T RectT<T>::GetArea()const noexcept
	{
		return GetWidth() * GetHeight();
	}
	
	template<class T>
	INLINE String RectT<T>::ToString()const noexcept
	{
		using Conv = ReflectedPlainType<T>;
		const auto left = Conv::ToString(Left);
		const auto top = Conv::ToString(Top);
		const auto width = Conv::ToString(GetWidth());
		const auto height = Conv::ToString(GetHeight());
		return String{"[" + left + ", " + top + "](" + width + ", " + height + ")"};
	}

	template<class T>
	INLINE bool RectT<T>::FromString(const String& str) noexcept
	{
		using Conv = ReflectedPlainType<T>;

		const auto ltBegin = str.find_first_of('[');
		const auto ltEnd = str.find_first_of(']');
		const auto whBegin = str.find_first_of('(');
		const auto whEnd = str.find_last_of(')');

		if(ltBegin == String::npos || ltEnd == String::npos
			|| whBegin == String::npos || whEnd == String::npos)
		{
			return false; // Tokens not found
		}

		const auto ltSplit = StringUtils::Tokenize(str.substr(ltBegin+1, ltEnd - ltBegin), ',');
		const auto whSplit = StringUtils::Tokenize(str.substr(whBegin + 1, whEnd - whBegin), ',');

		if(ltSplit.size() != 2 || whSplit.size() != 2)
		{
			return false; // Wrong split size
		}

		Conv::FromString(Left, ltSplit[0]);
		Conv::FromString(Top, ltSplit[1]);

		T width = T(-1), height = T(-1);
		Conv::FromString(width, whSplit[0]);
		Conv::FromString(height, whSplit[1]);

		if(width < 0 || height < 0)
		{
			return false; // Invalid conversion or wrong size
		}

		SetSize(width, height);
	}
}

namespace greaper
{
	template<class T>
	struct ReflectedPlainType<RectT<T>>
	{
		enum { ID = RTI_Rect }; enum { HasDynamicSize = 0 };

		static ReflectedSize_t ToStream(const RectT<T>& data, IStream& stream)
		{
			stream.Write(&data, sizeof(data));
		}

		static ReflectedSize_t FromStream(RectT<T>& data, IStream& stream)
		{
			stream.Read(&data, sizeof(data));
		}

		static String ToString(const RectT<T>& data)
		{
			return data.ToString();
		}

		static void FromString(RectT<T>& data, const String& str)
		{
			data.FromString(str);
		}

		static ReflectedSize_t GetSize(const RectT<T>& data)
		{
			return sizeof(data);
		}
	};
	template<class T> struct ReflectedTypeToID<RectT<T>> { static constexpr ReflectedTypeID_t ID = RTI_Rect; };
}
#endif /* CORE_RECT_H */