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
	uint32 correctSize = (uint32)GetCorrectFileList(".", false).size();

	File *file = new File(".");
	uint32 fileCount = file->GetFileCount();
	delete file;

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, correctSize, fileCount);
}

bool test_get_size()
{
	highloUnit::Timer timer("test_get_size");

	File *file = new File("./premake5.lua");
	int64 size = file->GetSize();
	delete file;

	std::filesystem::path p = std::filesystem::canonical("./premake5.lua");
	int64 realSize = (int64)std::filesystem::file_size(p);

	highloUnit::Test test;
	return test.AssertEqual<int64>(timer, size, realSize);
}

bool test_get_name()
{
	highloUnit::Timer timer("test_get_name");

	File *file = new File("./premake5.lua");
	HLString name = file->GetName();
	delete file;

	std::filesystem::path p = std::filesystem::canonical("premake5.lua");
	std::string realFileName = p.stem().string();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *name, (char*)realFileName.c_str());
}

bool test_get_extension()
{
	highloUnit::Timer timer("test_get_extension");

	File *file = new File("./premake5.lua");
	HLString extension = file->GetExtension();
	delete file;

	std::filesystem::path p = std::filesystem::canonical("./premake5.lua");
	std::string realfileExtension = p.extension().string();

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *extension, (char*)realfileExtension.c_str());
}

bool test_is_file()
{
	highloUnit::Timer timer("test_is_file");

	File *file = new File("./premake5.lua");
	bool isFile = file->IsFile();
	delete file;

	std::filesystem::path p = std::filesystem::canonical("./premake5.lua");
	bool isRealFile = !std::filesystem::is_directory(p);

	highloUnit::Test test;
	return test.AssertEqual(timer, isFile, isRealFile);
}

bool test_is_directory()
{
	highloUnit::Timer timer("test_is_directory");

	File *file = new File("bin/");
	bool isDir = file->IsDirectory();
	delete file;

	std::filesystem::path p = std::filesystem::canonical("bin/");
	bool isRealDir = std::filesystem::is_directory(p);

	highloUnit::Test test;
	return test.AssertEqual(timer, isDir, isRealDir);
}

