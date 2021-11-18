#include <HighLo.h>
#include <gtest/gtest.h>

#include "EncryptionTests.h"
#include "StringTests.h"

TEST(DemoTest, DemoTest)
{
	EXPECT_EQ(4, 2 + 2);
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

