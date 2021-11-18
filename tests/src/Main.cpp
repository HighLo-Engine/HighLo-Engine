// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <HighLo.h>
#include <gtest/gtest.h>

#include "tests/StringTests.h"
#include "tests/EncryptionTests.h"

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

