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

template<typename T>
static bool VectorEquals(const std::vector<T> &vec1, const std::vector<T> &vec2)
{
	if (vec1.size() != vec2.size())
		return false;

	for (uint32 i = 0; i < vec1.size(); ++i)
	{
		if (vec1[i] != vec2[i])
			return false;
	}

	return true;
}

template<typename T>
static bool MapEquals(const std::map<HLString, T> &map1, const std::map<HLString, T> &map2)
{
	if (map1.size() != map2.size())
		return false;

	for (auto it_m1 = map1.cbegin(), end_m1 = map1.cend(),
		 it_m2 = map2.cbegin(), end_m2 = map2.cend();
		 it_m1 != end_m1 || it_m2 != end_m2;)
	{
		if (it_m1 != end_m1 && it_m2 != end_m2)
		{
			if (it_m1->first != it_m2->first)
				return false;

			if (it_m1->second != it_m2->second)
				return false;

			++it_m1;
			++it_m2;
		}
	}

	return true;
}

template<typename T>
static bool ArrayEquals(T *arr1, uint32 arr1Size, T *arr2, uint32 arr2Size)
{
	if (arr1Size != arr2Size)
		return false;

	for (uint32 i = 0; i < arr1Size; ++i)
	{
		if (arr1[i] != arr2[i])
			return false;
	}

	return true;
}

