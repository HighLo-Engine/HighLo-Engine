// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2022-01-13) Added LexicalCast function (it can convert any type to any different type)
//     - 1.3 (2022-01-13) Added String conversion functions
//     - 1.2 (2021-11-19) Added Compare functions
//     - 1.1 (2021-09-29) Removed String conversion function because it should be natively now possible with our own String class
//     - 1.0 (2021-09-26) initial release
//

#pragma once

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

		static HLString FillWithLeading(HLString &str, const HLString &leadingCharacter, uint32 count = 1)
		{
			HLString leadingString = "";
			for (uint32 i = 0; i < count; ++i)
			{
				leadingString += leadingCharacter;
			}

			str = leadingString + str;
			return str;
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

		static HLString BytesToString(uint64 bytes)
		{
			constexpr uint64 GB = 1024 * 1024 * 1024;
			constexpr uint64 MB = 1024 * 1024;
			constexpr uint64 KB = 1024;

			char buffer[32];

			if (bytes > GB)
				sprintf_s(buffer, "%.2f GB", (float)bytes / (float)GB);
			else if (bytes > MB)
				sprintf_s(buffer, "%.2f MB", (float)bytes / (float)MB);
			else if (bytes > KB)
				sprintf_s(buffer, "%.2f KB", (float)bytes / (float)KB);
			else
				sprintf_s(buffer, "%.2f bytes", (float)bytes);

			return HLString(buffer);
		}
	}
}

