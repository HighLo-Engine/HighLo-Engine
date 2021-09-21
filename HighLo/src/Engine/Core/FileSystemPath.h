// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
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
		HLAPI FileSystemPath(const HLString &path);
		HLAPI FileSystemPath(const File &file);
		HLAPI ~FileSystemPath();
		HLAPI FileSystemPath &operator=(const FileSystemPath &other);

		HLAPI FileSystemPath &Assign(HLString &source);
		HLAPI void Swap(FileSystemPath &lhs, FileSystemPath &rhs);
		HLAPI uint64 Hash();

		HLAPI bool IsEmpty() const;
		HLAPI bool HasRootPath() const;
		HLAPI bool HasRootName() const;
		HLAPI bool HasRootDirectory() const;
		HLAPI bool HasParentPath() const;

		HLAPI bool IsAbsolute() const;
		HLAPI bool IsRelative() const;

		HLAPI FileSystemPath RelativePath();
		HLAPI FileSystemPath ParentPath();

		HLAPI HLString &String() { return m_CurrentPath; }
		HLAPI const HLString &String() const { return m_CurrentPath; }

		HLAPI bool operator==(const FileSystemPath &other) const;
		HLAPI bool operator!=(const FileSystemPath &other) const;
		
		HLAPI FileSystemPath &operator/=(const FileSystemPath &path);
		HLAPI FileSystemPath &operator+=(const FileSystemPath &path);
		HLAPI friend FileSystemPath operator/(const FileSystemPath &lhs, const FileSystemPath &rhs);

	private:

		HLString m_CurrentPath;
	};
}