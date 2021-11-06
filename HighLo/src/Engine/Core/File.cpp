// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "File.h"
#include "FileSystem.h"

namespace highlo
{
	File::File(const HLString &path, bool exists)
	{
		SetFullPath(path, exists);
	}

	File::~File()
	{
	}
	
	std::vector<File> File::GetFileList() const
	{
		std::vector<File> result;

		for (const auto &entry : std::filesystem::directory_iterator(m_Handle))
			result.push_back(File(entry.path().filename().string(), FileSystem::Get()->FileExists(entry.path().filename().string().c_str())));

		return result;
	}

	uint32 File::GetFileCount() const
	{
		uint32 fileCount = 0;

		for (const auto &entry : std::filesystem::directory_iterator(m_Handle))
			++fileCount;

		return fileCount;
	}

	void File::SetFullPath(const HLString &path, bool exists)
	{
		m_FilePath = path;
		m_ExistsOnHardDrive = exists;

		if (m_ExistsOnHardDrive)
			m_Handle = std::filesystem::canonical(*path);

		if (m_FilePath.Contains('\\'))
			m_FilePath = m_FilePath.Replace("\\", "/", -1);

		m_IsFile = !std::filesystem::is_directory(m_Handle);
		m_AbsoluteFilePath = std::filesystem::absolute(m_Handle).string();
		if (m_AbsoluteFilePath.Contains('\\'))
			m_AbsoluteFilePath = m_AbsoluteFilePath.Replace("\\", "/", -1);
	}

	HLString File::GetRelativePath(const HLString &parentPath) const
	{
		std::filesystem::path parentHandle(*parentPath);
		std::filesystem::path relativeHandle = std::filesystem::relative(m_Handle, parentHandle);
		return relativeHandle.string();
	}

	HLString File::GetName() const
	{
		HLString result;
		int32 pos = m_FilePath.FirstIndexOf('/');
		int32 i = 1;
		
		while (pos != HLString::NPOS)
		{
			result = m_FilePath.Substr(pos + 1);
			pos = m_FilePath.FirstIndexOf('/', i);
			++i;
		}

		if (result.Contains("."))
			result = result.Substr(0, result.IndexOf("."));

		return result;
	}

	HLString File::GetExtension() const
	{
		HLString result;
		int32 pos = m_FilePath.FirstIndexOf('/');
		int32 i = 1;
	
		while (pos != HLString::NPOS)
		{
			result = m_FilePath.Substr(pos + 1);
			pos = m_FilePath.FirstIndexOf('/', i);
			++i;
		}

		if (!result.Contains("."))
			return "-1";

		result = result.Substr(result.IndexOf("."));
		return result;
	}

	int64 File::GetSize() const
	{
		return FileSystem::Get()->GetFileSize(m_AbsoluteFilePath);
	}

	bool File::Exists() const
	{
		return FileSystem::Get()->FileExists(m_FilePath);
	}

	bool File::IsSymLink() const
	{
		const auto &handle = std::filesystem::directory_entry(m_Handle);
		return handle.is_symlink();
	}

	bool File::operator==(const File &other) const
	{
		return m_AbsoluteFilePath == other.m_AbsoluteFilePath;
	}
	
	bool File::operator!=(const File &other) const
	{
		return !(*this == other);
	}
	
	HLString File::GetFileName(const HLString &path)
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

		if (result.Contains("."))
			result = result.Substr(0, result.IndexOf("."));

		return result;
	}

	HLString File::GetFileExtension(const HLString &path)
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

		result = result.Substr(result.IndexOf("."));
		return result;
	}

	Ref<File> File::Create(const HLString &path, bool exists)
	{
		return Ref<File>::Create(path, exists);
	}
	
	HLString File::ExtractFullFileName(const HLString &filePath) const
	{
		HLString result;
		int32 pos = filePath.FirstIndexOf('/');
		int32 i = 1;

		while (pos != HLString::NPOS)
		{
			result = filePath.Substr(pos + 1);
			pos = filePath.FirstIndexOf('/', i);
			++i;
		}

		return result;
	}
}

