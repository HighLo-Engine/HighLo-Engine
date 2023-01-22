// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/FileSystem.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include <shellapi.h>
#include <fileapi.h>
#include <Shlobj.h>

#include "Engine/Core/FileSystemWatcher.h"

namespace highlo
{
    namespace utils
    {
        void CALLBACK FileIOCompleteRoutine(DWORD errorCode, DWORD numberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
        {
        }

        static HANDLE OpenFileInternal(const HLString &path)
        {
            return CreateFileW(path.W_Str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
        }

        static int64 GetFileSizeInternal(HANDLE file)
        {
            LARGE_INTEGER size;
            GetFileSizeEx(file, &size);
            return size.QuadPart;
        }

        static bool ReadFileInternal(HANDLE file, void *buffer, int64 size)
        {
            OVERLAPPED ol = { 0 };
            return ReadFileEx(file, buffer, (DWORD) size, &ol, FileIOCompleteRoutine);
        }

        // @see: https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shfileoperationw#return-value
        static HLString TranslateWindowsShellOperationErrorCodeIntoHumanReadable(int32 errorCode)
        {
            switch (errorCode)
            {
                case 0x71:      return "The source and destination files are the same file.";
                case 0x72:      return "Multiple file paths were specified in the source buffer, but only one destination file path.";
                case 0x73:      return "Rename operation was specified but the destination path is a different directory. Use the move operation instead.";
                case 0x74:      return "The source is a root directory, which cannot be moved or renamed.";
                case 0x75:      return "The operation was canceled by the user, or silently canceled if the appropriate flags were supplied to SHFileOperation.";
                case 0x76:      return "The destination is a subtree of the source.";
                case 0x78:      return "Security settings denied access to the source.";
                case 0x79:      return "The source or destination path exceeded or would exceed MAX_PATH.";
                case 0x7A:      return "The operation involved multiple destination paths, which can fail in the case of a move operation.";
                case 0x7C:      return "The path in the source or destination or both was invalid.";
                case 0x7D:      return "The source and destination have the same parent folder.";
                case 0x7E:      return "The destination path is an existing file.";
                case 0x80:      return "The destination path is an existing folder.";
                case 0x81:      return "The name of the file exceeds MAX_PATH.";
                case 0x82:      return "The destination is a read-only CD-ROM, possibly unformatted.";
                case 0x83:      return "The destination is a read-only DVD, possibly unformatted.";
                case 0x84:      return "The destination is a writable CD-ROM, possibly unformatted.";
                case 0x85:      return "The file involved in the operation is too large for the destination media or file system.";
                case 0x86:      return "The source is a read-only CD-ROM, possibly unformatted.";
                case 0x87:      return "The source is a read-only DVD, possibly unformatted.";
                case 0x88:      return "The source is a writable CD-ROM, possibly unformatted.";
                case 0xB7:      return "MAX_PATH was exceeded during the operation.";
                case 0x402:     return "An unknown error occurred. This is typically due to an invalid path in the source or destination. This error does not occur on Windows Vista and later.";
                case 0x10000:   return "An unspecified error occurred on the destination.";
                case 0x10074:   return "Destination is a root directory and cannot be renamed.";
            }

            return "Unknown Error: " + HLString::ToString(errorCode);
        }

        // @see: https://docs.microsoft.com/de-de/windows/win32/debug/system-error-codes--0-499-
        static HLString TranslateErrorCodeIntoHumanReadable(DWORD errorCode)
        {
            switch (errorCode)
            {
                case 0:
                    return "success";

                case 1:
                    return "Wrong function";

                case 2:
                    return "File not found!";

                case 3:
                    return "The system could not find the file!";

                case 4:
                    return "The system could not open the file!";

                case 5:
                    return "Access denied!";

                case 6:
                    return "Invalid handle!";

                case 7:
                    return "NOT ENOUGH RAM!";

                case 8:
                    return "NOT ENOUGH RESOURCES";

                case 9:
                    return "INVALID ADDRESS";

                case 10:
                    return "Invalid format";

                case 11:
                    return "a program was loaded in a wrong format";

                case 12:
                    return "access key is invalid";

                case 13:
                    return "the data is invalid";

                case 14:
                    return "not enough space on the hard drive";

                case 15:
                    return "could not find the drive";

                case 16:
                    return "the directory could not been removed";

                case 17:
                    return "the file could not be copied onto another drive";

                case 18:
                    return "no files were found";

                case 19:
                    return "the medium is write secured";

                case 20:
                    return "the system could not find the device";

                case 145:
                    return "The directory is not empty!";

                case 183:
                    return "File already exists!";
            }

            return "Unknown Error: " + HLString::ToString(errorCode);
        }

        static FileSystemPath GetFolderById(const KNOWNFOLDERID &id)
        {
            PWSTR filePath;
            HRESULT result = SHGetKnownFolderPath(id, KF_FLAG_DEFAULT, NULL, &filePath);
            HL_ASSERT(result == S_OK);

            std::wstring filepathWstr = filePath;
            std::replace(filepathWstr.begin(), filepathWstr.end(), L'\\', L'/');
            std::filesystem::path resultPath = filepathWstr + L"/";
            return FileSystemPath(resultPath.string());
        }
    }

    bool FileSystem::FileExists(const FileSystemPath &path)
    {
        HLString pathStr = path.String();
        if (pathStr.IsEmpty())
            return false;

        DWORD result = GetFileAttributesW(pathStr.W_Str());
        return !(result == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
    }

    bool FileSystem::SetCurrentWorkingDirectory(const FileSystemPath &path)
    {
        return ::SetCurrentDirectoryW(path.String().W_Str());
    }

    FileSystemPath FileSystem::GetCurrentWorkingDirectory()
    {
        CHAR buffer[MAX_PATH];
        DWORD dwRet;

        dwRet = ::GetCurrentDirectoryA(MAX_PATH, buffer);

        if (dwRet == 0)
        {
            HL_CORE_ERROR("GetCurrentDirectory failed {}", GetLastError());
            return FileSystemPath::INVALID_PATH;
        }

        if (dwRet >= MAX_PATH)
        {
            HL_CORE_ERROR("Buffer too small; need {} characters", dwRet);
            return FileSystemPath::INVALID_PATH;
        }

        buffer[dwRet] = '\0';
        return FileSystemPath(buffer);
    }

    bool FileSystem::FolderExists(const FileSystemPath &path)
    {
        HLString pathStr = path.String();
        if (pathStr.IsEmpty())
            return false;

        DWORD result = GetFileAttributesW(pathStr.W_Str());
        return (result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool FileSystem::RemoveFile(const FileSystemPath &path)
    {
        return ::DeleteFileW(path.String().W_Str());
    }

    bool FileSystem::Copy(const FileSystemPath &dest, const FileSystemPath &src, bool overrideIfExist)
    {
        return ::CopyFileW(src.String().W_Str(), dest.String().W_Str(), !overrideIfExist);
    }

    bool FileSystem::CopyRecursive(const FileSystemPath &dest, const FileSystemPath &src)
    {
        HLString source = src.String();
        if (!source.EndsWith("/"))
        {
            source = source + "\\*";
        }

        SHFILEOPSTRUCTW s = { 0 };
        s.wFunc = FO_COPY;
        s.pTo = dest.String().W_Str();
        s.pFrom = source.W_Str();
        s.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NO_UI;
        int32 result = SHFileOperationW(&s);
        return result == 0;
    }

    int64 FileSystem::GetFileSize(const FileSystemPath &path)
    {
        HANDLE file = utils::OpenFileInternal(path.String());
        if (file == INVALID_HANDLE_VALUE)
        {
            const DWORD error = GetLastError();
            HLString errorText = utils::TranslateErrorCodeIntoHumanReadable(error);
            return -1;
        }

        int64 size = utils::GetFileSizeInternal(file);
        CloseHandle(file);
        return size;
    }

    bool FileSystem::CreateFolder(const FileSystemPath &path)
    {
        bool success = CreateDirectoryW(path.String().W_Str(), NULL);
        if (!success)
            HL_CORE_ERROR("WinAPI Error: {0}", *utils::TranslateErrorCodeIntoHumanReadable(GetLastError()));

        return success;
    }

    bool FileSystem::RemoveFolder(const FileSystemPath &path, bool forceDelete)
    {
        if (!forceDelete)
        {
            bool success = RemoveDirectoryW(path.String().W_Str());
            if (!success)
                HL_CORE_ERROR("WinAPI Error: {0}", *utils::TranslateErrorCodeIntoHumanReadable(GetLastError()));

            return success;
        }
        else
        {
            // the user wants to force-delete the directory, without the limitation of only being able to remove an empty directory
            // therefore we use the Windows Shell instead of the Win32 API

            SHFILEOPSTRUCT fileOperation;
            fileOperation.hwnd = NULL;
            fileOperation.wFunc = FO_DELETE;
            fileOperation.pFrom = path.String().W_Str();
            fileOperation.pTo = L"";
            fileOperation.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
            fileOperation.fAnyOperationsAborted = false;
            fileOperation.fFlags = 0;
            fileOperation.hNameMappings = L"";

            int32 status = SHFileOperationW(&fileOperation);
            if (status != 0)
                HL_CORE_ERROR("WinAPI Error: {0}", *utils::TranslateWindowsShellOperationErrorCodeIntoHumanReadable(status));

            return status == 0;
        }
    }

    HLString FileSystem::Rename(const FileSystemPath &path, const FileSystemPath &newName)
    {
        FileSystemWatcher::Get()->DisableWatchUntilNextAction();
        HLString newFilePath;

        if (path.IsParentPath())
            newFilePath = path.ParentPath().String() + "/" + newName.String();
        else
            newFilePath = newName.String();

        BOOL result = MoveFileW(path.String().W_Str(), newFilePath.W_Str());
        HL_ASSERT(result);
        return newFilePath;
    }

    bool FileSystem::Move(const FileSystemPath &filePath, const FileSystemPath &dest)
    {
        FileSystemWatcher::Get()->DisableWatchUntilNextAction();
        BOOL result = MoveFileW(filePath.String().W_Str(), dest.String().W_Str());
        return result != 0;
    }

    Byte *FileSystem::ReadFile(const FileSystemPath &path, int64 *outSize)
    {
        HANDLE file = utils::OpenFileInternal(path.String());
        int64 size = utils::GetFileSizeInternal(file);
        Byte *buffer = new Byte[size];
        bool result = utils::ReadFileInternal(file, buffer, size);
        CloseHandle(file);

        if (!result)
            delete[] buffer;

        if (outSize)
            *outSize = size;
        
        return result ? buffer : nullptr;
    }

    HLString FileSystem::ReadTextFile(const FileSystemPath &path)
    {
        HANDLE file = utils::OpenFileInternal(path.String());
        uint32 size = (uint32)utils::GetFileSizeInternal(file);

        char *readBuffer = new char[size + 1];
        readBuffer[size] = '\0';
        bool success = utils::ReadFileInternal(file, &readBuffer[0], size);
        HLString result = HLString(readBuffer);
        delete[] readBuffer;

        CloseHandle(file);
        return success ? result : "";
    }

    bool FileSystem::WriteFile(const FileSystemPath &path, Byte *buffer, int64 size)
    {
        HANDLE file = CreateFileW(path.String().W_Str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE)
            return false;

        DWORD written;
        bool result = ::WriteFile(file, buffer, (DWORD)size, &written, NULL);
        CloseHandle(file);
        return result;
    }

    bool FileSystem::WriteTextFile(const FileSystemPath &path, const HLString &text)
    {
        return WriteFile(path, (Byte*)&text[0], (int64)text.Length());
    }

    void FileSystem::OpenInExplorer(const FileSystemPath &path)
    {
        ShellExecuteW(0, L"open", path.Absolute().W_Str(), 0, 0, SW_SHOWDEFAULT);
    }

    void FileSystem::OpenInBrowser(const HLString &url)
    {
        ShellExecuteW(0, 0, url.W_Str(), 0, 0, SW_SHOW);
    }

    bool FileSystem::HasEnvironmentVariable(const HLString &key)
    {
        HKEY hKey;
        LPCSTR keyPath = "Environment";
        LSTATUS lOpenStatus = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_ALL_ACCESS, &hKey);

        if (lOpenStatus == ERROR_SUCCESS)
        {
            lOpenStatus = RegQueryValueExA(hKey, *key, 0, NULL, NULL, NULL);
            RegCloseKey(hKey);
        }

        return lOpenStatus == ERROR_SUCCESS;
    }

    bool FileSystem::SetEnvironmentVariable(const HLString &key, const HLString &value)
    {
        HKEY hKey;
        LPCSTR keyPath = "Environment";
        DWORD createdNewKey;
        LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);
        
        if (lOpenStatus == ERROR_SUCCESS)
        {
            LSTATUS lSetStatus = RegSetValueExA(hKey, *key, 0, REG_SZ, (LPBYTE)*value, value.Length() + 1);
            RegCloseKey(hKey);
            
            if (lSetStatus == ERROR_SUCCESS)
            {
                SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
                return true;
            }
        }

        return false;
    }

    HLString FileSystem::GetEnvironmentVariable(const HLString &key)
    {
        HKEY hKey;
        LPCSTR keyPath = "Environment";
        DWORD createdNewKey;
        LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);

        if (lOpenStatus == ERROR_SUCCESS)
        {
            DWORD valueType;
            char *data = new char[512];
            DWORD dataSize = 512;
            LSTATUS status = RegGetValueA(hKey, NULL, *key, RRF_RT_ANY, &valueType, (PVOID)data, &dataSize);

            RegCloseKey(hKey);

            if (status == ERROR_SUCCESS)
            {
                HLString result(data);
                delete[] data;
                return result;
            }
        }

        return {};
    }

    FileSystemPath FileSystem::GetPersistentStoragePath()
    {
        return utils::GetFolderById(FOLDERID_RoamingAppData);
    }

    FileSystemPath FileSystem::GetUserDocumentsPath()
    {
        return utils::GetFolderById(FOLDERID_Documents);
    }

    FileSystemPath FileSystem::GetUserDownloadsPath()
    {
        return utils::GetFolderById(FOLDERID_Downloads);
    }

    FileSystemPath FileSystem::GetUserDesktopPath()
    {
        return utils::GetFolderById(FOLDERID_Desktop);
    }

    FileSystemPath FileSystem::GetStartMenuPath()
    {
        return utils::GetFolderById(FOLDERID_StartMenu);
    }

    FileSystemPath FileSystem::GetProgamsFilesPath()
    {
        return utils::GetFolderById(FOLDERID_ProgramFiles);
    }

    FileSystemPath FileSystem::GetProgamsX86FilesPath()
    {
        return utils::GetFolderById(FOLDERID_ProgramFilesX86);
    }
}

#endif // HL_PLATFORM_WINDOWS
