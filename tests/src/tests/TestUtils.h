// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-18) initial release
//

#pragma once

#include <HighLo.h>

using namespace highlo;

static bool CStringEquals(const char *str1, const char *str2)
{
	return strcmp(str1, str2) == 0;
}

static bool WStringEquals(const wchar_t *str1, const wchar_t *str2)
{
	return wcscmp(str1, str2) == 0;
}

static bool StringEquals(const HLString &str1, const HLString &str2)
{
	return CStringEquals(*str1, *str2);
}

static bool VectorEquals(const std::vector<HLString> &vec1, const std::vector<HLString> &vec2)
{
	if (vec1.size() != vec2.size())
		return false;

	bool equals = true;
	for (uint32 i = 0; i < vec1.size(); ++i)
	{
		if (vec1[i] != vec2[i])
		{
			equals = false;
			break;
		}
	}

	return equals;
}

static bool ArrayEquals(HLString *arr1, uint32 arr1Size, HLString *arr2, uint32 arr2Size)
{
	if (arr1Size != arr2Size)
		return false;

	bool equals = true;
	for (uint32 i = 0; i < arr1Size; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			equals = false;
			break;
		}
	}

	return equals;
}

