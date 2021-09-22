#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_assign_new_path, "test_assign_new_path" },
		{ test_swap_paths, "test_swap_paths" },
		{ test_hash_path, "test_hash_path" },

		{ test_folder_is_empty, "test_folder_is_empty" },
		{ test_has_root_path, "test_has_root_path" },
		{ test_has_parent_path, "test_has_parent_path" },

		{ test_is_absolute_path, "test_is_absolute_path" },
		{ test_get_relative_path, "test_get_relative_path" },

		{ test_is_relative_path, "test_is_relative_path" },
		{ test_get_parent_path, "test_get_parent_path" },

		{ test_get_file, "test_get_file" },
		{ test_get_path_as_str, "test_get_path_as_str" },

		{ test_append_path_to_path_1, "test_append_path_to_path_1" },
		{ test_append_path_to_path_2, "test_append_path_to_path_2" },
		{ test_append_path_to_path_3, "test_append_path_to_path_3" },

		{ test_append_str_to_path_1, "test_append_str_to_path_1" },
		{ test_append_str_to_path_2, "test_append_str_to_path_2" },
		{ test_append_str_to_path_3, "test_append_str_to_path_3" },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}