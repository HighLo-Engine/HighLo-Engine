#include "Tests.h"
using namespace highlo;

bool test_push_back_to_vector()
{
	highloUnit::Timer timer("test_push_back_to_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	HLString result = vector[0];

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Hello World!");
}

bool test_emplace_back_to_vector()
{
	highloUnit::Timer timer("test_emplace_back_to_vector");

	HLVector<HLString> vector;
	vector.EmplaceBack("Hello World!");
	HLString result = vector[0];

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Hello World!");
}

bool test_pop_back_from_vector()
{
	highloUnit::Timer timer("test_pop_back_from_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	vector.PushBack("lol!");
	vector.PushBack("test");
	vector.PopBack();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, vector[vector.Size() - 1], "lol!");
}

bool test_clear_vector()
{
	highloUnit::Timer timer("test_clear_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.Clear();

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, vector.IsEmpty(), true);
}

bool test_find_in_vector()
{
	highloUnit::Timer timer("test_find_in_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	HLString result = *vector.Find("Test!");
	
	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Test!");
}

bool test_get_size_of_vector()
{
	highloUnit::Timer timer("test_get_size_of_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	uint32 result = vector.Size();

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 3);
}

bool test_get_capacity_of_vector()
{
	highloUnit::Timer timer("test_get_capacity_of_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	uint32 result = vector.Capacity();

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, result, 3);
}

bool test_remove_from_vector()
{
	highloUnit::Timer timer("test_remove_from_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	vector.Remove(2);

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *vector.Back(), "Test!");
}

bool test_reserve_vector()
{
	highloUnit::Timer timer("test_reserve_vector");

	HLVector<HLString> vector;
	vector.Reserve(10);
	
	const uint32 size = vector.Size();
	const uint32 cap = vector.Capacity();

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, size, 0)
		&& test.AssertEqual<uint32>(timer, cap, 10);
}

bool test_resize_vector()
{
	highloUnit::Timer timer("test_resize_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.PushBack("Hello");
	vector.Resize(5);
	
	const uint32 size = vector.Size();
	const uint32 cap = vector.Capacity();

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, size, 5)
		&& test.AssertEqual<uint32>(timer, cap, 5);
}

bool test_get_first_value_of_vector()
{
	highloUnit::Timer timer("test_get_first_value_of_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	HLString result = vector.Front();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Hello World!");
}

bool test_get_last_value_of_vector()
{
	highloUnit::Timer timer("test_get_last_value_of_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	HLString result = vector.Back();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Another Test!");
}

bool test_get_by_array_index_operator_of_vector()
{
	highloUnit::Timer timer("test_get_by_array_index_operator_of_vector");

	HLVector<HLString> vector;
	vector.PushBack("Hello World!");
	vector.PushBack("Test!");
	vector.PushBack("Another Test!");
	HLString result = vector[1];

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Test!");
}

