#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_append_to_tree, "test_append_to_tree" },
		{ test_remove_all_from_tree, "test_remove_all_from_tree" },
		{ test_remove_node_from_tree, "test_remove_node_from_tree" },
		{ test_get_node_from_tree, "test_get_node_from_tree" },

		{ test_find_min_value_in_tree, "test_find_min_value_in_tree" },
		{ test_find_max_value_in_tree, "test_find_max_value_in_tree" },
		{ test_find_min_node_in_tree, "test_find_min_node_in_tree" },
		{ test_find_max_node_in_tree, "test_find_max_node_in_tree" },
		{ test_value_exists_in_tree, "test_value_exists_in_tree" },

		{ test_invert_tree, "test_invert_tree" },
		{ test_is_empty_tree, "test_is_empty_tree" },
		{ test_get_size_of_tree, "test_get_size_of_tree" },
		{ test_get_height_of_tree, "test_get_height_of_tree" },

		{ test_level_order_tree, "test_level_order_tree" },
		{ test_pre_order_tree, "test_pre_order_tree" },
		{ test_in_order_tree, "test_in_order_tree" },
		{ test_post_order_tree, "test_post_order_tree" },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}