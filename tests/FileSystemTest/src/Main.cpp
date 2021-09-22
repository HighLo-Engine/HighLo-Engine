#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_file_exists, "test_file_exists" },
		{ test_path_exists, "test_path_exists" },

		{ test_remove_file, "test_remove_file" },
		{ test_rename_file, "test_rename_file" },
		{ test_move_file, "test_move_file" },
		{ test_get_file_size, "test_get_file_size" },

		{ test_create_folder, "test_create_folder" },
		{ test_remove_folder, "test_remove_folder" },

		{ test_read_file, "test_read_file" },
		{ test_read_text_file, "test_read_text_file" },

		{ test_write_file, "test_write_file" },
		{ test_write_text_file, "test_write_text_file" },

		{ test_add_env_var, "test_add_env_var" },
		{ test_has_env_var, "test_has_env_var" },
		{ test_get_env_var, "test_get_env_var" },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}