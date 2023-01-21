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

struct VectorTests : public testing::Test
	{
	HLVector<HLString> Vector;

	VectorTests()
	{
		Vector.PushBack("Hello World!");
		Vector.PushBack("Test!");
		Vector.PushBack("Another Test!");
		Vector.PushBack("lol!");
		Vector.PushBack("test");
	}

	virtual ~VectorTests()
	{
	}
};

TEST_F(VectorTests, PushBack)
{
	HLString result = Vector[0];
	EXPECT_EQ(StringEquals(result, "Hello World!"), true);
}

TEST_F(VectorTests, EmplaceBack)
{
	Vector.EmplaceBack("Nice!");
	HLString result = Vector[Vector.Size() - 1];
	EXPECT_EQ(StringEquals(result, "Nice!"), true);
}

TEST_F(VectorTests, PopBack)
{
	Vector.PopBack();
	HLString result = Vector[Vector.Size() - 1];
	EXPECT_EQ(StringEquals(result, "lol!"), true);
}

TEST_F(VectorTests, Clear)
{
	Vector.Clear();
	EXPECT_EQ(Vector.Size(), 0);
	EXPECT_EQ(Vector.Capacity(), 6);
	EXPECT_EQ(Vector.IsEmpty(), true);
}

TEST_F(VectorTests, Find)
{
	HLVectorIterator it = Vector.Find("lol!");
	EXPECT_EQ(StringEquals(*it, "lol!"), true);
}

TEST_F(VectorTests, GetSize)
{
	EXPECT_EQ(Vector.Size(), 5);
}

TEST_F(VectorTests, GetCapacity)
{
	EXPECT_EQ(Vector.Capacity(), 6);
}

TEST_F(VectorTests, IsEmpty)
{
	EXPECT_EQ(Vector.IsEmpty(), false);
}

TEST_F(VectorTests, Remove)
{
	Vector.Remove(3);
	EXPECT_EQ(StringEquals(Vector[3], "test"), true);
}

TEST_F(VectorTests, Reserve)
{
	Vector.Reserve(10);
	EXPECT_EQ(Vector.Capacity(), 10);
}

TEST_F(VectorTests, Resize)
{
	Vector.Resize(2);
	EXPECT_EQ(Vector.Size(), 2);
	EXPECT_EQ(Vector.Capacity(), 2);
}

TEST_F(VectorTests, GetFirst)
{
	EXPECT_EQ(StringEquals(Vector.Front(), "Hello World!"), true);
}

TEST_F(VectorTests, GetLast)
{
	EXPECT_EQ(StringEquals(Vector.Back(), "test"), true);
}

TEST_F(VectorTests, GetByArrayIndexOperator)
{
	EXPECT_EQ(StringEquals(Vector[3], "lol!"), true);
}

