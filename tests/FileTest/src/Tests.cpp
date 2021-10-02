#include "Tests.h"
using namespace highlo;

bool test_get_file_list()
{
	highloUnit::Timer timer("test_get_file_list");

	File *file = new File(".");
	std::vector<File> dirList = file->GetFileList();

	std::vector<HLString> result;
	for (uint32 i = 0; i < dirList.size(); ++i)
	{
		if (dirList[i].IsDirectory())
		{
			std::cout << dirList[i].GetRelativePath() << std::endl;
			result.push_back(dirList[i].GetRelativePath());
		}
	}

	delete file;

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *result[0], "bin")
		&& test.AssertEqual<char*>(timer, *result[1], "bin-obj")
		&& test.AssertEqual<char*>(timer, *result[2], "src");
}

bool test_get_file_list_count()
{
	highloUnit::Timer timer("test_get_file_list_count");

	File *file = new File(".");
	uint32 fileCount = file->GetFileCount();
	delete file;

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, 7, fileCount);
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

	HLString fileName = File::GetFileName("");

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

	HLString fileExtension = File::GetFileExtension("");

	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_is_file()
{
	highloUnit::Timer timer("test_is_file");

	File *file = new File("");
	bool isFile = file->IsFile();
	delete file;

	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_is_directory()
{
	highloUnit::Timer timer("test_is_directory");

	File *file = new File("bin/");
	bool isDir = file->IsDirectory();
	delete file;

	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

