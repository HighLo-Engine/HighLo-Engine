// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-18) initial release
//

#pragma once

#undef TEST_CATEGORY
#define TEST_CATEGORY StringTests

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

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

TEST(TEST_CATEGORY, RemoveStringFromString)
{
	HLString str = "Hello World!";
	HLString expected = "Hello";

	str -= " World!";

	EXPECT_EQ(StringEquals(expected, str), true);
}

TEST(TEST_CATEGORY, RemoveCharFromString)
{
	HLString str = "Hello World!";
	HLString expected = "Hello World";

	str -= '!';

	EXPECT_EQ(StringEquals(str, expected), true);
}

TEST(TEST_CATEGORY, AtFuncTest)
{
	HLString str = "Hello World!";
	char expected = 'e';

	char result = str.At(1);

	EXPECT_EQ(result, expected);
}

TEST(TEST_CATEGORY, ArrIndexOperatorTest)
{
	HLString str = "Hello World!";
	char expected = 'e';

	char result = str[1];
	
	EXPECT_EQ(result, expected);
}

TEST(TEST_CATEGORY, DereferencingOperatorTest)
{
	HLString str = "Hello World!";
	char *expected = "Hello World!";

	char *result = *str;

	EXPECT_EQ(CStringEquals(result, expected), true);
}

TEST(TEST_CATEGORY, StringContainsLetter)
{
	HLString str = "Hello World!";
	bool result = str.Contains('e');
	EXPECT_EQ(result, true);
}

TEST(TEST_CATEGORY, StringContainsString)
{
	HLString str = "Hello World!";
	bool result = str.Contains("Hello");
	EXPECT_EQ(result, true);
}

TEST(TEST_CATEGORY, StringContainsNotLetter)
{
	HLString str = "Hello World!";
	bool result = str.Contains('f');
	EXPECT_EQ(result, false);
}

TEST(TEST_CATEGORY, StringContainsNotString)
{
	HLString str = "Hello World!";
	bool result = str.Contains("Car");
	EXPECT_EQ(result, false);
}

TEST(TEST_CATEGORY, ConvertStringToCStr)
{
	HLString str = "Hello World!";
	char *expected = "Hello World!";

	char *result = str.C_Str();

	EXPECT_EQ(CStringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, ConvertStringToWStr)
{
	HLString str = "Hello World!";
	wchar_t *expected = L"Hello World!";

	wchar_t *result = str.W_Str();

	EXPECT_EQ(WStringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, StringStartsWithLetter)
{
	HLString str = "Hello World!";
	bool result = str.StartsWith('H');
	EXPECT_EQ(result, true);
}

TEST(TEST_CATEGORY, StringStartsWithString)
{
	HLString str = "Hello World!";
	bool result = str.StartsWith("Hello");
	EXPECT_EQ(result, true);
}

TEST(TEST_CATEGORY, StringStartsNotWithLetter)
{
	HLString str = "Hello World!";
	bool result = str.StartsWith('e');
	EXPECT_EQ(result, false);
}

TEST(TEST_CATEGORY, StringStartsNotWithString)
{
	HLString str = "Hello World!";
	bool result = str.StartsWith("World!");
	EXPECT_EQ(result, false);
}

TEST(TEST_CATEGORY, StringEndsWithLetter)
{
	HLString str = "Hello World!";
	bool result = str.EndsWith('!');
	EXPECT_EQ(result, true);
}

TEST(TEST_CATEGORY, StringEndsWithString)
{
	HLString str = "Hello World!";
	bool result = str.EndsWith("World!");
	EXPECT_EQ(result, true);
}

TEST(TEST_CATEGORY, StringEndsNotWithLetter)
{
	HLString str = "Hello World!";
	bool result = str.EndsWith('H');
	EXPECT_EQ(result, false);
}

TEST(TEST_CATEGORY, StringEndsNotWithString)
{
	HLString str = "Hello World!";
	bool result = str.EndsWith("Hello");
	EXPECT_EQ(result, false);
}

TEST(TEST_CATEGORY, FirstIndexNotOf_Letter)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf('H');
	EXPECT_EQ(result, 1);
}

TEST(TEST_CATEGORY, FirstIndexNotOf_String)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf("Hello ");
	EXPECT_EQ(result, 6);
}

TEST(TEST_CATEGORY, FirstIndexNotOfNoLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf('W');
	EXPECT_EQ(result, 0);
}

TEST(TEST_CATEGORY, FirstIndexNotOfNoString)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf("World!");
	EXPECT_EQ(result, 0);
}

TEST(TEST_CATEGORY, FirstIndexOfLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf('H');
	EXPECT_EQ(result, 0);
}

TEST(TEST_CATEGORY, FirstIndexOfString)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf("Hello");
	EXPECT_EQ(result, 0);
}

TEST(TEST_CATEGORY, FirstIndexNotOfLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf("E");
	EXPECT_EQ(result, HLString::NPOS);
}

TEST(TEST_CATEGORY, FirstIndexNotOfString)
{
	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf("Car");
	EXPECT_EQ(result, HLString::NPOS);
}

TEST(TEST_CATEGORY, IndexOfLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.IndexOf(' ');
	EXPECT_EQ(result, 5);
}

TEST(TEST_CATEGORY, IndexOfString)
{
	HLString str = "Hello World!";
	uint32 result = str.IndexOf("World!");
	EXPECT_EQ(result, 6);
}

TEST(TEST_CATEGORY, IndexOfNoLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.IndexOf('F');
	EXPECT_EQ(result, HLString::NPOS);
}

TEST(TEST_CATEGORY, IndexOfNoString)
{
	HLString str = "Hello World!";
	uint32 result = str.IndexOf("Car");
	EXPECT_EQ(result, HLString::NPOS);
}

TEST(TEST_CATEGORY, LastIndexOfLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf('o');
	EXPECT_EQ(result, 7);
}

TEST(TEST_CATEGORY, LastIndexOfString)
{
	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf("World!");
	EXPECT_EQ(result, 6);
}

TEST(TEST_CATEGORY, LastIndexOfNoLetter)
{
	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf('F');
	EXPECT_EQ(result, HLString::NPOS);
}

TEST(TEST_CATEGORY, LastIndexOfNoString)
{
	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf("Car");
	EXPECT_EQ(result, HLString::NPOS);
}

TEST(TEST_CATEGORY, InvertString)
{
	HLString str = "Hello World!";
	HLString expected = "!dlroW olleH";

	HLString result = str.Reverse();

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, LengthOfString)
{
	HLString str = "Hello World!";
	uint32 expected = 12;

	uint32 result = str.Length();

	EXPECT_EQ(expected, result);
}

TEST(TEST_CATEGORY, ResizeString)
{
	HLString str;

	str.Resize(32);
	uint32 result = str.Length();

	EXPECT_EQ(result, 32);
}

TEST(TEST_CATEGORY, EmptyString)
{
	HLString str = "";
	EXPECT_EQ(str.Length(), 0);
}

TEST(TEST_CATEGORY, NotEmptyString)
{
	HLString str = "Hello World!";
	EXPECT_NE(str.Length(), 0);
}

TEST(TEST_CATEGORY, LowerCaseString)
{
	HLString str = "Hello World!";
	HLString expected = "hello world!";

	HLString result = str.ToLowerCase();

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, UpperCaseString)
{
	HLString str = "Hello World!";
	HLString expected = "HELLO WORLD!";

	HLString result = str.ToUpperCase();

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, ReplaceString)
{
	HLString str = "hello World!";
	HLString expected = "HELLO World!";

	HLString result = str.Replace("hello", "HELLO");

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, ReplaceAllStrings)
{
	HLString str = "hello world! hello world! hello world!";
	HLString expected = "HELLO world! HELLO world! HELLO world!";
	HLString result = str.Replace("hello", "HELLO");

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SplitStringIntoVectorWithSpaceDelimiter)
{
	HLString str = "hello world! hello world! hello world!";
	std::vector<HLString> expected =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	std::vector<HLString> result = str.Split(' ');
	EXPECT_EQ(VectorEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SplitStringIntoPointerArrayWithSpaceDelimiter)
{
	HLString str = "hello world! hello world! hello world!";
	HLString expected[] =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	uint32 wordCount;
	HLString *result = str.Split(' ', &wordCount);

	EXPECT_EQ(ArrayEquals(expected, 6, result, wordCount), true);
}

TEST(TEST_CATEGORY, SplitStringIntoVectorWithCommaDelimiter)
{
	HLString str = "hello,world!,hello,world!,hello,world!";
	std::vector<HLString> expected =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	std::vector<HLString> result = str.Split(',');

	EXPECT_EQ(VectorEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SplitStringIntoPointerArrayWithCommaDelimiter)
{
	HLString str = "hello,world!,hello,world!,hello,world!";
	HLString expected[] =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	uint32 wordCount;
	HLString *result = str.Split(',', &wordCount);

	EXPECT_EQ(ArrayEquals(expected, 6, result, wordCount), true);
}

TEST(TEST_CATEGORY, SubstringFirst5)
{
	HLString str = "Hello World!";
	HLString expected = "Hello";

	HLString result = str.Substr(0, 5);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SubstringFirst10)
{
	HLString str = "Hello World!";
	HLString expected = "Hello Worl";

	HLString result = str.Substr(0, 10);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SubstringLast5)
{
	HLString str = "Hello World!";
	HLString expected = "orld!";

	HLString result = str.Substr(str.Length() - 5);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SubstringLast10)
{
	HLString str = "Hello World!";
	HLString expected = "llo World!";

	HLString result = str.Substr(str.Length() - 10);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SubstringMid5)
{
	HLString str = "Hello World!";
	HLString expected = " Worl";

	HLString result = str.Substr(5, 10);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, SubstringMid10)
{
	HLString str = "Hello World!";
	HLString expected = " W";

	HLString result = str.Substr(5, str.Length() - 5);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, IntToString)
{
	int32 value = 42;
	HLString expected = "42";

	HLString result = HLString::ToString(value);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, FloatToString)
{
	float value = 42.4f;
	HLString expected = "42.4";

	HLString result = HLString::ToString(value);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, DoubleToString)
{
	double value = 42.4;
	HLString expected = "42.4";

	HLString result = HLString::ToString(value);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, BoolTrueToString)
{
	bool value = true;
	HLString expected = "1";

	HLString result = HLString::ToString(value);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, BoolFalseToString)
{
	bool value = false;
	HLString expected = "0";

	HLString result = HLString::ToString(value);

	EXPECT_EQ(StringEquals(expected, result), true);
}

TEST(TEST_CATEGORY, HashString)
{
	HLString str = "Hello World!";
	EXPECT_EQ(str.Hash(), 10459095042125088882);
}

TEST(TEST_CATEGORY, HashStrings)
{
	HLString str = "Hello World!";
	HLString str1 = "Hello World!";

	EXPECT_EQ(str1.Hash(), str1.Hash());
}

TEST(TEST_CATEGORY, HashDifferentStrings)
{
	HLString str = "Hello World!";
	HLString str1 = "abc";

	EXPECT_NE(str.Hash(), str1.Hash());
}

TEST(TEST_CATEGORY, Compare1)
{
	HLString a = "Batman";
	HLString b = "Superman";

	int32 result = a.Compare(b);
	HLString resultText = result < 0 ? "Batman comes before Superman" : result > 0 ? "Superman comes before Batman" : "Superman and Batman are the same";

	EXPECT_EQ(StringEquals(resultText, "Batman comes before Superman"), true);
}

TEST(TEST_CATEGORY, Compare2)
{
	HLString a = "Batman";
	HLString b = "Superman";

	int32 result = a.Compare(3, 3, b);
	HLString resultText = result < 0 ? "man comes before Superman" : result > 0 ? "Superman comes before man" : "Superman and man are the same";

	EXPECT_EQ(StringEquals(resultText, "Superman comes before man"), true);
}

TEST(TEST_CATEGORY, Compare3)
{
	HLString a = "Batman";
	HLString b = "Superman";

	int32 result = a.Compare(3, 3, b, 5, 3);
	HLString resultText = result < 0 ? "man comes before man" : result > 0 ? "man comes before man" : "man and man are the same";

	EXPECT_EQ(StringEquals(resultText, "man and man are the same"), true);
}

TEST(TEST_CATEGORY, LexicographicalCompare)
{
	std::vector<HLString> v1{ "A", "b", "c", "d" };
	std::vector<HLString> v2{ "a", "b", "c", "d" };

	bool isLess = HLString::LexicographicalCompare(v1.begin(), v1.end(), v2.begin(), v2.end());
	EXPECT_EQ(isLess, true);
}

TEST(TEST_CATEGORY, EqualsOperator)
{
	HLString a = "Batman";
	HLString b = "Batman";

	EXPECT_EQ(a == b, true);
}

TEST(TEST_CATEGORY, NotEqualsOperator)
{
	HLString a = "Batman";
	HLString b = "Superman";

	EXPECT_EQ(a != b, true);
}

TEST(TEST_CATEGORY, LessOperator)
{
	HLString a = "Batman";
	HLString b = "Superman";

	EXPECT_EQ(a < b, true);
}

TEST(TEST_CATEGORY, LessOrEqualOperator)
{
	HLString a = "Batman";
	HLString b = "Superman";

	EXPECT_EQ(a <= b, true);
}

TEST(TEST_CATEGORY, GreaterOperator)
{
	HLString a = "Batman";
	HLString b = "Superman";

	EXPECT_EQ(a > b, false);
}

TEST(TEST_CATEGORY, GreaterOrEqualOperator)
{
	HLString a = "Batman";
	HLString b = "Superman";

	EXPECT_EQ(a >= b, false);
}

// TOOD: Add Matrix/Vectors ToString checks

