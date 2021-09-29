#include "Tests.h"
using namespace highlo;

bool test_get_file_list()
{
	highloUnit::Timer timer("test_get_file_list");

	/*
	File *file = new File("assets");
	std::vector<File> dirList = file->GetFileList();

	for (uint32 i = 0; i < dirList.size(); ++i)
		std::cout << dirList[i].GetAbsolutePath() << std::endl;
	*/

	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_file_list_count()
{
	highloUnit::Timer timer("test_get_file_list_count");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_set_full_path()
{
	highloUnit::Timer timer("test_set_full_path");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_relative_path()
{
	highloUnit::Timer timer("test_get_relative_path");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_absolute_path()
{
	highloUnit::Timer timer("test_get_absolute_path");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_size()
{
	highloUnit::Timer timer("test_get_size");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_name()
{
	highloUnit::Timer timer("test_get_name");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_static_name()
{
	highloUnit::Timer timer("test_get_static_name");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_extension()
{
	highloUnit::Timer timer("test_get_extension");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_static_extension()
{
	highloUnit::Timer timer("test_get_static_extension");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_is_file()
{
	highloUnit::Timer timer("test_is_file");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_is_directory()
{
	highloUnit::Timer timer("test_is_directory");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

