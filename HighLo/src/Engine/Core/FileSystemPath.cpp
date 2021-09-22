// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FileSystemPath.h"

namespace highlo
{
	FileSystemPath::FileSystemPath(const HLString &path)
	{
		m_File = File::Create(path);
		m_CurrentPath = m_File->GetAbsolutePath();
	}
	
	FileSystemPath::FileSystemPath(const File &file)
	{
		m_File = File::Create(file.GetAbsolutePath());
		m_CurrentPath = m_File->GetAbsolutePath();
	}
	
	FileSystemPath::~FileSystemPath()
	{
	}

	FileSystemPath &FileSystemPath::operator=(const FileSystemPath &other)
	{
		m_CurrentPath = other.m_CurrentPath;
		m_File = other.m_File;
		return *this;
	}

	FileSystemPath &FileSystemPath::Assign(const HLString &source)
	{
		m_CurrentPath = source;
		m_File = File::Create(source);
		return *this;
	}

	void FileSystemPath::Swap(FileSystemPath &lhs, FileSystemPath &rhs)
	{
		FileSystemPath tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	uint64 FileSystemPath::Hash()
	{
		return m_CurrentPath.Hash();
	}

	bool FileSystemPath::IsEmpty() const
	{
		if (m_File->IsFile())
			return false;

		return m_File->GetFileCount() == 0;
	}

	bool FileSystemPath::HasRootPath() const
	{
		return m_CurrentPath.StartsWith('/') // Unix root path
			|| m_CurrentPath.StartsWith("C:") // windows root path
			|| m_CurrentPath.StartsWith("D:")
			|| m_CurrentPath.StartsWith("G:");
	}

	bool FileSystemPath::HasParentPath() const
	{
		// TODO
		return false;
	}

	bool FileSystemPath::IsAbsolute() const
	{
		return HasRootPath();
	}

	bool FileSystemPath::IsRelative() const
	{
		return !HasRootPath();
	}

	FileSystemPath FileSystemPath::RelativePath()
	{
		// TODO
		return {};
	}

	FileSystemPath FileSystemPath::ParentPath()
	{
		// TODO
		return {};
	}
	
	bool FileSystemPath::operator==(const FileSystemPath &other) const
	{
		return m_CurrentPath == other.m_CurrentPath;
	}

	bool FileSystemPath::operator!=(const FileSystemPath &other) const
	{
		return !(*this == other);
	}

	FileSystemPath &FileSystemPath::operator/=(const FileSystemPath &path)
	{
		m_CurrentPath += "/";
		m_CurrentPath += path.String();
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const FileSystemPath &path)
	{
		m_CurrentPath += "/";
		m_CurrentPath += path.String();
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator/=(const HLString &path)
	{
		m_CurrentPath += "/";
		m_CurrentPath += path;
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const HLString &path)
	{
		m_CurrentPath += "/";
		m_CurrentPath += path;
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}
	
	FileSystemPath operator/(FileSystemPath &lhs, const FileSystemPath &rhs)
	{
		HLString newPath = lhs.String() + "/" + rhs.String();
		return FileSystemPath(newPath);
	}
	
	FileSystemPath operator/(FileSystemPath &lhs, const HLString &path)
	{
		HLString newPath = lhs.String() + "/" + path;
		return FileSystemPath(newPath);
	}
}

