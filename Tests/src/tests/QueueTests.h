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

struct QueueTests : public testing::Test
	{
	HLQueue<HLString> Queue;

	QueueTests()
	{
		Queue.Enqueue("Hello World!");
		Queue.Enqueue("Test!");
		Queue.Enqueue("LOL");
	}

	virtual ~QueueTests()
	{
	}
};

TEST_F(QueueTests, Enqueue)
{
	Queue.Enqueue("Another Test");
	EXPECT_EQ(StringEquals(Queue.Front(), "Hello World!"), true);
	EXPECT_EQ(Queue.Size(), 4);
}

TEST_F(QueueTests, Dequeue)
{
	Queue.Dequeue();
	EXPECT_EQ(StringEquals(Queue.Front(), "Test!"), true);
}

TEST_F(QueueTests, Front)
{
	EXPECT_EQ(StringEquals(Queue.Front(), "Hello World!"), true);
}

TEST_F(QueueTests, IsEmpty)
{
	EXPECT_EQ(Queue.IsEmpty(), false);
}

TEST_F(QueueTests, Size)
{
	EXPECT_EQ(Queue.Size(), 3);
}

TEST_F(QueueTests, Clear)
{
	Queue.Clear();
	EXPECT_EQ(Queue.Size(), 0);
	EXPECT_EQ(Queue.IsEmpty(), true);
}

