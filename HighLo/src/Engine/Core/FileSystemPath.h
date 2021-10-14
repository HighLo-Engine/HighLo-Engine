// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.7 (2021-10-14) Fixed ParentPath implementation to take non root paths into account
//     - 1.6 (2021-10-05) Refactored implementation to be able to handle non-existent files on the hard drive
//     - 1.5 (2021-10-05) Fixed bug where the engine would crash if a Filepath does not exist
//     - 1.4 (2021-10-04) Refactored some functions to work in a const environment as well
//     - 1.3 (2021-09-26) Added constructor and operator= for the Strings
//     - 1.2 (2021-09-22) Added File getter, more overloads and implemented most methods
//     - 1.1 (2021-09-21) Added String methods
//     - 1.0 (2021-09-15) initial release
//

#pragma once

#include "File.h"

namespace highlo
{
	class FileSystemPath : public IsSharedReference
	{
	public:

		HLAPI FileSystemPath() = default;
		HLAPI FileSystemPath(const char *path);
		HLAPI FileSystemPath(const HLString &path);
		HLAPI FileSystemPath(const File &file);
		HLAPI FileSystemPath(const FileSystemPath&) = default;
		HLAPI ~FileSystemPath();

		HLAPI FileSystemPath &operator=(const FileSystemPath &other);
		HLAPI FileSystemPath &operator=(const HLString &str);

		HLAPI FileSystemPath &Assign(const HLString &source);
		HLAPI void Swap(FileSystemPath &lhs, FileSystemPath &rhs);
		HLAPI uint64 Hash() const;

		HLAPI bool IsEmpty() const;
		HLAPI bool HasRootPath() const;
		HLAPI bool HasParentPath() const;

		HLAPI bool IsAbsolute() const;
		HLAPI bool IsRelative() const;

		HLAPI FileSystemPath RelativePath() const;
		HLAPI FileSystemPath ParentPath() const;

		HLAPI Ref<File> &GetFile() { return m_File; }
		HLAPI const Ref<File> &GetFile() const { return m_File; }

		HLAPI HLString &String() { return m_CurrentPath; }
		HLAPI const HLString &String() const { return m_CurrentPath; }

		HLAPI bool operator==(const FileSystemPath &other) const;
		HLAPI bool operator!=(const FileSystemPath &other) const;
		
		HLAPI FileSystemPath &operator/=(const FileSystemPath &path);
		HLAPI FileSystemPath &operator+=(const FileSystemPath &path);
		HLAPI friend FileSystemPath operator/(FileSystemPath &lhs, const FileSystemPath &rhs);

		HLAPI FileSystemPath &operator/=(const HLString &path);
		HLAPI FileSystemPath &operator+=(const HLString &path);
		HLAPI friend FileSystemPath operator/(FileSystemPath &lhs, const HLString &path);

	private:

		HLString m_CurrentPath;
		Ref<File> m_File = nullptr;
	};
}