/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_RECT_H
#define CORE_RECT_H 1

#include "../CorePrerequisites.h"
//#include "../Reflection/ReflectedPlainType.h"
#include "../Reflection/PlainType.h"
#include "../StringUtils.h"
#include <Math/Vector2.h>
#if PLT_WINDOWS
#include "../Win/Win32Base.h"
#endif
#include "IntersectionResult.h"

namespace greaper
{
	template<class T>
	class RectT
	{
	public:
		static_assert(std::is_arithmetic_v<T>, "RectT can only be instanced with an integer or a floating point type.");

		using value_type = T;
		
		using point_type = typename std::conditional_t<std::is_floating_point_v<T>, math::Vector2Real<T>, 
							std::conditional_t<std::is_signed_v<T>, math::Vector2Signed<T>, math::Vector2Unsigned<T>>>;

		using TCategory = typename refl::TypeInfo<T>::Type;
		
		T Left = T(0);
		T Top = T(0);
		T Right = T(0);
		T Bottom = T(0);

		constexpr RectT() = default;
		constexpr RectT(T left, T top, T width, T height) noexcept;
		constexpr RectT(const point_type& origin, const point_type& size)noexcept;
		
		constexpr T GetWidth()const noexcept;
		constexpr T GetHeight()const noexcept;
		constexpr point_type GetOrigin()const noexcept;
		constexpr point_type GetSize()const noexcept;
		constexpr point_type GetLT()const noexcept;
		constexpr point_type GetRT()const noexcept;
		constexpr point_type GetLB()const noexcept;
		constexpr point_type GetRB()const noexcept;

		void Set(T left, T top, T width, T hegiht) noexcept;
		void Set(const point_type& position, const point_type& size)noexcept;
		void SetSize(T width, T height) noexcept;
		void SetSize(const point_type& size)noexcept;
		void SetOrigin(T left, T top, bool keepSize = false) noexcept;
		void SetOrigin(const point_type& pos, bool keepSize = false)noexcept;

		constexpr IntersectionResult_t IsInside(T x, T y)const noexcept;
		constexpr IntersectionResult_t IsInside(const point_type& point)const noexcept;
		constexpr IntersectionResult_t IsInside(const RectT& other)const noexcept;

		constexpr T GetArea()const noexcept;
		constexpr bool IsEmpty()const noexcept;

		constexpr bool IsEqual(const RectT& other)const noexcept;

		String ToString()const noexcept;
		bool FromString(const String& str) noexcept;
#if PLT_WINDOWS
		INLINE constexpr explicit RectT(const RECT& rect)noexcept
			:Left((T)rect.left)
			,Top((T)rect.top)
			,Right((T)rect.right)
			,Bottom((T)rect.bottom)
		{
			
		}
		NODISCARD INLINE constexpr operator RECT()const noexcept { return ToRECT(); }
		INLINE void Set(const RECT& rect) noexcept
		{
			Left = (T)rect.left;
			Top = (T)rect.top;
			Right = (T)rect.right;
			Bottom = (T)rect.bottom;
		}
		NODISCARD INLINE constexpr RECT ToRECT()const noexcept
		{
			return RECT{
					(LONG)Left,
					(LONG)Top,
					(LONG)Right,
					(LONG)Bottom
				};
		}
		NODISCARD INLINE constexpr bool IsInside(const POINT& p)const noexcept
		{
			return IsInside((T)p.x, (T)p.y);
		}
#endif
	};

	template<class T> NODISCARD INLINE constexpr bool operator==(const RectT<T>& left, const RectT<T>& right) noexcept
	{
		return left.IsEqual(right);
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
		,Bottom(top - height)
	{
		
	}

	template<class T>
	INLINE constexpr RectT<T>::RectT(const point_type& origin, const point_type& size) noexcept
		:Left(origin.X)
		,Top(origin.Y)
		,Right(origin.X + size.X)
		,Bottom(origin.Y - size.Y)
	{

	}

	template<class T>
	NODISCARD INLINE constexpr T RectT<T>::GetWidth()const noexcept
	{
		return Abs(Right - Left);
	}

	template<class T>
	NODISCARD INLINE constexpr T RectT<T>::GetHeight()const noexcept
	{
		return Abs(Bottom - Top);
	}

	template<class T>
	NODISCARD INLINE constexpr RectT<T>::point_type RectT<T>::GetOrigin() const noexcept
	{
		return point_type(Left, Top);
	}

	template<class T>
	NODISCARD INLINE constexpr RectT<T>::point_type RectT<T>::GetSize() const noexcept
	{
		return point_type(GetWidth(), GetHeight());
	}

	template<class T>
	NODISCARD INLINE constexpr RectT<T>::point_type RectT<T>::GetLT() const noexcept
	{
		return point_type(Left, Top);
	}

	template<class T>
	NODISCARD INLINE constexpr RectT<T>::point_type RectT<T>::GetRT() const noexcept
	{
		return point_type(Right, Top);
	}

	template<class T>
	NODISCARD INLINE constexpr RectT<T>::point_type RectT<T>::GetLB() const noexcept
	{
		return point_type(Left, Bottom);
	}

	template<class T>
	NODISCARD INLINE constexpr RectT<T>::point_type RectT<T>::GetRB() const noexcept
	{
		return point_type(Right, Bottom);
	}

	template<class T>
	INLINE void RectT<T>::Set(T left, T top, T width, T hegiht) noexcept
	{
		Left = left;
		Top = top;
		Right = left + width;
		Bottom = top - hegiht;
	}

	template<class T>
	INLINE void RectT<T>::Set(const point_type& position, const point_type& size) noexcept
	{
		Set(position.X, position.Y, size.X, size.Y);
	}

	template<class T>
	INLINE void RectT<T>::SetSize(T width, T height) noexcept
	{
		width = Abs(width);
		height = Abs(height);
		Right = Left + width;
		Bottom = Top - height;
	}

	template<class T>
	INLINE void RectT<T>::SetSize(const point_type& size) noexcept
	{
		SetSize(size.X, size.Y);
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
	INLINE void RectT<T>::SetOrigin(const point_type& pos, bool keepSize) noexcept
	{
		SetOrigin(pos.X, pos.Y, keepSize);
	}

	template<class T>
	NODISCARD INLINE constexpr IntersectionResult_t RectT<T>::IsInside(T x, T y)const noexcept
	{
		if(IsEmpty())
			return IntersectionResult_t::OUTSIDE;

		if(Left < x && Right > x && Top > y && Bottom < y)
			return IntersectionResult_t::FULLY_INSIDE;
		
		if(((Left == x || Right == x) && Top >= y && Bottom <= y) ||
			((Top == y || Bottom == y) && Left <= x && Right >= x))
			return IntersectionResult_t::ON_THE_EDGE;
		
		return IntersectionResult_t::OUTSIDE;
	}

	template<class T>
	NODISCARD INLINE constexpr IntersectionResult_t RectT<T>::IsInside(const point_type& point) const noexcept
	{
		return IsInside(point.X, point.Y);
	}

	template<class T>
	NODISCARD INLINE constexpr IntersectionResult_t RectT<T>::IsInside(const RectT<T>& other)const noexcept
	{
		if  (IsEmpty() || other.IsEmpty())
			return IntersectionResult_t::OUTSIDE;

		IntersectionResult_t ltInside = IsInside(other.GetLT());
		IntersectionResult_t lbInside = IsInside(other.GetLB());
		IntersectionResult_t rtInside = IsInside(other.GetRT());
		IntersectionResult_t rbInside = IsInside(other.GetRB());

		if (ltInside == IntersectionResult_t::FULLY_INSIDE &&
			lbInside == IntersectionResult_t::FULLY_INSIDE &&
			rtInside == IntersectionResult_t::FULLY_INSIDE &&
			rbInside == IntersectionResult_t::FULLY_INSIDE)
			return IntersectionResult_t::FULLY_INSIDE;
		
		if (ltInside == IntersectionResult_t::FULLY_INSIDE ||
			lbInside == IntersectionResult_t::FULLY_INSIDE ||
			rtInside == IntersectionResult_t::FULLY_INSIDE ||
			rbInside == IntersectionResult_t::FULLY_INSIDE)
			return IntersectionResult_t::PARTIALLY_INSIDE;
		
		if (ltInside == IntersectionResult_t::ON_THE_EDGE ||
			lbInside == IntersectionResult_t::ON_THE_EDGE ||
			rtInside == IntersectionResult_t::ON_THE_EDGE ||
			rbInside == IntersectionResult_t::ON_THE_EDGE)
			return IntersectionResult_t::ON_THE_EDGE;

		return IntersectionResult_t::OUTSIDE;
	}

	template<class T>
	NODISCARD INLINE constexpr T RectT<T>::GetArea()const noexcept
	{
		return GetWidth() * GetHeight();
	}

	template<class T>
	NODISCARD INLINE constexpr bool RectT<T>::IsEmpty() const noexcept
	{
		return GetArea() == T(0);
	}

	template<class T>
	NODISCARD INLINE constexpr bool RectT<T>::IsEqual(const RectT& other)const noexcept
	{
		return Left == other.Left
			&& Top == other.Top
			&& Right == other.Right
			&& Top == other.Top;
	}
	
	template<class T>
	NODISCARD INLINE String RectT<T>::ToString()const noexcept
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

namespace std
{
	template<class T>
	struct hash<greaper::RectT<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::RectT<T>& r)const noexcept
		{
			return ComputeHash(r.Left, r.Top, r.Right, r.Bottom);
		}
	};
}

#endif /* CORE_RECT_H */