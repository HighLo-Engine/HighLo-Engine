#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_add_component, "test_add_component" },
		{ for_each_component, "for_each_component" }
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}

