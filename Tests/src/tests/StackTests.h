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

struct StackTests : public testing::Test
	{
	HLStack<HLString> Stack;

	StackTests()
	{
		Stack.Push("Hello World!");
		Stack.Push("Test!");
	}

	virtual ~StackTests()
	{
	}
};

TEST_F(StackTests, Push)
{
	Stack.Push("LoL");
	HLString result = Stack.Top();
	EXPECT_EQ(StringEquals("LoL", result), true);
}

TEST_F(StackTests, Pop)
{
	Stack.Pop();
	HLString result = Stack.Top();
	EXPECT_EQ(StringEquals("Hello World!", result), true);
}

TEST_F(StackTests, Top)
{
	HLString result = Stack.Top();
	EXPECT_EQ(StringEquals("Test!", result), true);
}

TEST_F(StackTests, IsEmpty)
{
	EXPECT_EQ(Stack.IsEmpty(), false);
}

TEST_F(StackTests, Size)
{
	EXPECT_EQ(Stack.Size(), 2);
}

TEST_F(StackTests, Clear)
{
	Stack.Clear();
	EXPECT_EQ(Stack.Size(), 0);
	EXPECT_EQ(Stack.IsEmpty(), true);
}

