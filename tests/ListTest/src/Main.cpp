#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_append_to_list, "test_append_to_list" },
		{ test_insert_to_list, "test_insert_to_list" },

		{ test_remove_first_element_from_list, "test_remove_first_element_from_list" },
		{ test_remove_last_element_from_list, "test_remove_last_element_from_list" },
		{ test_remove_any_element_from_list, "test_remove_any_element_from_list" },

		{ test_get_first_element, "test_get_first_element" },
		{ test_get_last_element, "test_get_last_element" },
		{ test_get_any_element, "test_get_any_element" },

		{ test_is_empty_list, "test_is_empty_list" },
		{ test_get_size_from_list, "test_get_size_from_list" },

		{ test_clear_list, "test_clear_list" },
		{ test_convert_to_array, "test_convert_to_array" },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}

