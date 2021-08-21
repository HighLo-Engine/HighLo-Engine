#include <HighLo.h>

#include "Tests.h"

int main(int argc, char *argv[])
{
#ifdef HL_RELEASE
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_encryption_raw, "test_encryption_raw" },
		{ test_encryption_with_string, "test_encryption_with_string" },
		{ test_encryption_with_base64_string, "test_encryption_with_base64_string" }
	};
	
	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
#else
	// Debug builds are not supported
	return 1;
#endif // HL_RELEASE
}

