#include "Tests.h"

using namespace highlo;

bool test_append_string_to_string()
{
	highloUnit::Timer timer("test_append_string_to_string");

	HLString str = "Hello";
	HLString str2 = " World!";
	HLString result = str + str2;
	HLString expected = "Hello World!";

	highloUnit::Test test;
	return test.AssertEqual(timer, *expected, *result);
}

bool test_append_char_to_string()
{
	highloUnit::Timer timer("test_append_char_to_string");

	HLString str = "Hello World";
	char c = '!';
	HLString expected = "Hello World!";
	HLString result = str + c;

	highloUnit::Test test;
	return test.AssertEqual(timer, *expected, *result);
}

bool test_remove_string_from_string()
{
	highloUnit::Timer timer("test_remove_string_from_string");

	HLString str = "Hello World!";
	str -= " World!";
	HLString expected = "Hello";

	highloUnit::Test test;
	return test.AssertEqual(timer, *expected, *str);
}

bool test_remove_char_from_string()
{
	highloUnit::Timer timer("test_remove_char_from_string");
	
	HLString str = "Hello World!";
	char c = '!';
	HLString expected = "Hello World";
	HLString result = str - c;

	highloUnit::Test test;
	return test.AssertEqual(timer, *expected, *result);
}

bool test_at_function()
{
	highloUnit::Timer timer("test_at_function");

	HLString str = "Hello World!";
	char expected = 'e';
	char result = str.At(1);

	highloUnit::Test test;
	return test.AssertEqual(timer, expected, result);
}

bool test_array_index_operator()
{
	highloUnit::Timer timer("test_array_index_operator");
	HLString str = "Hello World!";
	char expected = 'e';
	char result = str[1];

	highloUnit::Test test;
	return test.AssertEqual(timer, expected, result);
}

bool test_dereferencing_operator()
{
	highloUnit::Timer timer("test_dereferencing_operator");

	HLString str = "Hello World!";
	char *expected = "Hello World!";
	char *result = *str;

	highloUnit::Test test;
	return test.AssertEqual(timer, expected, result);
}

bool test_string_contains_letter()
{
	highloUnit::Timer timer("test_string_contains_letter");

	HLString str = "Hello World!";
	bool result = str.Contains('e');

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

bool test_string_contains_string()
{
	highloUnit::Timer timer("test_string_contains_string");

	HLString str = "Hello World!";
	bool result = str.Contains("Hello");

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

bool test_string_contains_not_letter()
{
	highloUnit::Timer timer("test_string_contains_not_letter");

	HLString str = "Hello World!";
	bool result = str.Contains('f');

	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_string_contains_not_string()
{
	highloUnit::Timer timer("test_string_contains_not_string");

	HLString str = "Hello World!";
	bool result = str.Contains("Car");
	
	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_convert_string_to_cstr()
{
	highloUnit::Timer timer("test_convert_string_to_cstr");

	HLString str = "Hello World!";
	char *result = str.C_Str();
	char *expected = "Hello World!";

	highloUnit::Test test;
	return test.AssertEqual(timer, result, expected);
}

bool test_convert_string_to_wstr()
{
	highloUnit::Timer timer("test_convert_string_to_wstr");

	HLString str = "Hello World!";
	wchar_t *expected = L"Hello World!";
	wchar_t *result = str.W_Str();

	highloUnit::Test test;
	return test.AssertEqual(timer, expected, result);
}

bool test_string_starts_with_letter()
{
	highloUnit::Timer timer("test_string_starts_with_letter");

	HLString str = "Hello World!";
	bool result = str.StartsWith('H');

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

bool test_string_starts_not_with_letter()
{
	highloUnit::Timer timer("test_string_starts_not_with_letter");

	HLString str = "Hello World!";
	bool result = str.StartsWith('e');

	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_string_starts_with_string()
{
	highloUnit::Timer timer("test_string_starts_with_string");

	HLString str = "Hello World!";
	bool result = str.StartsWith("Hello");

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

bool test_string_starts_not_with_string()
{
	highloUnit::Timer timer("test_string_starts_not_with_string");

	HLString str = "Hello World!";
	bool result = str.StartsWith("World");

	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_string_ends_with_letter()
{
	highloUnit::Timer timer("test_string_ends_with_letter");

	HLString str = "Hello World!";
	bool result = str.EndsWith('!');

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

bool test_string_ends_not_with_letter()
{
	highloUnit::Timer timer("test_string_ends_not_with_letter");

	HLString str = "Hello World!";
	bool result = str.EndsWith('d');

	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_string_ends_with_string()
{
	highloUnit::Timer timer("test_string_ends_with_string");

	HLString str = "Hello World!";
	bool result = str.EndsWith("World!");

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

bool test_string_ends_not_with_string()
{
	highloUnit::Timer timer("test_string_ends_not_with_string");

	HLString str = "Hello World!";
	bool result = str.EndsWith("Hello");

	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_first_index_not_of_letter()
{
	highloUnit::Timer timer("test_first_index_not_of_letter");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf('H');

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 1);
}

bool test_first_index_not_of_string()
{
	highloUnit::Timer timer("test_first_index_not_of_string");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf("World!");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 0);
}

bool test_first_index_not_of_not_letter()
{
	highloUnit::Timer timer("test_first_index_not_of_not_letter");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf('H');

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 0);
}

bool test_first_index_not_of_not_string()
{
	highloUnit::Timer timer("test_first_index_not_of_not_string");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexNotOf("World!");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 0);
}

bool test_first_index_of_letter()
{
	highloUnit::Timer timer("test_first_index_of_letter");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf('H');

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 0);
}

bool test_first_index_of_string()
{
	highloUnit::Timer timer("test_first_index_of_string");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf("Hello");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 0);
}

bool test_first_index_of_not_letter()
{
	highloUnit::Timer timer("test_first_index_of_not_letter");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf('E');

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 0);
}

bool test_first_index_of_not_string()
{
	highloUnit::Timer timer("test_first_index_of_not_string");

	HLString str = "Hello World!";
	uint32 result = str.FirstIndexOf("World!");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 0);
}

bool test_index_of_letter()
{
	highloUnit::Timer timer("test_index_of_letter");

	HLString str = "Hello World!";
	uint32 result = str.IndexOf(' ');

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 5);
}

bool test_index_of_string()
{
	highloUnit::Timer timer("test_index_of_string");

	HLString str = "Hello World!";
	uint32 result = str.IndexOf("World!");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 6);
}

bool test_index_of_not_letter()
{
	highloUnit::Timer timer("test_index_of_not_letter");

	HLString str = "Hello World!";
	uint32 result = str.IndexOf('!');

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 0);
}

bool test_index_of_not_string()
{
	highloUnit::Timer timer("test_index_of_not_string");

	HLString str = "Hello World!";
	uint32 result = str.IndexOf("Wo");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 0);
}

bool test_last_index_of_letter()
{
	highloUnit::Timer timer("test_last_index_of_letter");

	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf('o');

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 7);
}

bool test_last_index_of_not_letter()
{
	highloUnit::Timer timer("test_last_index_of_not_letter");

	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf('H');

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 11);
}

bool test_last_index_of_string()
{
	highloUnit::Timer timer("test_last_index_of_string");

	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf("World!");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 6);
}

bool test_last_index_of_not_string()
{
	highloUnit::Timer timer("test_last_index_of_not_string");

	HLString str = "Hello World!";
	uint32 result = str.LastIndexOf("Hello");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, result, 6);
}

bool test_invert_string()
{
	highloUnit::Timer timer("test_invert_string");

	HLString str = "Hello World!";
	HLString result = str.Reverse();
	HLString expected = "!dlroW olleH";

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_length_of_string()
{
	highloUnit::Timer timer("test_length_of_string");

	HLString str = "Hello World!";
	uint32 expected = 12;
	uint32 result = str.Length();

	highloUnit::Test test;
	return test.AssertEqual(timer, result, expected);
}

bool test_print_string()
{
	highloUnit::Timer timer("test_print_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_resize_string()
{
	highloUnit::Timer timer("test_resize_string");

	HLString str;
	str.Resize(32);
	uint32 result = str.Length();
	uint32 expected = 32;

	highloUnit::Test test;
	return test.AssertEqual(timer, result, expected);
}

bool test_empty_string()
{
	highloUnit::Timer timer("test_empty_string");

	HLString emptyStr = "";

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, emptyStr.Length(), 0);
}

bool test_not_empty_string()
{
	highloUnit::Timer timer("test_not_empty_string");

	HLString str = "Hello World!";

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, str.Length(), 0);
}

bool test_lower_case_string()
{
	highloUnit::Timer timer("test_lower_case_string");

	HLString str = "HELLO WORLD!";
	HLString expected = "hello world!";
	HLString result = str.ToLowerCase();

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_upper_case_string()
{
	highloUnit::Timer timer("test_upper_case_string");

	HLString str = "hello world!";
	HLString expected = "HELLO WORLD!";
	HLString result = str.ToUpperCase();

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_replace_string()
{
	highloUnit::Timer timer("test_replace_string");

	HLString str = "hello world!";
	HLString expected = "HELLO world!";
	HLString result = str.Replace("hello", "HELLO");

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_replace_all_strings()
{
	highloUnit::Timer timer("test_replace_all_strings");

	HLString str = "hello world! hello world! hello world!";
	HLString expected = "HELLO world! HELLO world! HELLO world!";
	HLString result = str.Replace("hello", "HELLO", 3);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_split_string_vector_with_space_delimiter()
{
	highloUnit::Timer timer("test_split_string_vector_with_space_delimiter");

	HLString str = "hello world! hello world! hello world!";
	std::vector<HLString> result = str.Split(' ');
	std::vector<char*> realResult;

	for (uint32 i = 0; i < result.size(); ++i)
	{
		realResult.push_back(*result[i]);
	}

	std::vector<char*> expected =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	highloUnit::Test test;
	return test.AssertEqual(timer, realResult, expected);
}

bool test_split_string_array_with_space_delimiter()
{
	highloUnit::Timer timer("test_split_string_array_with_space_delimiter");

	HLString str = "hello world! hello world! hello world!";

	uint32 wordCount;
	HLString *result = str.Split(' ', &wordCount);

	HLString expected[] =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	highloUnit::Test test;
	return test.AssertEqual(timer, *result[0], *expected[0])
		&& test.AssertEqual(timer, *result[1], *expected[1])
		&& test.AssertEqual(timer, *result[2], *expected[2])
		&& test.AssertEqual(timer, *result[3], *expected[3])
		&& test.AssertEqual(timer, *result[4], *expected[4])
		&& test.AssertEqual(timer, *result[5], *expected[5]);
}

bool test_split_string_vector_with_comma_delimiter()
{
	highloUnit::Timer timer("test_split_string_vector_with_comma_delimiter");

	HLString str = "hello,world!,hello,world!,hello,world!";
	std::vector<HLString> result = str.Split(',');
	std::vector<char*> realResult;

	for (uint32 i = 0; i < result.size(); ++i)
	{
		realResult.push_back(*result[i]);
	}

	std::vector<char*> expected =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	highloUnit::Test test;
	return test.AssertEqual(timer, realResult, expected);
}

bool test_split_string_array_with_comma_delimiter()
{
	highloUnit::Timer timer("test_split_string_array_with_comma_delimiter");

	HLString str = "hello,world!,hello,world!,hello,world!";

	uint32 wordCount;
	HLString *result = str.Split(',', &wordCount);

	HLString expected[] =
	{
		"hello",
		"world!",
		"hello",
		"world!",
		"hello",
		"world!"
	};

	highloUnit::Test test;
	return test.AssertEqual(timer, *result[0], *expected[0])
		&& test.AssertEqual(timer, *result[1], *expected[1])
		&& test.AssertEqual(timer, *result[2], *expected[2])
		&& test.AssertEqual(timer, *result[3], *expected[3])
		&& test.AssertEqual(timer, *result[4], *expected[4])
		&& test.AssertEqual(timer, *result[5], *expected[5]);
}

bool test_substring_first_5()
{
	highloUnit::Timer timer("test_substring_first_5");

	HLString str = "hello world! hello world! hello world!";
	HLString expected = "hello";
	HLString result = str.Substr(0, 5);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_substring_first_10()
{
	highloUnit::Timer timer("test_substring_first_10");

	HLString str = "hello world! hello world! hello world!";
	HLString expected = "hello worl";
	HLString result = str.Substr(0, 10);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_substring_last_5()
{
	highloUnit::Timer timer("test_substring_last_5");

	HLString str = "hello world! hello world! hello world!";
	HLString expected = "orld!";
	HLString result = str.Substr(str.Length() - 5);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_substring_last_10()
{
	highloUnit::Timer timer("test_substring_last_10");

	HLString str = "hello world! hello world! hello world!";
	HLString expected = "llo world!";
	HLString result = str.Substr(str.Length() - 10);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_integer_to_string()
{
	highloUnit::Timer timer("test_integer_to_string");

	int32 value = 42;
	HLString expected = "42";
	HLString result = HLString::ToString(value);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_float_to_string()
{
	highloUnit::Timer timer("test_float_to_string");

	float value = 42.4f;
	HLString expected = "42.4";
	HLString result = HLString::ToString(value);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_double_to_string()
{
	highloUnit::Timer timer("test_double_to_string");

	double value = 42.4;
	HLString expected = "42.4";
	HLString result = HLString::ToString(value);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_bool_to_string()
{
	highloUnit::Timer timer("test_bool_to_string");

	bool value = true;
	HLString expected = "1";
	HLString result = HLString::ToString(value);

	highloUnit::Test test;
	return test.AssertEqual(timer, *result, *expected);
}

bool test_vector2_to_string()
{
	highloUnit::Timer timer("test_vector2_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_vector3_to_string()
{
	highloUnit::Timer timer("test_vector3_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_vector4_to_string()
{
	highloUnit::Timer timer("test_vector4_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_mat2_to_string()
{
	highloUnit::Timer timer("test_mat2_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_mat3_to_string()
{
	highloUnit::Timer timer("test_mat3_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_mat4_to_string()
{
	highloUnit::Timer timer("test_mat4_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_transform_to_string()
{
	highloUnit::Timer timer("test_transform_to_string");

	// TODO
	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_hash_string()
{
	highloUnit::Timer timer("test_hash_string");

	HLString str = "Hello World!";

	highloUnit::Test test;
	return test.AssertEqual<uint64>(timer, 2005729231029082450, str.Hash());
}

bool test_hash_multiple_string()
{
	highloUnit::Timer timer("test_hash_multiple_string");

	HLString str = "Hello World!";
	HLString str1 = "Hello World!";

	highloUnit::Test test;
	return test.AssertEqual<uint64>(timer, str.Hash(), str1.Hash());
}

bool test_hash_different_multiple_string()
{
	highloUnit::Timer timer("test_hash_different_multiple_string");

	HLString str = "Hello World!";
	HLString str1 = "abc";

	highloUnit::Test test;
	return test.AssertNotEqual<uint64>(timer, str.Hash(), str1.Hash());
}

