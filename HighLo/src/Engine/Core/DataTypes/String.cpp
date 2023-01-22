// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "String.h"

#include <codecvt>

namespace highlo::utils
{
	HLString32 ToUTF32(const HLString &str)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
		std::u32string result = conv.from_bytes(*str);
		return HLString32(result.c_str(), (uint32)result.length());
	}

	HLString16 ToUTF16(const HLString &str)
	{
		std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
		std::u16string result = conv.from_bytes(*str);
		return HLString16(result.c_str(), (uint32)result.length());
	}

	HLString ToUTF8(const HLString32 &str)
	{
		std::wstring_convert<std::codecvt_utf8<int32>, int32> conv;
		const int32 *p = reinterpret_cast<const int32*>(*str);
		return conv.to_bytes(p, p + str.Length());
	}

	HLString ToUTF8(const HLString16 &str)
	{
		std::wstring_convert<std::codecvt_utf8<int16>, int16> conv;
		const int16 *p = reinterpret_cast<const int16*>(*str);
		return conv.to_bytes(p, p + str.Length());
	}
}
