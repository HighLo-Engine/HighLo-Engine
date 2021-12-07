// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.8 (2021-11-02) Added operator overloading for native char* strings
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

#include <filesystem>

// FileSystemPath path = "Hello" / "World" -> Hello/World

namespace highlo
{
	struct File
	{
		HLString FileName;					// Represents the real filename without any file extension
		HLString Name;						// Represents the filename with the file extension
		HLString Extension;					// Represents the file extension
		HLString FullPath;					// Represents the full path name (absolute path)
		int64 Size = 0;						// Represents the file size
		bool ExistsOnHardDrive = false;		// Represents, whether the file exists on the hard drive
		bool IsFile = false;				// Represents, whether the file is a file or a folder

		HLAPI File() = default;

		HLAPI File(const HLString &name, int64 size, bool existsOnHardDrive = false, bool isFile = false)
			: Name(name), Size(size), ExistsOnHardDrive(existsOnHardDrive), IsFile(isFile)
		{
		}

		HLAPI File(const File&) = default;
		HLAPI File &operator=(const File&) = default;
	};

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
		HLAPI FileSystemPath &operator=(const File &file);

		HLAPI void Assign(const HLString &source);
		HLAPI void Swap(FileSystemPath &lhs, FileSystemPath &rhs);
		HLAPI uint64 Hash() const;

		HLAPI bool IsEmpty() const;
		HLAPI bool IsRootPath() const;
		HLAPI bool IsParentPath() const;

		HLAPI bool IsAbsolute() const;
		HLAPI bool IsRelative() const;

		HLAPI std::vector<File> GetFileList() const;
		HLAPI uint32 GetFileCount() const;

		HLAPI bool Exists() const;
		HLAPI int64 Size() const;
		HLAPI HLString Absolute() const;
		HLAPI const HLString &Filename() const;
		HLAPI const HLString &Name() const;
		HLAPI const HLString &Extension() const;

		HLAPI bool IsFile() const;
		HLAPI bool IsDirectory() const;
		HLAPI bool IsSymLink() const;

		HLAPI FileSystemPath RelativePath() const;
		HLAPI FileSystemPath RelativePath(const FileSystemPath &parentPath) const;
		HLAPI FileSystemPath ParentPath() const;

		HLAPI HLString &String() { return m_CurrentPath; }
		HLAPI const HLString &String() const { return m_CurrentPath; }

		HLAPI HLString operator*()
		{
			return m_CurrentPath;
		}

		HLAPI const HLString operator*() const
		{
			return m_CurrentPath;
		}

		HLAPI bool operator==(const FileSystemPath &other) const;
		HLAPI bool operator!=(const FileSystemPath &other) const;
		
		HLAPI FileSystemPath &operator/=(const FileSystemPath &path);
		HLAPI FileSystemPath &operator+=(const FileSystemPath &path);
		HLAPI friend FileSystemPath operator/(FileSystemPath &lhs, const FileSystemPath &rhs);

		HLAPI FileSystemPath &operator/=(const HLString &path);
		HLAPI FileSystemPath &operator+=(const HLString &path);
		HLAPI friend FileSystemPath operator/(FileSystemPath &lhs, const HLString &path);

		HLAPI FileSystemPath &operator/=(const char *path);
		HLAPI FileSystemPath &operator+=(const char *path);
		HLAPI friend FileSystemPath operator/(FileSystemPath &lhs, const char *path);

		HLAPI static HLString ExtractFileNameFromPath(const HLString &path, bool excludeExtension = false);
		HLAPI static HLString ExtractFileExtensionFromPath(const HLString &path, bool excludeDot = false);
		HLAPI static HLString ExtractFolderNameFromPath(const HLString &path);

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const FileSystemPath &other);

	private:

		void UpdateAbsolutePath();

		HLString m_CurrentPath;
		HLString m_CurrentAbsolutePath;
		File m_File;
		std::filesystem::path m_Handle;
	};
}

namespace std
{
	template<>
	struct hash<highlo::FileSystemPath>
	{
		std::size_t operator()(const highlo::FileSystemPath &path) const
		{
			return hash<uint64>()(path.Hash());
		}
	};
}

