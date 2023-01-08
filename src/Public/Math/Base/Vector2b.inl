/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR2B_H
#define MATH_VECTOR2B_H 1

#include "../MathPrerequisites.h"
#include <Core/StringUtils.h>
#include <array>

namespace greaper::math
{
	class Vector2b
	{
	public:
		static constexpr sizet ComponentCount = 2;
		using value_type = bool;

		bool X{ false };
		bool Y{ false };

		constexpr Vector2b()noexcept = default;
		INLINE constexpr Vector2b(bool x, bool y)noexcept :X(x), Y(y) {  }
		INLINE constexpr explicit Vector2b(const std::array<bool, ComponentCount>& arr) : X(arr[0]), Y(arr[1]) {  }

		NODISCARD INLINE constexpr bool& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr const bool& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector2, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr std::array<bool, ComponentCount> ToArray()const noexcept
		{
			return { X, Y };
		}
		INLINE void Set(const Vector2b& other)noexcept
		{
			X = other.X;
			Y = other.Y;
		}
		INLINE void Set(bool x, bool y)noexcept
		{
			X = x;
			Y = y;
		}
		INLINE void SetZero()noexcept
		{
			X = false;
			Y = false;
		}
		NODISCARD INLINE constexpr bool IsEqual(const Vector2b& other)const noexcept
		{
			return X == other.X && Y == other.Y;
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ false, false });
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y;
		}
		NODISCARD INLINE String ToString()const noexcept
		{
			static constexpr StringView trueValueTxt = "true"sv;
			static constexpr StringView falseValueTxt = "false"sv;
			static constexpr StringView splitTxt = ", "sv;
			static constexpr auto maxValueSize = Max(trueValueTxt.length(), falseValueTxt.length());
			static constexpr auto maxStringSize = ComponentCount * maxValueSize + (ComponentCount - 1) * splitTxt.length();

			String rtn;
			rtn.reserve(maxStringSize);
			auto x = X ? trueValueTxt : falseValueTxt;
			auto y = Y ? trueValueTxt : falseValueTxt;
			return rtn.append(x).append(splitTxt).append(y);
		}
		INLINE bool FromString(StringView str)noexcept
		{
			auto split = StringUtils::Tokenize(str, ',');
			if (split.size() != ComponentCount)
				return false;

			for (auto& r : split)
				StringUtils::ToLowerSelf(StringUtils::TrimSelf(r));

			X = split[0] == "true"sv ? true : false;
			Y = split[1] == "true"sv ? true : false;

			return true;
		}

		static const Vector2b ZERO;
		static const Vector2b UNIT;
	};

	inline const Vector2b Vector2b::ZERO = Vector2b{};
	inline const Vector2b Vector2b::UNIT = Vector2b(true, true);

	NODISCARD INLINE constexpr bool operator==(const Vector2b& left, const Vector2b& right)noexcept { return left.IsEqual(right); }
	NODISCARD INLINE constexpr bool operator!=(const Vector2b& left, const Vector2b& right)noexcept { return !(left == right); }
}

namespace std
{
	template<>
	struct hash<greaper::math::Vector2b>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector2b& v)const noexcept
		{
			return ComputeHash(v.X, v.Y);
		}
	};
}

#endif /* MATH_VECTOR2B_H */