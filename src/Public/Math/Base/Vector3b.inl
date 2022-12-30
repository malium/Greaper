/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR3B_H
#define MATH_VECTOR3B_H 1

#include "Vector2b.inl"
#include "VecRef.h"

namespace greaper::math
{
	class Vector3b
	{
	public:
		static constexpr sizet ComponentCount = 3;
		using value_type = bool;

		bool X = false;
		bool Y = false;
		bool Z = false;

		constexpr Vector3b()noexcept = default;
		INLINE constexpr Vector3b(bool x, bool y, bool z)noexcept :X(x), Y(y), Z(z) {  }
		INLINE constexpr explicit Vector3b(const std::array<bool, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {  }
		INLINE constexpr explicit Vector3b(const Vector2b& v2, bool z)noexcept :X(v2.X), Y(v2.Y), Z(z) {  }

		NODISCARD INLINE constexpr bool& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr const bool& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector3, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC3();
		NODISCARD INLINE constexpr std::array<bool, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z };
		}
		INLINE void Set(const Vector3b& other)noexcept
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		INLINE void Set(bool x, bool y, bool z)noexcept
		{
			X = x;
			Y = y;
			Z = z;
		}
		INLINE void SetZero()noexcept
		{
			X = false;
			Y = false;
			Z = false;
		}
		NODISCARD INLINE constexpr bool IsEqual(const Vector3b& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ false, false, false });
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z;
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
			auto z = Z ? trueValueTxt : falseValueTxt;
			return rtn.append(x).append(splitTxt).append(y).append(splitTxt).append(z);
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
			Z = split[2] == "true"sv ? true : false;

			return true;
		}

		static const Vector3b ZERO;
		static const Vector3b UNIT;
	};

	const Vector3b Vector3b::ZERO = Vector3b{ };
	const Vector3b Vector3b::UNIT = Vector3b(true, true, true);

	NODISCARD INLINE constexpr bool operator==(const Vector3b& left, const Vector3b& right)noexcept { return left.IsEqual(right); }
	NODISCARD INLINE constexpr bool operator!=(const Vector3b& left, const Vector3b& right)noexcept { return !(left == right); }
}

namespace std
{
	template<>
	struct hash<greaper::math::Vector3b>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector3b& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR3B_H */