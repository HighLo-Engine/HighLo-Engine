#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_put_to_hashmap, "test_put_to_hashmap" },
		{ test_set_to_hashmap, "test_set_to_hashmap" },
		{ test_find_in_hashmap, "test_find_in_hashmap" },
		{ test_has_key_in_hashmap, "test_has_key_in_hashmap" },
		{ test_has_value_in_hashmap, "test_has_value_in_hashmap" },

		{ test_get_first_from_hashmap, "test_get_first_from_hashmap" },
		{ test_get_last_from_hashmap, "test_get_last_from_hashmap" },
		{ test_get_any_from_hashmap, "test_get_any_from_hashmap" },
		{ test_get_key_from_hashmap, "test_get_key_from_hashmap" },
		{ test_get_by_key_index_operator_from_hashmap, "test_get_by_key_index_operator_from_hashmap" },
		{ test_get_by_index_operator_from_hashmap, "test_get_by_index_operator_from_hashmap" },
		
		{ test_is_empty_hashmap, "test_is_empty_hashmap" },
		{ test_get_size_from_hashmap, "test_get_size_from_hashmap" },

		{ test_remove_all_from_hashmap, "test_remove_all_from_hashmap" },
		{ test_remove_first_from_hashmap, "test_remove_first_from_hashmap" },
		{ test_remove_last_from_hashmap, "test_remove_last_from_hashmap" },
		{ test_remove_by_full_pair_from_hashmap, "test_remove_by_full_pair_from_hashmap" },
		{ test_remove_by_key_from_hashmap, "test_remove_by_key_from_hashmap" },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}