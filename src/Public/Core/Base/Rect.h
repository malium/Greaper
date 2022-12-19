/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_RECT_H
#define CORE_RECT_H 1

#include "../Memory.h"
//#include "../Reflection/ReflectedPlainType.h"
#include "../Reflection/PlainType.h"
#include "../StringUtils.h"

namespace greaper
{
	template<class T>
	class RectT
	{
	public:
		static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "RectT can only be instanced with an integer or a floating point type.");

		using TCategory = typename refl::TypeInfo<T>::Type;
		
		T Left = T(0);
		T Top = T(0);
		T Right = T(0);
		T Bottom = T(0);

		constexpr RectT() = default;
		constexpr RectT(T left, T top, T width, T height) noexcept;
		
		NODISCARD constexpr T GetWidth()const noexcept;
		NODISCARD constexpr T GetHeight()const noexcept;

		void SetSize(T width, T height) noexcept;
		void SetOrigin(T left, T top, bool keepSize = false) noexcept;

		constexpr bool IsInside(T x, T y)const noexcept;
		constexpr bool IsInside(const RectT& other)const noexcept;

		NODISCARD constexpr T GetArea()const noexcept;

		NODISCARD String ToString()const noexcept;
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

	template<class T> NODISCARD INLINE constexpr bool operator==(const RectT<T>& left, const RectT<T>& right) noexcept
	{
		return left.Left == right.Left
			&& left.Top == right.Top
			&& left.Right == right.Right
			&& left.Top == right.Top;
	}
	template<class T> NODISCARD INLINE constexpr bool operator!=(const RectT<T>& left, const RectT<T>& right) noexcept
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
		String left = TCategory::ToString(Left);
		String top = TCategory::ToString(Top);
		String width = TCategory::ToString(GetWidth());
		String height = TCategory::ToString(GetHeight());
		return String{'[' + left + ", " + top + "](" + width + ", " + height + ')'};
	}

	template<class T>
	INLINE bool RectT<T>::FromString(const String& str) noexcept
	{
		const auto ltBegin = str.find_first_of('[');
		const auto ltEnd = str.find_first_of(']');
		const auto whBegin = str.find_first_of('(');
		const auto whEnd = str.find_last_of(')');

		if(ltBegin == String::npos || ltEnd == String::npos
			|| whBegin == String::npos || whEnd == String::npos)
		{
			return false; // Tokens not found
		}

		StringVec ltSplit = StringUtils::Tokenize(str.substr(ltBegin+1, ltEnd - ltBegin), ',');
		for(auto& s : ltSplit) StringUtils::TrimSelf(s);
		StringVec whSplit = StringUtils::Tokenize(str.substr(whBegin + 1, whEnd - whBegin), ',');
		for(auto& s : whSplit) StringUtils::TrimSelf(s);

		if(ltSplit.size() != 2 || whSplit.size() != 2)
		{
			return false; // Wrong split size
		}
		
		if(TCategory::FromString(Left, ltSplit[0]).HasFailed()) 
			return false;
		if(TCategory::FromString(Top, ltSplit[1]).HasFailed())
			return false;

		T width = T(-1), height = T(-1);
		if(TCategory::FromString(width, whSplit[0]).HasFailed())
			return false;
		if(TCategory::FromString(height, whSplit[1]).HasFailed())
			return false;

		if(width == T(-1) || height == T(-1))
		{
			return false; // Invalid conversion or wrong size
		}

		SetSize(width, height);
	}
}

#include "../Reflection/Rect.h"

//ALLOW_MEMCPY_SERIALIZATION(greaper::RectF, data.ToString(), data.FromString(str));
//ALLOW_MEMCPY_SERIALIZATION(greaper::RectI, data.ToString(), data.FromString(str));
//ALLOW_MEMCPY_SERIALIZATION(greaper::RectU, data.ToString(), data.FromString(str));

#endif /* CORE_RECT_H */