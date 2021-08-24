#include "Tests.h"
using namespace highlo;

bool test_enqueue_queue()
{
	highloUnit::Timer timer("test_enqueue_queue");

	HLQueue<HLString> queue;
	queue.Enqueue("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, queue.IsEmpty(), false)
		&& test.AssertEqual<char*>(timer, *queue.Front(), "Hello World!");
}

bool test_dequeue_queue()
{
	highloUnit::Timer timer("test_dequeue_queue");
	
	HLQueue<HLString> queue;
	queue.Enqueue("Hello World!");
	queue.Enqueue("Test!");
	queue.Dequeue();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *queue.Front(), "Test!");
}

bool test_front_queue()
{
	highloUnit::Timer timer("test_front_queue");

	HLQueue<HLString> queue;
	queue.Enqueue("Hello World!");
	HLString result = queue.Front();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result, "Hello World!");
}

bool test_is_empty_queue()
{
	highloUnit::Timer timer("test_is_empty_queue");

	HLQueue<HLString> queue;
	queue.Enqueue("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, queue.IsEmpty(), false);
}

bool test_get_size_of_queue()
{
	highloUnit::Timer timer("test_get_size_of_queue");

	HLQueue<HLString> queue;
	queue.Enqueue("Hello World!");

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, queue.Size(), 1);
}

bool test_clear_queue()
{
	highloUnit::Timer timer("test_clear_queue");

	HLQueue<HLString> queue;
	queue.Enqueue("Hello World!");
	queue.Enqueue("Test!");
	queue.Enqueue("LOL");
	queue.Clear();

	highloUnit::Test test;
	return test.AssertEqual<bool>(timer, queue.IsEmpty(), true)
		&& test.AssertEqual<uint32>(timer, queue.Size(), 0);
}

