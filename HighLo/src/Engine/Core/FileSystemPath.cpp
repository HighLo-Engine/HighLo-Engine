// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FileSystemPath.h"

namespace highlo
{
	FileSystemPath::FileSystemPath(const HLString &path)
	{
	}
	
	FileSystemPath::FileSystemPath(const File &file)
	{
	}
	
	FileSystemPath::~FileSystemPath()
	{
	}

	FileSystemPath &FileSystemPath::operator=(const FileSystemPath &other)
	{

		return *this;
	}

	FileSystemPath &FileSystemPath::Assign(HLString &source)
	{

		return *this;
	}

	void FileSystemPath::Swap(FileSystemPath &lhs, FileSystemPath &rhs)
	{
	}

	uint64 FileSystemPath::Hash()
	{
		// TODO
		return 0;
	}

	bool FileSystemPath::IsEmpty() const
	{
		return false;
	}

	bool FileSystemPath::HasRootPath() const
	{
		return false;
	}

	bool FileSystemPath::HasRootName() const
	{
		return false;
	}

	bool FileSystemPath::HasRootDirectory() const
	{
		return false;
	}

	bool FileSystemPath::HasParentPath() const
	{
		return false;
	}

	bool FileSystemPath::IsAbsolute() const
	{
		return false;
	}

	bool FileSystemPath::IsRelative() const
	{
		return false;
	}

	FileSystemPath FileSystemPath::RelativePath()
	{
		return {};
	}

	FileSystemPath FileSystemPath::ParentPath()
	{
		return {};
	}
	
	bool FileSystemPath::operator==(const FileSystemPath &other) const
	{
		return false;
	}

	bool FileSystemPath::operator!=(const FileSystemPath &other) const
	{
		return !(*this == other);
	}

	FileSystemPath &FileSystemPath::operator/=(const FileSystemPath &path)
	{

		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const FileSystemPath &path)
	{

		return *this;
	}
	
	FileSystemPath operator/(const FileSystemPath &lhs, const FileSystemPath &rhs)
	{
		return {};
	}
}

