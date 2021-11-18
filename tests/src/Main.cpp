#include <HighLo.h>
#include <gtest/gtest.h>

#include "tests/StringTests.h"
#include "tests/EncryptionTests.h"

TEST(DemoTest, DemoTest)
{
	EXPECT_EQ(4, 2 + 2);
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

