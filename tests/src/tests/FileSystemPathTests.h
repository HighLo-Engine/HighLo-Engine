// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-18) initial release
//

#pragma once

#undef TEST_CATEGORY
#define TEST_CATEGORY FileSystemPathTests

#define TEST_FILE "editorconfig.ini"
#define TEST_FOLDER "./"

#include <HighLo.h>
#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace highlo;

TEST(TEST_CATEGORY, AssignNewPath)
{
	FileSystemPath path("test.lua");
	path.Assign(TEST_FILE);

	HLString pathStr = path.String();
	HLString expected = TEST_FILE;

	EXPECT_EQ(StringEquals(pathStr, expected), true);
}

TEST(TEST_CATEGORY, SwapPaths)
{
	FileSystemPath first("test.lua");
	FileSystemPath second(TEST_FILE);
	HLString firstExpected = TEST_FILE;
	HLString secondExpected = "test.lua";

	first.Swap(first, second);
	
	EXPECT_EQ(StringEquals(first.String(), firstExpected), true);
	EXPECT_EQ(StringEquals(second.String(), secondExpected), true);
}

TEST(TEST_CATEGORY, HashPath)
{
	FileSystemPath path("premake5.lua");
	uint64 expectedHash = 5126544290880117183;
	EXPECT_EQ(path.Hash(), expectedHash);
}

TEST(TEST_CATEGORY, FolderIsEmpty)
{
	FileSystemPath path = TEST_FOLDER;
	bool isEmpty = path.IsEmpty();
	bool expected = false;

	EXPECT_EQ(isEmpty, expected);
}

TEST(TEST_CATEGORY, IsRootPath)
{
	FileSystemPath path = TEST_FOLDER;
	bool isRootPath = path.IsRootPath();
	bool expected = false;

	EXPECT_EQ(isRootPath, expected);
}

TEST(TEST_CATEGORY, IsParentPath)
{
	FileSystemPath path = TEST_FOLDER;
	bool hasParent = path.IsParentPath();
	bool expected = true;

	EXPECT_EQ(hasParent, expected);
}

TEST(TEST_CATEGORY, IsAbsolute)
{
	FileSystemPath path = TEST_FOLDER;
	bool isAbs = path.IsAbsolute();
	bool expected = false;

	EXPECT_EQ(isAbs, expected);
}

TEST(TEST_CATEGORY, IsRelative)
{
	FileSystemPath path = TEST_FOLDER;
	bool relative = path.IsRelative();
	bool expected = true;

	EXPECT_EQ(relative, expected);
}

TEST(TEST_CATEGORY, GetRelativePath)
{
	FileSystemPath path = TEST_FOLDER;
	FileSystemPath relative = path.RelativePath();

	std::cout << "Before: " << path.String().C_Str() << std::endl;
	std::cout << "After: " << relative.String().C_Str() << std::endl;
}

TEST(TEST_CATEGORY, GetParentPath)
{
	FileSystemPath path = TEST_FOLDER;
	FileSystemPath parent = path.ParentPath();

	std::cout << "Before: " << path.Absolute().C_Str() << std::endl;
	std::cout << "After: " << parent.String().C_Str() << std::endl;
}

TEST(TEST_CATEGORY, GetPathAsStr)
{
	FileSystemPath path = TEST_FILE;
	HLString pathStr = path.String();

	EXPECT_EQ(StringEquals(pathStr, TEST_FILE), true);
}

TEST(TEST_CATEGORY, AppendPathToPath1)
{
	FileSystemPath first = "test";
	FileSystemPath second = "premake5.lua";
	FileSystemPath path = first / second;

	HLString expected = "test/premake5.lua";

	EXPECT_EQ(StringEquals(path.String(), expected), true);
}

TEST(TEST_CATEGORY, AppendPathToPath2)
{
	FileSystemPath path = "test";
	FileSystemPath appendMe = "premake5.lua";
	path /= appendMe;

	HLString expected = "test/premake5.lua";

	EXPECT_EQ(StringEquals(path.String(), expected), true);
}

TEST(TEST_CATEGORY, AppendStringToPath1)
{
	FileSystemPath path = FileSystemPath("test") / HLString("premake5.lua");
	HLString expected = "test/premake5.lua";

	EXPECT_EQ(StringEquals(path.String(), expected), true);
}

TEST(TEST_CATEGORY, AppendStringToPath2)
{
	FileSystemPath path = "test";
	HLString appendMe = "premake5.lua";
	path /= appendMe;

	HLString expected = "test/premake5.lua";

	EXPECT_EQ(StringEquals(path.String(), expected), true);
}

TEST(TEST_CATEGORY, GetFileSizeTest)
{
	FileSystemPath path = TEST_FILE;
	uint64 size = path.Size();

	EXPECT_GT(size, 0);
}

TEST(TEST_CATEGORY, GetFileListCount)
{
	FileSystemPath path(TEST_FOLDER);
	uint32 fileCount = path.GetFileCount();
	uint32 expected = 7;

	EXPECT_EQ(fileCount, expected);
}

TEST(TEST_CATEGORY, GetFileList)
{
	FileSystemPath path(TEST_FOLDER);
	std::vector<File> fileList = path.GetFileList();

	EXPECT_EQ(StringEquals(fileList[0].Name, "assimp-vc142-mtd.dll"), true);
	EXPECT_EQ(StringEquals(fileList[1].Name, "editorconfig.ini"), true);
	EXPECT_EQ(StringEquals(fileList[2].Name, "HighLoTest.exe"), true);
	EXPECT_EQ(StringEquals(fileList[3].Name, "HighLoTest.pdb"), true);
	EXPECT_EQ(StringEquals(fileList[4].Name, "libcrypto-3-x64.dll"), true);
	EXPECT_EQ(StringEquals(fileList[5].Name, "libssl-3-x64.dll"), true);
	EXPECT_EQ(StringEquals(fileList[6].Name, "shaderc_sharedd.dll"), true);
}

TEST(TEST_CATEGORY, GetIsFile)
{
	FileSystemPath path = TEST_FILE;
	bool isFile = path.IsFile();
	bool expected = true;

	EXPECT_EQ(isFile, expected);
}

TEST(TEST_CATEGORY, GetIsDirectory)
{
	FileSystemPath path = TEST_FILE;
	bool isDir = path.IsDirectory();
	bool expected = false;

	EXPECT_EQ(isDir, expected);
}

