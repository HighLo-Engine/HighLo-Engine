#include "Tests.h"
using namespace highlo;

bool test_push_to_stack()
{
	highloUnit::Timer timer("test_push_to_stack");

	HLStack<HLString> stack;
	stack.Push("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *stack.Top(), "Hello World!");
}

bool test_pop_from_stack()
{
	highloUnit::Timer timer("test_pop_from_stack");

	HLStack<HLString> stack;
	stack.Push("Hello World!");
	stack.Push("Test!");
	stack.Pop();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *stack.Top(), "Hello World!");
}

bool test_top_stack()
{
	highloUnit::Timer timer("test_top_stack");

	HLStack<HLString> stack;
	stack.Push("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *stack.Top(), "Hello World!");
}

bool test_is_empty_stack()
{
	highloUnit::Timer timer("test_is_empty_stack");

	HLStack<HLString> stack;
	stack.Push("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, stack.IsEmpty(), false);
}

bool test_size_of_stack()
{
	highloUnit::Timer timer("test_size_of_stack");

	HLStack<HLString> stack;
	stack.Push("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, stack.Size(), 1);
}

bool test_clear_stack()
{
	highloUnit::Timer timer("test_clear_stack");

	HLStack<HLString> stack;
	stack.Push("Hello World!");
	stack.Push("Test!");
	stack.Clear();

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, stack.IsEmpty(), true);
}

