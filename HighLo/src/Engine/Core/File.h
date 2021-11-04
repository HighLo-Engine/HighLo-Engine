// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2021-10-05) Refactored File implementation to carry a new flag along that determines whether a file actually exists on the hard drive
//     - 1.3 (2021-10-04) Added GetFullName function and refactored some functions to also be callable in a const environment
//     - 1.2 (2021-10-01) fixed GetFileList function (it now pushes only relative paths into the vector and you can retrieve the absolute path from each entry inidividually)
//     - 1.1 (2021-09-22) Added GetFileCount and SetFullPath
//     - 1.0 (2021-09-15) initial release
//

#pragma once

#include <filesystem>

namespace highlo
{
	class File : public IsSharedReference
	{
	public:

		HLAPI File() = default;
		HLAPI File(const HLString &path, bool exists = true);
		HLAPI ~File();

		HLAPI std::vector<File> GetFileList() const;
		HLAPI uint32 GetFileCount() const;

		HLAPI void SetFullPath(const HLString &path);

		HLAPI const HLString &GetRelativePath() const { return m_FilePath; }
		HLAPI HLString GetRelativePath(const HLString &parentPath) const;
		HLAPI const HLString &GetAbsolutePath() const { return m_AbsoluteFilePath; }

		HLAPI HLString GetFullName() { return ExtractFullFileName(m_FilePath); }
		HLAPI const HLString GetFullName() const { return ExtractFullFileName(m_FilePath); }

		HLAPI HLString GetName() const;
		HLAPI HLString GetExtension() const;
		HLAPI int64 GetSize() const;
		HLAPI bool Exists() const;

		HLAPI bool IsFile() const { return m_IsFile; }
		HLAPI bool IsDirectory() const { return !m_IsFile; }
		HLAPI bool IsSymLink() const;

		HLAPI bool operator==(const File &other) const;
		HLAPI bool operator!=(const File &other) const;
		HLAPI operator HLString&() { return m_AbsoluteFilePath; }
		HLAPI operator const HLString&() const { return m_AbsoluteFilePath; }

		HLAPI static HLString GetFileName(const HLString &path);
		HLAPI static HLString GetFileExtension(const HLString &path);
		HLAPI static Ref<File> Create(const HLString &path, bool exists = true);

	private:

		HLString ExtractFullFileName(const HLString &filePath) const;

		std::filesystem::path m_Handle;
		HLString m_FilePath;
		HLString m_AbsoluteFilePath;
		bool m_IsFile = true;
		bool m_ExistsOnHardDrive = true;
	};
}