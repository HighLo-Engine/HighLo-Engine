#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_push_back_to_vector, "test_push_back_to_vector", true },
		{ test_emplace_back_to_vector, "test_emplace_back_to_vector", true },
		{ test_pop_back_from_vector, "test_pop_back_from_vector", true },

		{ test_clear_vector, "test_clear_vector", true },
		{ test_find_in_vector, "test_find_in_vector", true },
		{ test_get_size_of_vector, "test_get_size_of_vector", true },
		{ test_get_capacity_of_vector, "test_get_capacity_of_vector", true },

		{ test_get_first_value_of_vector, "test_get_first_value_of_vector", true },
		{ test_get_last_value_of_vector, "test_get_last_value_of_vector", true },
		{ test_get_by_array_index_operator_of_vector, "test_get_by_array_index_operator_of_vector", true },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}