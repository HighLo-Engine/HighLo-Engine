// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-15) initial release
//

#pragma once

#include <filesystem>

namespace highlo
{
	class File : public IsSharedReference
	{
	public:

		HLAPI File(const HLString &path);
		HLAPI ~File();

		HLAPI std::vector<File> GetFileList();

		HLAPI const HLString &GetRelativePath() const { return m_FilePath; }
		HLAPI const HLString &GetAbsolutePath() const { return m_AbsoluteFilePath; }

		HLAPI HLString GetName();
		HLAPI HLString GetExtension();
		HLAPI int64 GetSize();

		HLAPI bool IsFile() const { return m_IsFile; }
		HLAPI bool IsDirectory() const { return !m_IsFile; }

		HLAPI bool operator==(const File &other) const;
		HLAPI bool operator!=(const File &other) const;
		HLAPI operator HLString&() { return m_AbsoluteFilePath; }
		HLAPI operator const HLString&() const { return m_AbsoluteFilePath; }

		HLAPI static HLString GetFileName(const HLString &path);
		HLAPI static HLString GetFileExtension(const HLString &path);
		HLAPI static Ref<File> Create(const HLString &path);

	private:

		std::filesystem::path m_Handle;
		HLString m_FilePath;
		HLString m_AbsoluteFilePath;
		bool m_IsFile;
	};
}