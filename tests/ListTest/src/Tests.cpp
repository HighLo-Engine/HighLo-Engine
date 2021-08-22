#include "Tests.h"

using namespace highlo;

bool test_append_to_list()
{
	highloUnit::Timer timer("test_append_to_list");

	List<HLString> list;
	list.Append("Test");
	list.Append("First");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *list.GetFirst(), "Test");
}

bool test_insert_to_list()
{
	highloUnit::Timer timer("test_insert_to_list");

	List<HLString> list;
	list.Append("Test");
	list.Insert("First");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *list.GetFirst(), "First");
}

bool test_remove_first_element_from_list()
{
	highloUnit::Timer timer("test_remove_first_element_from_list");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.RemoveFirst();

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *list.GetFirst(), "Second");
}

bool test_remove_last_element_from_list()
{
	highloUnit::Timer timer("test_remove_last_element_from_list");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.RemoveLast();

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *list.GetLast(), "First");
}

bool test_remove_any_element_from_list()
{
	highloUnit::Timer timer("test_remove_any_element_from_list");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");
	list.Remove(1); // "Second" should be removed

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *list.Get(1), "Third");
}

bool test_get_first_element()
{
	highloUnit::Timer timer("test_get_first_element");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");
	HLString result = list.GetFirst();

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *result, "First");
}

bool test_get_last_element()
{
	highloUnit::Timer timer("test_get_last_element");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");
	HLString result = list.GetLast();

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *result, "Third");
}

bool test_get_any_element()
{
	highloUnit::Timer timer("test_get_any_element");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");
	HLString result = list.Get(1);

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, list.Size(), 0)
		&& test.AssertEqual<char*>(timer, *result, "Second");
}

bool test_is_empty_list()
{
	highloUnit::Timer timer("test_is_empty_list");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");
	bool result = list.IsEmpty();

	highloUnit::Test test;
	return test.AssertEqual(timer, result, false);
}

bool test_get_size_from_list()
{
	highloUnit::Timer timer("test_get_size_from_list");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, list.Size(), 3);
}

bool test_clear_list()
{
	highloUnit::Timer timer("test_clear_list");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");
	list.Clear();

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, list.Size(), 0);
}

bool test_convert_to_array()
{
	highloUnit::Timer timer("test_convert_to_array");

	List<HLString> list;
	list.Append("First");
	list.Append("Second");
	list.Append("Third");

	HLString *arr = list.ToArray();
	uint32 equalCount = 0;
	equalCount += arr[0] == "First";
	equalCount += arr[1] == "Second";
	equalCount += arr[2] == "Third";
	bool convertedCorrectly = equalCount == 3;
	delete[] arr;

	highloUnit::Test test;
	return test.AssertEqual(timer, convertedCorrectly, true);
}
