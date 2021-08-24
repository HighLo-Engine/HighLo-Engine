#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_push_to_stack, "test_push_to_stack" },
		{ test_pop_from_stack, "test_pop_from_stack" },
		{ test_top_stack, "test_top_stack" },

		{ test_is_empty_stack, "test_is_empty_stack" },
		{ test_size_of_stack, "test_size_of_stack" },
		{ test_clear_stack, "test_clear_stack" }
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}

