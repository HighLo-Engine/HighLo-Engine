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

struct BinarySearchTreeTests : public testing::Test
{
	BinarySearchTree<HLString> BinarySearchTree;

	BinarySearchTreeTests()
	{
	}

	virtual ~BinarySearchTreeTests()
	{
	}
};

TEST_F(BinarySearchTreeTests, AppendNode)
{

}

TEST_F(BinarySearchTreeTests, RemoveNode)
{

}

TEST_F(BinarySearchTreeTests, RemoveAll)
{

}

TEST_F(BinarySearchTreeTests, FindMin)
{

}

TEST_F(BinarySearchTreeTests, FindMinNode)
{

}

TEST_F(BinarySearchTreeTests, FindMax)
{

}

TEST_F(BinarySearchTreeTests, FindMaxNode)
{

}

TEST_F(BinarySearchTreeTests, Invert)
{

}

TEST_F(BinarySearchTreeTests, GetSize)
{

}

TEST_F(BinarySearchTreeTests, GetHeight)
{

}

TEST_F(BinarySearchTreeTests, IsEmpty)
{

}

TEST_F(BinarySearchTreeTests, LevelOrder)
{

}

TEST_F(BinarySearchTreeTests, PreOrder)
{

}

TEST_F(BinarySearchTreeTests, InOrder)
{

}

TEST_F(BinarySearchTreeTests, PostOrder)
{

}
