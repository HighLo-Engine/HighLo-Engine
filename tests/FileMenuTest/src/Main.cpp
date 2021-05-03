#include "FileMenuTest.h"

int main()
{
	std::unique_ptr<FileMenuTest> test = std::make_unique<FileMenuTest>();
	test->Run();

	return 0;
}