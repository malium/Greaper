/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef GREAPER_CORE_STRING_UTILS_H
#define GREAPER_CORE_STRING_UTILS_H 1

#include "PHAL.h"
#include "Memory.h"
#include <iterator>

namespace greaper::StringUtils
{
	namespace Impl
	{
		template<typename T>
		T UTF8To32(T begin, T end, u32char& output, u32char invalidChar = 0)
		{
			if (begin >= end)
				return begin;

			static constexpr uint32 offsets[6] = { 0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080 };

			uint32 numBytes = 0;

			const auto firstByte = (uint8)*begin;
			if (firstByte < 192)
				numBytes = 1;
			else if (firstByte < 224)
				numBytes = 2;
			else if (firstByte < 240)
				numBytes = 3;
			else if (firstByte < 248)
				numBytes = 4;
			else if (firstByte < 252)
				numBytes = 5;
			else
				numBytes = 6;

			if ((begin + numBytes) > end)
			{
				output = invalidChar;
				return end;
			}

			output = 0;
			switch (numBytes)
			{
				case 6: output += (uint8)(*begin); ++begin; output <<= 6;
				case 5: output += (uint8)(*begin); ++begin; output <<= 6;
				case 4: output += (uint8)(*begin); ++begin; output <<= 6;
				case 3: output += (uint8)(*begin); ++begin; output <<= 6;
				case 2: output += (uint8)(*begin); ++begin; output <<= 6;
				case 1: output += (uint8)(*begin); ++begin;
				default: break;
			}
			output -= offsets[numBytes - 1];
			return begin;
		}

		template<typename T>
		T UTF32To8(u32char input, T output, uint32 maxElems, achar invalidChar = 0)
		{
			if (maxElems == 0)
				return output;
			
			static constexpr uint8 headers[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

			if ((input > 0x0010FFFF) || ((input >= 0xD800) && (input <= 0xDBFF)))
			{
				*output = invalidChar;
				++output;
				return output;
			}

			uint32 numBytes = 0;;
			if (input < 0x80)
				numBytes = 1;
			else if (input < 0x800)
				numBytes = 2;
			else if (input < 0x10000)
				numBytes = 3;
			else
				numBytes = 4;

			if (numBytes > maxElems)
			{
				*output = invalidChar;
				++output;
				return output;
			}

			achar bytes[4];
			switch (numBytes)
			{
				case 4: bytes[3] = (achar)((input | 0x80) & 0xBF); input >>= 6;
				case 3: bytes[2] = (achar)((input | 0x80) & 0xBF); input >>= 6;
				case 2: bytes[1] = (achar)((input | 0x80) & 0xBF); input >>= 6;
				case 1: bytes[0] = (achar)(input | headers[numBytes]);
				default: break;
			}

			output = std::copy(bytes, bytes + numBytes, output);
			return output;
		}

		template<typename T>
		T UTF16To32(T begin, T end, u32char& output, u32char invalidChar = 0)
		{
			if (begin >= end)
				return begin;

			const auto firstElem = (u16char)*begin;
			++begin;

			if ((firstElem >= 0xD800) && (firstElem <= 0xDBFF))
			{
				if (begin >= end)
				{
					output = invalidChar;
					return end;
				}

				const auto secondElem = (u32char)*begin;
				++begin;

				if ((secondElem >= 0xDC00) && (secondElem <= 0xDFFF))
					output = (u32char)(((firstElem - 0xD800) << 10) + (secondElem - 0xDC00) + 0x0010000);
				else
					output = invalidChar;
			}
			else
			{
				output = (u32char)firstElem;
				return begin;
			}
			return begin;
		}

		template<typename T>
		static T UTF32To16(u32char input, T output, uint32 maxElems, u16char invalidChar = 0)
		{
			if (maxElems == 0)
				return output;

			if (input > 0x0010FFFF)
			{
				*output = invalidChar;
				++output;

				return output;
			}

			if (input <= 0xFFFF)
			{
				if ((input >= 0xD800) && (input <= 0xDFFF))
				{
					*output = invalidChar;
					++output;

					return output;
				}

				*output = (u16char)input;
				++output;
			}
			else
			{
				if (maxElems < 2)
				{
					*output = invalidChar;
					++output;

					return output;
				}

				input -= 0x0010000;
				*output = (u16char)((input >> 10) + 0xD800);
				++output;

				*output = (u16char)((input & 0x3FFUL) + 0xDC00);
				++output;
			}
			return output;
		}

		template<typename T>
		T WIDEToUTF32(T begin, T end, u32char& output, u32char invalidChar = 0)
		{
#if PLT_LINUX
			output = (u32char)*begin;
			++begin;
			return begin;
#else
			return UTF16To32(begin, end, output, invalidChar);
#endif
		}

		template<typename T>
		T UTF32ToWIDE(u32char input, T output, uint32 maxElems, wchar invalidChar = 0)
		{
#if PLT_LINUX
			* output = (wchar)input;
			++output;

			return output;
#else
			return UTF32To16(input, output, maxElems, invalidChar);
#endif
		}

		template<typename T>
		struct CaseImpl
		{
			int(*Lower)(int C) = &::tolower;
			int(*Upper)(int C) = &::toupper;
		};

		template<>
		struct CaseImpl<achar>
		{
			int(*Lower)(int C) = &::tolower;
			int(*Upper)(int C) = &::toupper;
		};

		template<>
		struct CaseImpl<wchar>
		{
			wint_t(*Lower)(wint_t c) = &::towlower;
			wint_t(*Upper)(wint_t c) = &::towupper;
		};
	}

	/**
	 * @brief Converts a WIDE string into an ANSI string
	 * 
	 * @tparam _Alloca_ ANSI string allocator, auto deducted by param str
	 * @tparam _Allocw_ WIDE string allocator
	 * @param str The WIDE string
	 * @return BasicString<achar, _Alloca_> The converted ANSI string
	 */
	template<class _Alloca_ = StdAlloc<achar>, class _Allocw_ = StdAlloc<wchar>>
	BasicString<achar, _Alloca_> FromWIDE(const BasicString<wchar, _Allocw_>& str)
	{
		static_assert(std::is_same_v<wchar, _Allocw_::value_type>, "Bad WIDE allocator type");
		static_assert(std::is_same_v<achar, _Alloca_::value_type>, "Bad ANSI allocator type");
		
		BasicString<achar, _Alloca_> output;
		output.reserve(str.size());

		auto backInserter = std::back_inserter(output);

		for (auto it = str.begin(); it != str.end();)
		{
			u32char c32 = 0;
			it = Impl::WIDEToUTF32(it, str.end(), c32);
			Impl::UTF32To8(c32, backInserter, 4);
		}
		return output;
	}

	/**
	 * @brief Converts an ANSI string into a WIDE string
	 * 
	 * @tparam _Allocw_ WIDE string allocator
	 * @tparam _Alloca_ ANSI string allocator, auto deducted by param str
	 * @param str The ANSI string
	 * @return BasicString<wchar, _Allocw_> The Converted WIDE string
	 */
	template<class _Allocw_ = StdAlloc<wchar>, class _Alloca_ = StdAlloc<achar>>
	BasicString<wchar, _Allocw_> ToWIDE(const BasicString<achar, _Alloca_>& str)
	{
		static_assert(std::is_same_v<wchar, typename _Allocw_::value_type>, "Bad WIDE allocator type");
		static_assert(std::is_same_v<achar, typename _Alloca_::value_type>, "Bad ANSI allocator type");

		BasicString<wchar, _Allocw_> output;
		output.reserve(str.size());

		auto backInserter = std::back_inserter(output);

		for (auto it = str.begin(); it != str.end(); )
		{
			u32char c32 = 0;
			it = Impl::UTF8To32(it, str.end(), c32);
			Impl::UTF32ToWIDE(c32, backInserter, 2);

		}

		return output;
	}

	/**
	 * @brief Counts how many characters 'token' are inside the given string
	 * 
	 * @tparam T Type of character, auto deducted by param str
	 * @tparam _Alloca_ Allocator of the string, auto deducted by param str
	 * @param str The string to look for tokens
	 * @param token The character to find
	 * @return sizet The amount of characters found
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	sizet Contains(const BasicString<T, _Alloca_>& str, T token)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Trying to Contains and convert a string at the same time");
		
		sizet count = 0;
		for (const T c : str)
		{
			if (c == token)
				++count;
		}
		return count;
	}

	/**
	 * @brief Counts how many strings 'token' are inside the given string
	 * 
	 * @tparam T The type of character, auto deducted by param str
	 * @tparam _Alloca_ The Allocator of the string, auto deducted by param str
	 * @tparam _AllocT_ The Allocator of the token string, auto deducted by param token
	 * @param str The string to look for tokens
	 * @param token The string to find
	 * @return sizet The amount of strings found
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _AllocT_ = StdAlloc<T>>
	sizet Contains(const BasicString<T, _Alloca_>& str, const BasicString<T, _AllocT_>& token)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Trying to Contains and convert a string at the same time");
		static_assert(std::is_same_v<T, typename _AllocT_::value_type>, "Trying to Contains and convert a string at the same time");
		
		sizet count = 0;
		for (auto it = str.begin(); it != str.end(); ++it)
		{
			auto itt = it;
			bool match = true;
			for (auto ittt = token.begin(); ittt != token.end(); ++ittt)
			{
				if ((*itt) != (*ittt))
				{
					match = false;
					break;
				}
				++itt;
			}
			if (match)
				++count;
		}
		return count;
	}

	/**
	 * @brief Joins a vector of string into a single string with a given separator
	 * 
	 * @tparam T Type of char, auto deducted from vec and separator
	 * @tparam _Alloca_ The Allocator of the Strings inside the vector
	 * @tparam _VAlloca_ The Allocator of the Vector
	 * @param vec Vector of strings to compose into one
	 * @param separator The character that will be between the strings (NULL/0) no in-between character
	 * @return BasicString<T, _Alloca_> The string composed from all the strings in the vector
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<BasicString<T, _Alloca_>>>
	BasicString<T, _Alloca_> ComposeString(const std::vector<BasicString<T, _Alloca_>, _VAlloca_>& vec, T separator = T(0))
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Trying to ComposeString and convert a string at the same time");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::value_type>, "Trying to ComposeString and convert a string at the same time");
		
		BasicString<T, _Alloca_> rtn;
		if(separator != T(0))
		{
			for (sizet i = 0; i < vec.size(); ++i)
			{
				rtn += vec[i];
				if (i != (vec.size() - 1))
					rtn += separator;
			}
		}
		else
		{
			for(const auto& str : vec)
				rtn += str;
		}

		return rtn;
	}

	/**
	 * @brief Joins a vector of string into a single string with a given separator
	 * 
	 * @tparam T Type of char, auto deducted from vec and separator
	 * @tparam _Alloca_ The Allocator of the Strings inside the vector
	 * @tparam _VAlloca_ The Allocator of the Vector
	 * @tparam _SAlloca_ The Allocator of the separator string
	 * @param vec Vector of strings to compose into one
	 * @param separator The string that will be between the strings
	 * @return BasicString<T, _Alloca_> The string composed from all the strings in the vector
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<BasicString<T, _Alloca_>>, class _SAlloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> ComposeString(const std::vector<BasicString<T, _Alloca_>, _VAlloca_>& vec,
		const BasicString<T, _SAlloca_>& separator)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Trying to ComposeString and convert a string at the same time");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::value_type>, "Trying to ComposeString and convert a string at the same time");
		static_assert(std::is_same_v<T, typename _SAlloca_::value_type>, "Trying to ComposeString and convert a string at the same time");
		
		BasicString<T, _Alloca_> rtn;
		for (sizet i = 0; i < vec.size(); ++i)
		{
			rtn += vec[i];
			if (i != (vec.size() - 1))
				rtn += separator;
		}
		return rtn;
	}

	/**
	 * @brief Separates a string given a token character
	 * 
	 * @tparam T Type of char
	 * @tparam _Alloca_ The Allocator of the Strings inside the vector
	 * @tparam _VAlloca_ The Allocator of the Vector
	 * @param str The string that is going to be separated
	 * @param token The character to find in order to split the string
	 * @return std::vector<BasicString<T, _Alloca_>, _VAlloca_> The vector of splitted strings
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<BasicString<T, _Alloca_>>>
	std::vector<BasicString<T, _Alloca_>, _VAlloca_> Tokenize(const BasicString<T, _Alloca_>& str, T token)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Trying to Tokenize and convert a string at the same time");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::value_type>, "Trying to Tokenize and convert a string at the same time");
		
		std::vector<BasicString<T, _Alloca_>, _VAlloca_> rtn;
		if (str.empty())
			return rtn;
		auto lastToken = str.begin();
		for (auto it = str.begin(); it != str.end(); ++it)
		{
			if ((*it) == token)
			{
				rtn.emplace_back(str.substr(std::distance(str.begin(), lastToken), std::distance(lastToken, it)));
				lastToken = it;
				++lastToken;
			}
		}
		rtn.push_back(BasicString<T, _Alloca_>(lastToken, str.end()));
		return rtn;
	}

	/**
	 * @brief Splits a string given a token string
	 * 
	 * @tparam T The type of char
	 * @tparam _Alloca_ Allocator of the string to split
	 * @tparam _VAlloca_ Allocator of the Vector
	 * @tparam _TAlloca_ Allocator of the token
	 * @param str The string to split
	 * @param token The string to find
	 * @return std::vector<BasicString<T, _Alloca_>, _VAlloca_> The Vector of strings from str
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<BasicString<T, _Alloca_>>, class _TAlloca_ = StdAlloc<T>>
	std::vector<BasicString<T, _Alloca_>, _VAlloca_> Tokenize(const BasicString<T, _Alloca_>& str,
		const BasicString<T, _TAlloca_>& token)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::value_type>, "Bad StringVec allocator type");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::allocator_type::value_type>, "Bad StringVec allocator type");
		
		std::vector<BasicString<T, _Alloca_>, _VAlloca_> rtn;
		if (str.empty())
			return rtn;
		auto lastToken = str.begin();
		const size_t tokenSize = token.size();
		for (auto it = str.begin(); it != str.end(); ++it)
		{
			if (static_cast<size_t>(std::distance(it, str.end())) < tokenSize)
			{
				rtn.emplace_back(str.substr(std::distance(str.begin(), lastToken)));
				break;
			}
			auto itt = it;
			bool match = true;
			for (auto ittt = token.begin(); ittt != token.end(); ++ittt)
			{
				if ((*itt) != (*ittt))
				{
					match = false;
					break;
				}
				++itt;
			}
			if (match)
			{
				rtn.emplace_back(str.substr(std::distance(str.begin(), lastToken), tokenSize));
				it += tokenSize;
			}
		}
		rtn.push_back(BasicString<T, _Alloca_>(lastToken, str.end()));
		return rtn;
	}

	/**
	 * @brief Splits a string by spaces
	 * 
	 * @tparam T The type of character
	 * @tparam _Alloca_ The allocator of the string to split
	 * @tparam _VAlloca_ The allocator of the returned vector
	 * @param str The string to split
	 * @return std::vector<BasicString<T, _Alloca_>, _VAlloca_> The vector of strings
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<BasicString<T, _Alloca_>>>
	std::vector<BasicString<T, _Alloca_>, _VAlloca_> SeparateBySpace(const BasicString<T, _Alloca_>& str)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::value_type>, "Bad StringVec allocator");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type::allocator_type::value_type>, "Bad StringVec allocator");

		return Tokenize(str, T(' '));
	}

	/**
	 * @brief Removes the unnecessary characters from the beginning and the ending of the string
	 * 
	 * @tparam T The type of the character
	 * @tparam _Alloca_ The allocator of the string
	 * @tparam _VAlloca_ The allocator of the vector of characters to remove
	 * @param str The string to trim
	 * @param delims Which characters to remove
	 * @param left Trim the beginning?
	 * @param right Trim the ending?
	 * @return BasicString<T, _Alloca_>& The trimmed string
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_>& TrimSelf(BasicString<T, _Alloca_>& str, const Vector<T, _VAlloca_>& delims,
		bool left = true, bool right = true)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type>, "Bad string alloator type");

		if (right)
			str.erase(str.find_last_not_of(delims.data(), 0, delims.size()) + 1);
		if (left)
			str.erase(str.find_first_not_of(delims.data(), 0, delims.size()));
		return str;
	}

	/**
	 * @brief Removes the unnecessary characters from the beginning and the ending of the string
	 * 
	 * @tparam T The type of the character
	 * @tparam _Alloca_ The allocator of the string
	 * @tparam _VAlloca_ The allocator of the vector of characters to remove
	 * @param str The string to trim
	 * @param delims Which characters to remove
	 * @param left Trim the beginning?
	 * @param right Trim the ending?
	 * @return BasicString<T, _Alloca_> The trimmed string
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _VAlloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> Trim(const BasicString<T, _Alloca_>& str, const Vector<T, _VAlloca_>& delims,
		bool left = true, bool right = true)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");
		static_assert(std::is_same_v<T, typename _VAlloca_::value_type>, "Bad string alloator type");

		BasicString<T, _Alloca_> cpy{str};

		TrimSelf(cpy, delims, left, right);
		return cpy;
	}

	/**
	 * @brief Removes the unnecessary characters from the beginning and the ending of the string
	 * 
	 * @tparam T The type of the character
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to trim
	 * @param left Trim the beginning?
	 * @param right Trim the ending?
	 * @return BasicString<T, _Alloca_>& The trimmed string
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_>& TrimSelf(BasicString<T, _Alloca_>& str, bool left = true, bool right = true)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		//static constexpr T delims[] = { T(' '), T('\t'), T('\r'), T('\000') };
		static constexpr Vector<T, _Alloca_> delims {T(' '), T('\t', T('\r'), T('\n'))};
		return TrimSelf(str, delims, left, right);
	}

	/**
	 * @brief Removes the unnecessary characters from the beginning and the ending of the string
	 * 
	 * @tparam T The type of the character
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to trim
	 * @param left Trim the beginning?
	 * @param right Trim the ending?
	 * @return BasicString<T, _Alloca_> The trimmed string
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> Trim(const BasicString<T, _Alloca_>& str, bool left = true, bool right = true)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		static constexpr Vector<T, _Alloca_> delims {T(' '), T('\t', T('\r'), T('\n'))};
		return Trim(str, delims, left, right);
	}

	/**
	 * @brief Replaces a given character to another from a string
	 * 
	 * @tparam T The type of the character
	 * @tparam _Alloca_ The allocator type of the string
	 * @param str The string to replace the character
	 * @param toReplace The character wanted to replace
	 * @param replacement The character that subtitutes
	 * @return BasicString<T, _Alloca_> The string with the characters replaced
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> Replace(const BasicString<T, _Alloca_>& str, T toReplace, T replacement)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		return ComposeString(Tokenize(str, toReplace), replacement);
	}

	/**
	 * @brief Replaces a given string to another from a string
	 * 
	 * @tparam T The type of the character
	 * @tparam _Alloca_ The allocator type of the string str
	 * @tparam _TAlloca_ The allocator type of the string toReplace
	 * @tparam _RAlloca_ The allocator type of the string replacement
	 * @param str The string to find toReplace and change it for replacement
	 * @param toReplace The string to find inside str
	 * @param replacement The string to substitute 
	 * @return BasicString<T, _Alloca_> The resulting string
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>, class _TAlloca_ = StdAlloc<T>, class _RAlloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> Replace(const BasicString<T, _Alloca_>& str,
		const BasicString<T, _TAlloca_>& toReplace, const BasicString<T, _RAlloca_>& replacement)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");
		static_assert(std::is_same_v<T, typename _TAlloca_::value_type>, "Bad string allocator type");
		static_assert(std::is_same_v<T, typename _RAlloca_::value_type>, "Bad string allocator type");

		return ComposeString(Tokenize(str, toReplace), replacement);
	}

	/**
	 * @brief Lowers the given string
	 * 
	 * @tparam T The type of the characters
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to lower
	 * @return BasicString<T, _Alloca_>& The string lowered
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_>& ToLowerSelf(BasicString<T, _Alloca_>& str)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		Impl::CaseImpl<T> caseTransform;
		for (T& c : str)
			c = (T)caseTransform.Lower(c);
		return str;
	}
	
	/**
	 * @brief Lowers the given string
	 * 
	 * @tparam T The type of the characters
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to lower
	 * @return BasicString<T, _Alloca_> The string lowered
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> ToLower(const BasicString<T, _Alloca_>& str)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		BasicString<T, _Alloca_> temp{ str.c_str(), str.size() };
		return ToLowerSelf(temp);
	}

	/**
	 * @brief Uppers the given string
	 * 
	 * @tparam T The type of the characters
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to upper
	 * @return BasicString<T, _Alloca_>& The string uppered
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_>& ToUpperSelf(BasicString<T, _Alloca_>& str)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		Impl::CaseImpl<T> caseTransform;
		for (T& c : str)
			c = (T)caseTransform.Upper(c);
		return str;
	}

	/**
	 * @brief Uppers the given string
	 * 
	 * @tparam T The type of the characters
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to upper
	 * @return BasicString<T, _Alloca_> The string uppered
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	BasicString<T, _Alloca_> ToUpper(const BasicString<T, _Alloca_>& str)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");

		BasicString<T, _Alloca_> temp{ str.c_str(), str.size() };
		return ToUpperSelf(temp);
	}

	/**
	 * @brief Returns whether if the given string is only numbers or contains other information
	 * 
	 * @tparam T The type of the characters
	 * @tparam _Alloca_ The allocator of the string
	 * @param str The string to check
	 * @return true The string only contains numbers
	 * @return false The string contains other characters
	 */
	template<typename T, class _Alloca_ = StdAlloc<T>>
	bool StringNumber(const BasicString<T, _Alloca_>& str)
	{
		static_assert(std::is_same_v<T, typename _Alloca_::value_type>, "Bad string allocator type");
		
		for (auto& chr : str)
		{
			if (!IsWithinInclusive(chr, T('0'), T('9')) && chr != T(',') && chr != T('.') && chr != T('+') && chr != T('-') && chr != T('e') && chr != T('E'))
				return false;
		}
		return true;
	}
}

#endif /* GREAPER_CORE_STRING_UTILS_H */
