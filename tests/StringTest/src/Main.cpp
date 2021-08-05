#include <HighLo.h>
using namespace highlo;

bool test_append_to_string()
{
	highloUnit::Timer timer("test_append_to_string");

	HLString str = "Hello";
	HLString str2 = " World!";
	HLString result = str + str2;
	HLString expected = "Hello World!";

	highloUnit::UnitTest test;
	return test.AssertEqual(timer, *expected, *result);
}

bool test_remove_from_string()
{
	highloUnit::Timer timer("test_remove_from_string");
	
	HLString str = "Hello World!";
	str -= " World!";
	HLString expected = "Hello";

	highloUnit::UnitTest test;
	return test.AssertEqual(timer, *expected, *str);
}

bool test_empty_string()
{
	highloUnit::Timer timer("test_empty_string");
	
	HLString emptyStr = "";

	highloUnit::UnitTest test;
	return test.AssertEqual<uint32>(timer, emptyStr.Length(), 0);
}

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> funcs = 
	{
		{ test_append_to_string, true, "test_append_to_string" },
		{ test_remove_from_string, false, "test_remove_from_string" },
		{ test_empty_string, true, "test_empty_string" },
	};

	highloUnit::UnitTest unitTest;
	unitTest.AppendAllTests(funcs);
	return unitTest.ExecuteTests();
}

