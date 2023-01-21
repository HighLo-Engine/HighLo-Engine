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

struct HashmapTests : public testing::Test
{
	HLHashmap<int32, HLString> Hashmap;

	HashmapTests()
	{
		Hashmap.Put(42, "Hello World!");
		Hashmap.Put(50, "Test");
		Hashmap.Put(100, "Another Test");
	}

	virtual ~HashmapTests()
	{
	}
};

TEST_F(HashmapTests, Put)
{
	EXPECT_EQ(StringEquals("Hello World!", Hashmap.GetFirst()), true);
}

TEST_F(HashmapTests, Set)
{
	Hashmap.Set(42, "Test");
	EXPECT_EQ(StringEquals("Test", Hashmap.GetFirst()), true);
}

TEST_F(HashmapTests, HasKey)
{
	EXPECT_EQ(Hashmap.HasKey(42), true);
}

TEST_F(HashmapTests, HasValue)
{
	EXPECT_EQ(Hashmap.HasValue("Hello World!"), true);
}

TEST_F(HashmapTests, GetFirst)
{
	EXPECT_EQ(StringEquals("Hello World!", Hashmap.GetFirst()), true);
}

TEST_F(HashmapTests, GetLast)
{
	EXPECT_EQ(StringEquals("Another Test", Hashmap.GetLast()), true);
}

TEST_F(HashmapTests, GetAny)
{
	EXPECT_EQ(StringEquals("Test", Hashmap.GetAt(1)), true);
}

TEST_F(HashmapTests, GetAnyKey)
{
	EXPECT_EQ(50, Hashmap.GetKey(1));
}

TEST_F(HashmapTests, GetByIndexOperator)
{
	HLString result = Hashmap[50];
	EXPECT_EQ(StringEquals("Test", result), true);
}

TEST_F(HashmapTests, IsEmpty)
{
	EXPECT_EQ(Hashmap.IsEmpty(), false);
}

TEST_F(HashmapTests, GetSize)
{
	EXPECT_EQ(3, Hashmap.Size());
}

TEST_F(HashmapTests, RemoveAll)
{
	Hashmap.RemoveAll();
	EXPECT_EQ(Hashmap.Size(), 0);
	EXPECT_EQ(Hashmap.IsEmpty(), true);
}

TEST_F(HashmapTests, RemoveFirst)
{
	Hashmap.RemoveFirst();
	EXPECT_EQ(StringEquals("Test", Hashmap.GetFirst()), true);
}

TEST_F(HashmapTests, RemoveLast)
{
	Hashmap.RemoveLast();
	EXPECT_EQ(StringEquals("Test", Hashmap.GetLast()), true);
}

TEST_F(HashmapTests, RemoveByFullPair)
{
	Hashmap.Remove(50, "Test");
	EXPECT_EQ(StringEquals("Another Test", Hashmap.GetAt(1)), true);
}

TEST_F(HashmapTests, RemoveByKey)
{
	Hashmap.Remove(50);
	EXPECT_EQ(StringEquals("Another Test", Hashmap.GetAt(1)), true);
}

