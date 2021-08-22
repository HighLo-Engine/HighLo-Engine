#include "Tests.h"
using namespace highlo;

bool test_put_to_hashmap()
{
	highloUnit::Timer timer("test_put_to_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, hashmap.Size(), 0)
		&& test.AssertEqual<char*>(timer, *hashmap[42], "Hello World!");
}

bool test_set_to_hashmap()
{
	highloUnit::Timer timer("test_set_to_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Set(42, "Test");

	highloUnit::Test test;
	return test.AssertNotEqual<uint32>(timer, hashmap.Size(), 0)
		&& test.AssertEqual<char*>(timer, *hashmap[42], "Test");
}

bool test_find_in_hashmap()
{
	highloUnit::Timer timer("test_find_in_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	HLString result = hashmap.Find(52);

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Test");
}

bool test_has_key_in_hashmap()
{
	highloUnit::Timer timer("test_has_key_in_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");
	
	bool result = hashmap.HasKey(102);
	bool wrongResult = hashmap.HasKey(1);

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, result, true)
		&& test.AssertEqual<bool>(timer, wrongResult, false);
}

bool test_has_value_in_hashmap()
{
	highloUnit::Timer timer("test_has_value_in_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	bool result = hashmap.HasValue("Test");
	bool wrongResult = hashmap.HasValue("LOL");

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, result, true)
		&& test.AssertEqual<bool>(timer, wrongResult, false);
}

bool test_get_first_from_hashmap()
{
	highloUnit::Timer timer("test_get_first_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap.GetFirst(), "Hello World!");
}

bool test_get_last_from_hashmap()
{
	highloUnit::Timer timer("test_get_last_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap.GetLast(), "Another Test");
}

bool test_get_any_from_hashmap()
{
	highloUnit::Timer timer("test_get_any_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap.GetAt(1), "Test");
}

bool test_get_key_from_hashmap()
{
	highloUnit::Timer timer("test_get_key_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<int32>(timer, hashmap.GetKey(1), 52);
}

bool test_get_by_key_index_operator_from_hashmap()
{
	highloUnit::Timer timer("test_get_by_key_index_operator_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap[52], "Test");
}

bool test_get_by_index_operator_from_hashmap()
{
	highloUnit::Timer timer("test_get_by_index_operator_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap[(size_t)2], "Another Test"); // we have to cast this because the keyType is an int as well
}

bool test_is_empty_hashmap()
{
	highloUnit::Timer timer("test_is_empty_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, hashmap.IsEmpty(), false);
}

bool test_get_size_from_hashmap()
{
	highloUnit::Timer timer("test_get_size_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, hashmap.Size(), 3);
}

bool test_remove_all_from_hashmap()
{
	highloUnit::Timer timer("test_remove_all_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");
	hashmap.RemoveAll();

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, hashmap.Size(), 0)
		&& test.AssertEqual<bool>(timer, hashmap.IsEmpty(), true);
}

bool test_remove_first_from_hashmap()
{
	highloUnit::Timer timer("test_remove_first_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");
	hashmap.RemoveFirst();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap.GetFirst(), "Test");
}

bool test_remove_last_from_hashmap()
{
	highloUnit::Timer timer("test_remove_last_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");
	hashmap.RemoveLast();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *hashmap.GetLast(), "Test");
}

bool test_remove_by_full_pair_from_hashmap()
{
	highloUnit::Timer timer("test_remove_by_full_pair_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");
	hashmap.Remove(52, "Test");

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, hashmap.HasKey(52), false);
}

bool test_remove_by_key_from_hashmap()
{
	highloUnit::Timer timer("test_remove_by_key_from_hashmap");

	HLHashmap<int32, HLString> hashmap;
	hashmap.Put(42, "Hello World!");
	hashmap.Put(52, "Test");
	hashmap.Put(102, "Another Test");
	hashmap.Remove(52);

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, hashmap.HasKey(52), false);
}

