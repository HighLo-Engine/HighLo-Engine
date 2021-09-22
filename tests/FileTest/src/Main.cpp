#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> tests =
	{
		{ test_get_file_list, "test_get_file_list" },
		{ test_get_file_list_count, "test_get_file_list_count" },

		{ test_set_full_path, "test_set_full_path" },
		{ test_get_relative_path, "test_get_relative_path" },
		{ test_get_absolute_path, "test_get_absolute_path" },
		{ test_get_size, "test_get_size" },

		{ test_get_name, "test_get_name" },
		{ test_get_static_name, "test_get_static_name" },
		{ test_get_extension, "test_get_extension" },
		{ test_get_static_extension, "test_get_static_extension" },

		{ test_is_file, "test_is_file" },
		{ test_is_directory, "test_is_directory" },
	};

	highloUnit::UnitTest test;
	test.AppendAllTests(tests);
	return test.ExecuteTests();
}