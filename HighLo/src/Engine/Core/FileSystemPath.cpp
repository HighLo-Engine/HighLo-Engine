// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FileSystemPath.h"

#include "FileSystem.h"

namespace highlo
{
	FileSystemPath::FileSystemPath(const char *path)
	{
		m_CurrentPath = path;
		m_File = File::Create(path, FileSystem::Get()->FileExists(*this));

	}
	
	FileSystemPath::FileSystemPath(const HLString &path)
	{
		m_CurrentPath = path;
		m_File = File::Create(path, FileSystem::Get()->FileExists(*this));
	}
	
	FileSystemPath::FileSystemPath(const File &file)
	{
		m_CurrentPath = file.GetAbsolutePath();
		m_File = File::Create(file.GetRelativePath(), FileSystem::Get()->FileExists(*this));
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

	FileSystemPath &FileSystemPath::operator=(const HLString &str)
	{
		m_File = File::Create(str, FileSystem::Get()->FileExists(*this));
		m_CurrentPath = str;

		return *this;
	}

	FileSystemPath &FileSystemPath::Assign(const HLString &source)
	{
		m_File = File::Create(source, FileSystem::Get()->FileExists(*this));
		m_CurrentPath = source;
		
		return *this;
	}

	void FileSystemPath::Swap(FileSystemPath &lhs, FileSystemPath &rhs)
	{
		FileSystemPath tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	uint64 FileSystemPath::Hash() const
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
		uint32 minSlashes = 0;
		
		if (HasRootPath())
			++minSlashes;
		
		if (m_CurrentPath.EndsWith("/"))
			++minSlashes;

		return minSlashes > 0;
	}

	bool FileSystemPath::IsAbsolute() const
	{
		return HasRootPath();
	}

	bool FileSystemPath::IsRelative() const
	{
		return !HasRootPath();
	}

	FileSystemPath FileSystemPath::RelativePath() const
	{
		return m_File->GetRelativePath();
	}

	FileSystemPath FileSystemPath::RelativePath(const FileSystemPath &parentPath) const
	{
		return m_File->GetRelativePath(parentPath.String());
	}

	FileSystemPath FileSystemPath::ParentPath() const
	{
		if (HasParentPath())
		{
			HLString result = m_CurrentPath;
			
			uint32 offset = 0;
			if (result.EndsWith("/"))
				offset = 1;

			result = result.Substr(0, result.LastIndexOf("/", offset));
			return FileSystemPath(result);
		}
	
		return *this;
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
		if (path.String().IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path.String();
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const FileSystemPath &path)
	{
		if (path.String().IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path.String();
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator/=(const HLString &path)
	{
		if (path.IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const HLString &path)
	{
		if (path.IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator/=(const char *path)
	{
		if (strlen(path) == 0)
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const char *path)
	{
		if (strlen(path) == 0)
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		m_File->SetFullPath(m_CurrentPath);
		return *this;
	}
	
	FileSystemPath operator/(FileSystemPath &lhs, const FileSystemPath &rhs)
	{
		if (rhs.String().IsEmpty())
			return lhs;

		HLString newPath = "";

		if (lhs.String().EndsWith('/'))
			newPath = lhs.String() + rhs.String();
		else
			newPath = lhs.String() + "/" + rhs.String();

		return FileSystemPath(newPath);
	}
	
	FileSystemPath operator/(FileSystemPath &lhs, const HLString &path)
	{
		if (path.IsEmpty())
			return lhs;

		HLString newPath = "";

		if (lhs.String().EndsWith('/'))
			newPath = lhs.String() + path;
		else
			newPath = lhs.String() + "/" + path;

		return FileSystemPath(newPath);
	}

	FileSystemPath operator/(FileSystemPath &lhs, const char *path)
	{
		if (strlen(path) == 0)
			return lhs;

		HLString newPath = "";

		if (lhs.String().EndsWith('/'))
			newPath = lhs.String() + path;
		else
			newPath = lhs.String() + "/" + path;

		return FileSystemPath(newPath);
	}
}

