// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <HighLo.h>
#include <gtest/gtest.h>

#include "tests/StringTests.h"
#include "tests/EncryptionTests.h"
#include "tests/HashmapTests.h"
#include "tests/ECSTests.h"
#include "tests/ListTests.h"
#include "tests/StackTests.h"
#include "tests/QueueTests.h"
#include "tests/VectorTests.h"
#include "tests/BinaryTreeTests.h"
#include "tests/BinarySearchTreeTests.h"
#include "tests/FileSystemTests.h"
#include "tests/FileSystemPathTests.h"

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

