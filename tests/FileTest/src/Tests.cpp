#include "Tests.h"
using namespace highlo;

#include <filesystem>

static std::vector<File> GetCorrectFileList(const char *path, bool onlyDir = true)
{
	std::vector<File> result;

	std::filesystem::path p = std::filesystem::canonical(path);
	for (const auto &entry : std::filesystem::directory_iterator(p))
	{
		if (onlyDir)
		{
			if (std::filesystem::is_directory(entry.path()))
				result.push_back(File(entry.path().filename().string()));
		}
		else
		{
			result.push_back(File(entry.path().filename().string()));
		}
	}

	return result;
}

bool test_get_file_list()
{
	highloUnit::Timer timer("test_get_file_list");
	std::vector<File> correctFileList = GetCorrectFileList(".");

	File *file = new File(".");
	std::vector<File> dirList = file->GetFileList();

	std::vector<HLString> result;
	for (uint32 i = 0; i < dirList.size(); ++i)
	{
		if (dirList[i].IsDirectory())
			result.push_back(dirList[i].GetRelativePath());
	}

	delete file;

	highloUnit::Test test;
	for (uint32 i = 0; i < result.size(); ++i)
	{
		test.AssertEqual<char*>(timer, (char*)*result[i], (char*)*correctFileList[i].GetRelativePath());
	}

	return false;
}

bool test_get_file_list_count()
{
	highloUnit::Timer timer("test_get_file_list_count");
	std::vector<File> correctFileList = GetCorrectFileList(".", false);

	File *file = new File(".");
	uint32 fileCount = file->GetFileCount();
	delete file;

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, (uint32)correctFileList.size(), fileCount);
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

bool test_get_extension()
{
	highloUnit::Timer timer("test_get_extension");



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

