#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_add_component, "test_add_component" },
		{ test_add_component_with_value_change, "test_add_component_with_value_change" },
		{ test_for_each_multiple_component, "test_for_each_multiple_component" },
		{ test_for_each_multiple_component_with_value_change, "test_for_each_multiple_component_with_value_change" }
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}

