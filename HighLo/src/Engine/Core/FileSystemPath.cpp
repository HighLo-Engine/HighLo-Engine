// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FileSystemPath.h"

#include "FileSystem.h"

namespace highlo
{
	FileSystemPath::FileSystemPath(const char *path)
	{
		Assign(path);
	}
	
	FileSystemPath::FileSystemPath(const HLString &path)
	{
		Assign(path);
	}

	FileSystemPath::FileSystemPath(const File &file)
	{
		Assign(file.FullPath);
	}
	
	FileSystemPath::~FileSystemPath()
	{
	}

	FileSystemPath &FileSystemPath::operator=(const FileSystemPath &other)
	{
		Assign(other.String());
		return *this;
	}

	FileSystemPath &FileSystemPath::operator=(const HLString &str)
	{
		Assign(str);
		return *this;
	}

	FileSystemPath &FileSystemPath::operator=(const File &file)
	{
		Assign(file.FullPath);
		return *this;
	}

	void FileSystemPath::Assign(const HLString &source)
	{
		m_CurrentPath = source;
		if (m_CurrentPath.Contains('\\'))
			m_CurrentPath = m_CurrentPath.Replace("\\", "/");

		if (Exists())
		{
			m_Handle = std::filesystem::path(*m_CurrentPath);
			m_File.ExistsOnHardDrive = true;
			m_File.IsFile = !std::filesystem::is_directory(m_Handle);
			m_File.Size = Size();
			m_File.FullPath = std::filesystem::absolute(m_Handle).string();

			if (m_File.FullPath.Contains('\\'))
				m_File.FullPath = m_File.FullPath.Replace("\\", "/");

			if (m_File.IsFile)
			{
				m_File.Name = ExtractFileNameFromPath(m_File.FullPath);
				m_File.FileName = ExtractFileNameFromPath(m_File.FullPath, true);
				m_File.Extension = ExtractFileExtensionFromPath(m_File.FullPath, true);
			}
			else
			{
				m_File.Extension = "folder";
				m_File.Name = ExtractFolderNameFromPath(m_File.FullPath);
				m_File.FileName = m_File.Name;

			}
		}
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
		return m_CurrentPath.IsEmpty();
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

	std::vector<File> FileSystemPath::GetFileList() const
	{
		std::vector<File> result;

		for (const auto &entry : std::filesystem::directory_iterator(m_Handle))
		{
			int64 size = FileSystem::Get()->GetFileSize(entry.path().string().c_str());

			File file;
			file.ExistsOnHardDrive = entry.is_block_file() || entry.is_regular_file() || entry.is_directory() || entry.is_character_file() || entry.is_other() || entry.is_symlink();
			file.IsFile = !entry.is_directory();
			file.FullPath = std::filesystem::absolute(entry).string().c_str();
			file.Size = size;

			if (file.FullPath.Contains('\\'))
				file.FullPath = file.FullPath.Replace("\\", "/");

			if (file.IsFile)
			{
				file.Extension = ExtractFileExtensionFromPath(file.FullPath, true);
				file.Name = ExtractFileNameFromPath(file.FullPath);
				file.FileName = ExtractFileNameFromPath(file.FullPath, true);
			}
			else
			{
				file.Extension = "folder";
				file.Name = ExtractFolderNameFromPath(file.FullPath);
				file.FileName = file.Name;
			}

			result.push_back(file);
		}

		return result;
	}

	uint32 FileSystemPath::GetFileCount() const
	{
		uint32 fileCount = 0;

		for (const auto &entry : std::filesystem::directory_iterator(m_Handle))
			++fileCount;

		return fileCount;
	}

	bool FileSystemPath::Exists() const
	{
		return FileSystem::Get()->FileExists(*this) || FileSystem::Get()->FolderExists(*this);
	}

	int64 FileSystemPath::Size() const
	{
		return FileSystem::Get()->GetFileSize(*this);
	}

	const HLString &FileSystemPath::Absolute() const
	{
		return m_File.FullPath;
	}

	const HLString &FileSystemPath::Filename() const
	{
		return m_File.FileName;
	}

	const HLString &FileSystemPath::Name() const
	{
		return m_File.Name;
	}

	const HLString &FileSystemPath::Extension() const
	{
		return m_File.Extension;
	}

	bool FileSystemPath::IsFile() const
	{
		return m_File.IsFile;
	}

	bool FileSystemPath::IsDirectory() const
	{
		return !m_File.IsFile;
	}

	bool FileSystemPath::IsSymLink() const
	{
		const auto &handle = std::filesystem::directory_entry(m_Handle);
		return handle.is_symlink();
	}

	FileSystemPath FileSystemPath::RelativePath() const
	{
		std::filesystem::path relativeHandle = std::filesystem::relative(m_Handle);
		return relativeHandle.string();
	}

	FileSystemPath FileSystemPath::RelativePath(const FileSystemPath &parentPath) const
	{
		std::filesystem::path parentHandle(**parentPath);
		std::filesystem::path relativeHandle = std::filesystem::relative(m_Handle, parentHandle);
		return relativeHandle.string();
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
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const FileSystemPath &path)
	{
		if (path.String().IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path.String();
		return *this;
	}

	FileSystemPath &FileSystemPath::operator/=(const HLString &path)
	{
		if (path.IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const HLString &path)
	{
		if (path.IsEmpty())
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		return *this;
	}

	FileSystemPath &FileSystemPath::operator/=(const char *path)
	{
		if (strlen(path) == 0)
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		return *this;
	}

	FileSystemPath &FileSystemPath::operator+=(const char *path)
	{
		if (strlen(path) == 0)
			return *this;

		if (!m_CurrentPath.EndsWith('/'))
			m_CurrentPath += "/";

		m_CurrentPath += path;
		return *this;
	}

	HLString FileSystemPath::ExtractFileNameFromPath(const HLString &path, bool excludeExtension)
	{
		HLString result;
		int32 pos = path.FirstIndexOf('/');
		int32 i = 1;

		while (pos != HLString::NPOS)
		{
			result = path.Substr(pos + 1);
			pos = path.FirstIndexOf('/', i);
			++i;
		}

		if (excludeExtension)
		{
			if (result.Contains("."))
			{
				result = result.Substr(0, result.IndexOf("."));
			}
		}

		return result;
	}

	HLString FileSystemPath::ExtractFileExtensionFromPath(const HLString &path, bool excludeDot)
	{
		HLString result;
		int32 pos = path.FirstIndexOf('/');
		int32 i = 1;

		while (pos != HLString::NPOS)
		{
			result = path.Substr(pos + 1);
			pos = path.FirstIndexOf('/', i);
			++i;
		}

		if (!result.Contains("."))
			return "-1";

		if (excludeDot)
			result = result.Substr(result.IndexOf(".") + 1);
		else
			result = result.Substr(result.IndexOf("."));

		return result;
	}

	HLString FileSystemPath::ExtractFolderNameFromPath(const HLString &path)
	{
		HLString result = path;
		if (result.EndsWith('/'))
			result = result.Substr(0, result.LastIndexOf('/'));

		return result.Substr(result.LastIndexOf('/') + 1);
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
	
	std::ostream &operator<<(std::ostream &stream, const FileSystemPath &other)
	{
		stream << other.Filename();
		return stream;
	}
}

