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
	bool unitTestResult = test.AssertEqual(timer, *expected, *result);
	if (unitTestResult)
	{
		std::cout << "Test test_append_to_string succeeded!" << std::endl;
	}
	else
	{
		std::cout << "Test test_append_to_string failed!" << std::endl;
	}

	return unitTestResult;
}

bool test_remove_from_string()
{
	highloUnit::Timer timer("test_remove_from_string");
	
	HLString str = "Hello World!";
	str -= " World!";
	HLString expected = "Hello";

	highloUnit::UnitTest test;
	bool unitTestResult = test.AssertEqual(timer, *expected, *str);
	if (unitTestResult)
	{
		std::cout << "Test test_remove_from_string succeeded!" << std::endl;
	}
	else
	{
		std::cout << "Test test_remove_from_string failed!" << std::endl;
	}

	return unitTestResult;
}

bool test_empty_string()
{
	highloUnit::Timer timer("test_empty_string");

	HLString emptyStr = "";

	highloUnit::UnitTest test;
	bool unitTestResult = test.AssertEqual<uint32>(timer, emptyStr.Length(), 0);
	if (unitTestResult)
	{
		std::cout << "Test test_empty_string succeeded!" << std::endl;
	}
	else
	{
		std::cout << "Test test_empty_string failed!" << std::endl;
	}

	return unitTestResult;
}

int main(int argc, char *argv[])
{
	std::vector<highloUnit::UnitTestEntry> funcs = 
	{
		{ test_append_to_string, true, "test_append_to_string" },
		{ test_remove_from_string, true, "test_remove_from_string" },
		{ test_empty_string, true, "test_empty_string" },
	};

	highloUnit::UnitTest unitTest;
	unitTest.AppendAllTests(funcs);
	return unitTest.ExecuteTests();
}

