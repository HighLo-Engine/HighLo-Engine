#include "Tests.h"
using namespace highlo;

#include <filesystem>
#include <fstream>

bool test_file_exists()
{
	highloUnit::Timer timer("test_file_exists");

	FileSystemPath fs("./premake5.lua");	
	bool result = FileSystem::FileExists(fs);

	std::filesystem::path realPath("./premake5.lua");
	bool realExists = std::filesystem::exists(realPath);

	highloUnit::Test test;
	return test.AssertEqual(timer, result, realExists);
}

bool test_path_exists()
{
	highloUnit::Timer timer("test_path_exists");

	FileSystemPath fs("bin/");
	bool result = FileSystem::PathExists(fs);

	std::filesystem::path p("bin/");
	bool realExists = std::filesystem::is_directory(p);

	highloUnit::Test test;
	return test.AssertEqual(timer, result, realExists);
}

bool test_remove_file()
{
	highloUnit::Timer timer("test_remove_file");
	char *fileName = "testFiles/testFileToRemove.txt";

	FileSystemPath fs(fileName);
	bool result = FileSystem::RemoveFile(fs);

	std::filesystem::path p(fileName);
	bool shouldNotExist = !std::filesystem::exists(p);

	highloUnit::Test test;
	return test.AssertEqual(timer, result, shouldNotExist);
}

bool test_rename_file()
{
	highloUnit::Timer timer("test_rename_file");
	char *fileName = "testFiles/testFileToRename.txt";
	char *fileNameAfter = "testFiles/testFileAfterRenaming.txt";

	FileSystemPath fs(fileName);
	FileSystemPath newFs(fileNameAfter);
	HLString newFilePath = FileSystem::Rename(fs, newFs);
	

	std::filesystem::path oldP(fileName);
	std::filesystem::path p(fileNameAfter);
	bool exists = std::filesystem::exists(p);
	bool oldDoesNotExist = !std::filesystem::exists(oldP);

	highloUnit::Test test;
	return test.AssertEqual(timer, exists, oldDoesNotExist);
}

bool test_move_file()
{
	highloUnit::Timer timer("test_move_file");

	char *fileName = "testFiles/testFileToMove.txt";
	char *fileNameAfter = "testFiles/testFileAfterMoving.txt";
	FileSystemPath fs(fileName);
	FileSystemPath movedFs(fileNameAfter);

	bool result = FileSystem::Move(fs, movedFs);
	if (!result)
		return true;

	std::filesystem::path oldP(fileName);
	std::filesystem::path p(fileNameAfter);
	
	bool exists = std::filesystem::exists(p);
	bool oldDoesNotExist = !std::filesystem::exists(oldP);

	highloUnit::Test test;
	return test.AssertEqual(timer, exists, oldDoesNotExist);
}

bool test_get_file_size()
{
	highloUnit::Timer timer("test_get_file_size");

	FileSystemPath fs("./premake5.lua");
	uint64 size = FileSystem::GetFileSize(fs);

	std::filesystem::path p("./premake5.lua");
	uint64 realSize = (uint64)std::filesystem::file_size(p);

	highloUnit::Test test;
	return test.AssertEqual<uint64>(timer, size, realSize);
}

bool test_create_folder()
{
	highloUnit::Timer timer("test_create_folder");

	FileSystemPath fs("testFolder");
	bool result = FileSystem::CreateFolder(fs);

	std::filesystem::path p("testFolder");
	bool realResult = std::filesystem::is_directory(p);

	highloUnit::Test test;
	return test.AssertEqual(timer, result, realResult);
}

bool test_remove_folder()
{
	highloUnit::Timer timer("test_remove_folder");

	FileSystemPath fs("testFolder");
	bool result = FileSystem::RemoveFolder(fs);

	std::filesystem::path p("testFolder");
	bool realResult = !std::filesystem::is_directory(p);

	highloUnit::Test test;
	return test.AssertEqual(timer, result, realResult);
}

bool test_read_file()
{
	highloUnit::Timer timer("test_read_file");

	FileSystemPath fs("./premake5.lua");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_read_text_file()
{
	highloUnit::Timer timer("test_read_text_file");

	FileSystemPath fs("./premake5.lua");
	HLString text = FileSystem::ReadTextFile(fs);

	char realText[4096];
	std::ifstream file("./premake5.lua");
	
	if (file.is_open())
	{
		file.read(realText, 4096);
		file.close();
	}

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *text, realText);
}

bool test_write_file()
{
	highloUnit::Timer timer("test_write_file");
	HLString textToWrite = "This is a demo String!";
	char *fileName = "./premake5.lua";

	FileSystemPath fs(fileName);
	bool result = FileSystem::WriteTextFile(fs, textToWrite);

	std::ofstream file(fileName);

	if (file.is_open())
	{
		file.write(*textToWrite, textToWrite.Length());
		file.close();
	}

	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_write_text_file()
{
	highloUnit::Timer timer("test_write_text_file");
	HLString textToWrite = "__TEST__";

	FileSystemPath fs("./premake5.lua");
	bool result = FileSystem::WriteTextFile(fs, textToWrite);
	if (!result)
		return true;

	HLString readText = FileSystem::ReadTextFile(fs);

	highloUnit::Test test;
	return test.AssertEqual(timer, readText.Contains(textToWrite), true);
}

bool test_add_env_var()
{
	highloUnit::Timer timer("test_add_env_var");
	HLString userName = "John Doe";
	HLString resultUserName;

	FileSystem::SetEnvironmentVariable("HIGHLO_ENV_USERNAME", userName);
	if (FileSystem::HasEnvironmentVariable("HIGHLO_ENV_USERNAME"))
		resultUserName = FileSystem::GetEnvironmentVariable("HIGHLO_ENV_USERNAME");

	highloUnit::Test test;
	return test.AssertEqual<char*>(timer, *userName, *resultUserName);
}

bool test_has_env_var()
{
	highloUnit::Timer timer("test_has_env_var");

	FileSystem::SetEnvironmentVariable("HIGHLO_ENV_USERNAME", "John Doe");
	bool result = FileSystem::HasEnvironmentVariable("HIGHLO_ENV_USERNAME");

	highloUnit::Test test;
	return test.AssertEqual(timer, result, true);
}

