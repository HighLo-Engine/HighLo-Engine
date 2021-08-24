#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_enqueue_queue, "test_enqueue_queue" },
		{ test_dequeue_queue, "test_dequeue_queue" },
		{ test_front_queue, "test_front_queue" },

		{ test_is_empty_queue, "test_is_empty_queue" },
		{ test_get_size_of_queue, "test_get_size_of_queue" },
		{ test_clear_queue, "test_clear_queue" }
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}

