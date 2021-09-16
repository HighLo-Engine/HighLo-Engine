// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "File.h"
#include "FileSystem.h"

namespace highlo
{
	File::File(const HLString &path)
	{
		m_Handle = std::filesystem::canonical(*path);
		m_FilePath = m_Handle.string();
		if (m_FilePath.Contains('\\'))
			m_FilePath = m_FilePath.Replace("\\", "/", -1);

		m_IsFile = true;
		if (std::filesystem::is_directory(m_Handle))
			m_IsFile = false;

		m_AbsoluteFilePath = std::filesystem::absolute(m_Handle).string();
		if (m_AbsoluteFilePath.Contains('\\'))
			m_AbsoluteFilePath = m_AbsoluteFilePath.Replace("\\", "/", -1);
	}

	File::~File()
	{
	}
	
	std::vector<File> File::GetFileList()
	{
		std::vector<File> result;

		for (const auto &entry : std::filesystem::directory_iterator(m_Handle))
			result.push_back(File(entry.path().string()));

		return result;
	}

	HLString File::GetName()
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

	HLString File::GetExtension()
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

		result = result.Substr(result.IndexOf(".") + 1);
		return result;
	}

	int64 File::GetSize()
	{
		return FileSystem::GetFileSize(m_AbsoluteFilePath);
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

		result = result.Substr(result.IndexOf(".") + 1);
		return result;
	}

	Ref<File> File::Create(const HLString &path)
	{
		return Ref<File>::Create(path);
	}
}
