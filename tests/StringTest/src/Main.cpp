#include <HighLo.h>
#include "Tests.h"

int main(int argc, char *argv[])
{
#ifdef HL_RELEASE
	std::vector<highloUnit::UnitTestEntry> funcs = 
	{
		{ test_append_string_to_string, "test_append_string_to_string" },
		{ test_append_char_to_string, "test_append_char_to_string" },

		{ test_remove_string_from_string, "test_remove_string_from_string" },
		{ test_remove_char_from_string, "test_remove_char_from_string" },

		{ test_at_function, "test_at_function" },
		{ test_array_index_operator, "test_array_index_operator" },
		{ test_dereferencing_operator, "test_dereferencing_operator" },

		{ test_string_contains_letter, "test_string_contains_letter" },
		{ test_string_contains_string, "test_string_contains_string" },
		{ test_string_contains_not_letter, "test_string_contains_not_letter" },
		{ test_string_contains_not_string, "test_string_contains_not_string" },
		
		{ test_convert_string_to_cstr, "test_convert_string_to_cstr" },
		{ test_convert_string_to_wstr, "test_convert_string_to_wstr" },

		{ test_string_starts_with_letter, "test_string_starts_with_letter" },
		{ test_string_starts_not_with_letter, "test_string_starts_not_with_letter" },
		{ test_string_starts_with_string, "test_string_starts_with_string" },
		{ test_string_starts_not_with_string, "test_string_starts_not_with_string" },

		{ test_string_ends_with_letter, "test_string_ends_with_letter" },
		{ test_string_ends_not_with_letter, "test_string_ends_not_with_letter" },
		{ test_string_ends_with_string, "test_string_ends_with_string" },
		{ test_string_ends_not_with_string, "test_string_ends_not_with_string" },

		{ test_first_index_not_of_letter, "test_first_index_not_of_letter" },
		{ test_first_index_not_of_string, "test_first_index_not_of_string" },
		{ test_first_index_not_of_not_letter, "test_first_index_not_of_not_letter" },
		{ test_first_index_not_of_not_string, "test_first_index_not_of_not_string" },

		{ test_first_index_of_letter, "test_first_index_of_letter" },
		{ test_first_index_of_string, "test_first_index_of_string" },
		{ test_first_index_of_not_letter, "test_first_index_of_not_letter" },
		{ test_first_index_of_not_string, "test_first_index_of_not_string" },

		{ test_index_of_letter, "test_index_of_letter" },
		{ test_index_of_string, "test_index_of_string" },
		{ test_index_of_not_letter, "test_index_of_not_letter" },
		{ test_index_of_not_string, "test_index_of_not_string" },
	
		{ test_last_index_of_letter, "test_last_index_of_letter" },
		{ test_last_index_of_not_letter, "test_last_index_of_not_letter" },
		{ test_last_index_of_string, "test_last_index_of_string" },
		{ test_last_index_of_not_string, "test_last_index_of_not_string" },

		{ test_invert_string, "test_invert_string" },
		{ test_length_of_string, "test_length_of_string" },
		{ test_print_string, "test_print_string" },
		{ test_resize_string, "test_resize_string" },
		{ test_empty_string, "test_empty_string" },
		{ test_not_empty_string, "test_not_empty_string" },

		{ test_lower_case_string, "test_lower_case_string" },
		{ test_upper_case_string, "test_upper_case_string" },
		{ test_replace_string, "test_replace_string" },
		{ test_replace_all_strings, "test_replace_all_strings" },

		{ test_split_string_vector_with_space_delimiter, "test_split_string_vector_with_space_delimiter" },
		{ test_split_string_array_with_space_delimiter, "test_split_string_array_with_space_delimiter" },
		{ test_split_string_vector_with_comma_delimiter, "test_split_string_vector_with_comma_delimiter" },
		{ test_split_string_array_with_comma_delimiter, "test_split_string_array_with_comma_delimiter" },

		{ test_substring_first_5, "test_substring_first_5" },
		{ test_substring_first_10, "test_substring_first_10" },
		{ test_substring_last_5, "test_substring_last_5" },
		{ test_substring_last_10, "test_substring_last_10" },

		{ test_integer_to_string, "test_integer_to_string" },
		{ test_float_to_string, "test_float_to_string" },
		{ test_double_to_string, "test_double_to_string" },
		{ test_bool_to_string, "test_bool_to_string" },
		{ test_vector2_to_string, "test_vector2_to_string" },
		{ test_vector3_to_string, "test_vector3_to_string" },
		{ test_vector4_to_string, "test_vector4_to_string" },
		{ test_mat2_to_string, "test_mat2_to_string" },
		{ test_mat3_to_string, "test_mat3_to_string" },
		{ test_mat4_to_string, "test_mat4_to_string" },
		{ test_transform_to_string, "test_transform_to_string" },

		{ test_empty_string, "test_empty_string" },
	};

	highloUnit::UnitTest unitTest;
	unitTest.AppendAllTests(funcs);
	return unitTest.ExecuteTests();
#else
	// Debug builds are not supported
	return 1;
#endif // HL_RELEASE
}

