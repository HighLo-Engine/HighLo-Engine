// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-18) initial release
//

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

struct ListTests : public testing::Test
{
	List<HLString> List;

	ListTests()
	{
		List.Append("First");
		List.Append("Second");
		List.Append("Third");
	}

	virtual ~ListTests()
	{
	}
};

TEST_F(ListTests, IsEmpty)
{
	EXPECT_EQ(List.IsEmpty(), false);
}

TEST_F(ListTests, GetSize)
{
	EXPECT_EQ(List.Size(), 3);
}

TEST_F(ListTests, Clear)
{
	List.Clear();
	EXPECT_EQ(List.Size(), 0);
}

TEST_F(ListTests, Append)
{
	EXPECT_NE(List.Size(), 0);
	EXPECT_EQ(StringEquals(List.GetFirst(), "First"), true);
}

TEST_F(ListTests, Insert)
{
	List.Insert("Zero");

	EXPECT_NE(List.Size(), 0);
	EXPECT_EQ(StringEquals(List.GetFirst(), "Zero"), true);
}

TEST_F(ListTests, GetFirstElement)
{
	HLString result = List.GetFirst();
	EXPECT_EQ(StringEquals(result, "First"), true);
}

TEST_F(ListTests, GetLastElement)
{
	HLString result = List.GetLast();
	EXPECT_EQ(StringEquals(result, "Third"), true);
}

TEST_F(ListTests, GetAnyElement)
{
	HLString result = List.Get(1);
	EXPECT_EQ(StringEquals(result, "Second"), true);
}

TEST_F(ListTests, RemoveFirstElement)
{
	List.RemoveFirst();
	HLString result = List.GetFirst();
	EXPECT_EQ(StringEquals(result, "Second"), true);
}

TEST_F(ListTests, RemoveLastElement)
{
	List.RemoveLast();
	HLString result = List.GetLast();
	EXPECT_EQ(StringEquals(result, "Second"), true);
}

TEST_F(ListTests, RemoveAnyElement)
{
	List.Remove(1);
	HLString result = List.Get(1);
	EXPECT_EQ(StringEquals(result, "Third"), true);
}

TEST_F(ListTests, ConvertToArray)
{
	HLString expected[] =
	{
		"First",
		"Second",
		"Third",
	};

	uint32 size;
	HLString *arr = List.ToArray(&size);

	EXPECT_EQ(ArrayEquals(expected, 3, arr, size), true);
}

