/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef MATH_VECTOR4B_H
#define MATH_VECTOR4B_H 1

#include "Vector3b.inl"
#include "VecRef.h"

namespace greaper::math
{
	class Vector4b
	{
	public:
		static constexpr sizet ComponentCount = 4;
		using value_type = bool;

		bool X = false;
		bool Y = false;
		bool Z = false;
		bool W = false;

		constexpr Vector4b()noexcept = default;
		INLINE constexpr Vector4b(bool x, bool y, bool z, bool w)noexcept :X(x), Y(y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4b(const std::array<bool, ComponentCount>& arr) : X(arr[0]), Y(arr[1]), Z(arr[2]), W(arr[3]) {  }
		INLINE constexpr explicit Vector4b(const Vector2b& v2, bool z, bool w)noexcept :X(v2.X), Y(v2.Y), Z(z), W(w) {  }
		INLINE constexpr explicit Vector4b(const Vector2b& v20, const Vector2b& v21)noexcept :X(v20.X), Y(v20.Y), Z(v21.X), W(v21.Y) {  }
		INLINE constexpr explicit Vector4b(const Vector3b& v3, bool w)noexcept :X(v3.X), Y(v3.Y), Z(v3.Z), W(w) {  }

		NODISCARD INLINE constexpr bool& operator[](sizet index)noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		NODISCARD INLINE constexpr const bool& operator[](sizet index)const noexcept
		{
			VerifyLess(index, ComponentCount, "Trying to access a Vector4, but the index %" PRIuPTR " was out of range.", index);
			return (&X)[index];
		}
		DEF_SWIZZLE_VEC4();
		NODISCARD INLINE constexpr std::array<bool, ComponentCount> ToArray()const noexcept
		{
			return { X, Y, Z, W };
		}
		INLINE void Set(const Vector4b& other)noexcept
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
		}
		INLINE void Set(bool x, bool y, bool z, bool w)noexcept
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		INLINE void SetZero()noexcept
		{
			X = false;
			Y = false;
			Z = false;
			W = false;
		}
		NODISCARD INLINE constexpr bool IsEqual(const Vector4b& other)const noexcept
		{
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
		NODISCARD INLINE constexpr bool IsZero()const noexcept
		{
			return IsEqual({ false, false, false, false });
		}
		NODISCARD INLINE constexpr bool AreComponentsEqual()const noexcept
		{
			return X == Y && X == Z && X == W;
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
			auto w = W ? trueValueTxt : falseValueTxt;
			return rtn.append(x).append(splitTxt).append(y).append(splitTxt).append(z).append(splitTxt).append(w);
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
			W = split[3] == "true"sv ? true : false;

			return true;
		}

		static const Vector4b ZERO;
		static const Vector4b UNIT;
	};

	inline const Vector4b Vector4b::ZERO = Vector4b{};
	inline const Vector4b Vector4b::UNIT = Vector4b(true, true, true, true);

	NODISCARD INLINE constexpr bool operator==(const Vector4b& left, const Vector4b& right)noexcept { return left.IsEqual(right); }
	NODISCARD INLINE constexpr bool operator!=(const Vector4b& left, const Vector4b& right)noexcept { return !(left == right); }
}

namespace std
{
	template<>
	struct hash<greaper::math::Vector4b>
	{
		NODISCARD INLINE size_t operator()(const greaper::math::Vector4b& v)const noexcept
		{
			return ComputeHash(v.X, v.Y, v.Z);
		}
	};
}

#endif /* MATH_VECTOR4B_H */