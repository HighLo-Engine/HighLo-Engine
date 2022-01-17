// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2022-01-13) Added LexicalCast function (it can convert any type to any different type)
//     - 1.3 (2022-01-13) Added String conversion functions
//     - 1.2 (2021-11-19) Added Compare functions
//     - 1.1 (2021-09-29) Removed String conversion function because it should be natively now possible with our own String class
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include <codecvt>

namespace highlo
{
	namespace utils
	{
		static bool NextLine(int32 index, const std::vector<int32> &lines)
		{
			for (int32 line : lines)
			{
				if (line == index)
					return true;
			}

			return false;
		}

		static HLString32 ToUTF32(const HLString &str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
			std::u32string result = conv.from_bytes(*str);
			return HLString32(result.c_str(), (uint32)result.length());
		}

		static HLString16 ToUTF16(const HLString &str)
		{
			std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
			std::u16string result = conv.from_bytes(*str);
			return HLString16(result.c_str(), (uint32)result.length());
		}

		static HLString ToUTF8(const HLString32 &str)
		{
			std::wstring_convert<std::codecvt_utf8<int32>, int32> conv;
			const int32 *p = reinterpret_cast<const int32*>(*str);
			return conv.to_bytes(p, p + str.Length());
		}

		static HLString ToUTF8(const HLString16 &str)
		{
			std::wstring_convert<std::codecvt_utf8<int16>, int16> conv;
			const int16 *p = reinterpret_cast<const int16*>(*str);
			return conv.to_bytes(p, p + str.Length());
		}

		template<typename toType, typename fromType>
		static toType LexicalCast(fromType const &x)
		{
			std::stringstream os;
			toType result;

			os << x;
			os >> result;

			return result;
		}
	}
}

