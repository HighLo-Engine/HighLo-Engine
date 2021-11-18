#pragma once

#define TEST_CATEGORY StringTests

#include <HighLo.h>
#include <gtest/gtest.h>

using namespace highlo;

static bool StringEquals(const HLString &str1, const HLString &str2)
{
	return strcmp(*str1, *str2) == 0;
}

TEST(TEST_CATEGORY, AppendStringToString)
{
	HLString str1 = "Hello";
	HLString str2 = " World!";
	HLString expected = "Hello World!";
	
	HLString result = str1 + str2;

	EXPECT_EQ(StringEquals(result, expected), true);
}

TEST(TEST_CATEGORY, AppendCharToString)
{
	HLString str = "Hello World";
	char letter = '!';
	HLString expected = "Hello World!";

	HLString result = str + letter;

	EXPECT_EQ(StringEquals(result, expected), true);
}

