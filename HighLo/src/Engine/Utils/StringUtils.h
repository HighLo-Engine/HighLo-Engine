// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include <string>
#include <codecvt>

namespace highlo
{
	namespace utils
	{
		static std::u32string ConvertToUtf32Str(const std::string &str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
			return conv.from_bytes(str);
		}

		static bool NextLine(int32 index, const std::vector<int32> &lines)
		{
			for (int32 line : lines)
			{
				if (line == index)
					return true;
			}

			return false;
		}

		static bool IsMatchingSearch(HLString &item, HLString &searchQuery, bool caseSensitive = false, bool stripWhiteSpaces = true, bool stripUnderscores = true)
		{
			if (searchQuery.IsEmpty())
				return true;

			if (item.IsEmpty())
				return false;

			HLString nameSanitized = stripUnderscores ? item.Replace("_", "") : item;
			if (stripWhiteSpaces)
				nameSanitized = nameSanitized.Replace(" ", "");

			HLString searchString = stripWhiteSpaces ? searchQuery.Replace(" ", "") : searchQuery;

			if (!caseSensitive)
			{
				nameSanitized = nameSanitized.ToLowerCase();
				searchString = searchString.ToLowerCase();
			}

			return nameSanitized.Contains(searchString);
		}


	}
}

