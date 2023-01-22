#include "HighLoPch.h"
#include "StringUtils.h"

#include <codecvt>

namespace highlo::utils
{
	bool NextLine(int32 index, const std::vector<int32> &lines)
	{
		for (int32 line : lines)
		{
			if (line == index)
				return true;
		}

		return false;
	}

	HLString FillWithLeading(HLString &str, const HLString &leadingCharacter, uint32 count)
	{
		HLString leadingString = "";
		for (uint32 i = 0; i < count; ++i)
		{
			leadingString += leadingCharacter;
		}

		str = leadingString + str;
		return str;
	}
}

