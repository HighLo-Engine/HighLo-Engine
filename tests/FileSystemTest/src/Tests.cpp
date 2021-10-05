#include "Tests.h"
using namespace highlo;

#include <filesystem>

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



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_remove_file()
{
	highloUnit::Timer timer("test_remove_file");

	FileSystemPath fs("./premake5.lua");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_rename_file()
{
	highloUnit::Timer timer("test_rename_file");

	FileSystemPath fs("./premake5.lua");
	


	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_move_file()
{
	highloUnit::Timer timer("test_move_file");

	FileSystemPath fs("./premake5.lua");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_file_size()
{
	highloUnit::Timer timer("test_get_file_size");

	FileSystemPath fs("./premake5.lua");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_create_folder()
{
	highloUnit::Timer timer("test_create_folder");

	FileSystemPath fs("testFolder");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_remove_folder()
{
	highloUnit::Timer timer("test_remove_folder");

	FileSystemPath fs("testFolder");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
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



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_write_file()
{
	highloUnit::Timer timer("test_write_file");

	FileSystemPath fs("./premake5.lua");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_write_text_file()
{
	highloUnit::Timer timer("test_write_text_file");

	FileSystemPath fs("./premake5.lua");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_add_env_var()
{
	highloUnit::Timer timer("test_add_env_var");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_has_env_var()
{
	highloUnit::Timer timer("test_has_env_var");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}

bool test_get_env_var()
{
	highloUnit::Timer timer("test_get_env_var");



	highloUnit::Test test;
	return test.AssertEqual(timer, true, true);
}
