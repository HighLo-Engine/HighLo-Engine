#include <HighLo.h>

#include "Tests.h"

int main(int argc, char *argv[])
{
#ifdef HL_RELEASE
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_encryption, "test_encryption" },
		{ test_encryption_with_base64, "test_encryption_with_base64" }
	};
	
	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
#else
	// Debug builds are not supported
	return 1;
#endif // HL_RELEASE
}

