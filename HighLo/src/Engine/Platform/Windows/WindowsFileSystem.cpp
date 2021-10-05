// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/FileSystem.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include <shellapi.h>
#include <fileapi.h>

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
    }

    bool FileSystem::FileExists(const FileSystemPath &path)
    {
        HLString pathStr = path.String();
        if (pathStr.IsEmpty())
            return false;

        DWORD result = GetFileAttributesW(pathStr.W_Str());
        return !(result == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
    }

    bool FileSystem::PathExists(const FileSystemPath &path)
    {
        HLString pathStr = path.String();
        if (pathStr.IsEmpty())
            return false;

        DWORD result = GetFileAttributesW(pathStr.W_Str());
        return (result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool FileSystem::RemoveFile(const FileSystemPath &path)
    {
        FileSystemWatcher::Get()->DisableWatchUntilNextAction();

        char *fp = (char*)*path.String();
        fp[strlen(fp)] = '\0';
        SHFILEOPSTRUCTA file_op;
        file_op.hwnd = NULL;
        file_op.wFunc = FO_DELETE;
        file_op.pFrom = fp;
        file_op.pTo = "";
        file_op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
        file_op.fAnyOperationsAborted = false;
        file_op.hNameMappings = 0;
        file_op.lpszProgressTitle = "";
        return SHFileOperationA(&file_op) == 0;
    }

    int64 FileSystem::GetFileSize(const FileSystemPath &path)
    {
        HANDLE file = utils::OpenFileInternal(path.String());
        if (file == INVALID_HANDLE_VALUE)
            return -1;

        int64 size = utils::GetFileSizeInternal(file);
        CloseHandle(file);
        return size;
    }

    bool FileSystem::CreateFolder(const FileSystemPath &path)
    {
        return CreateDirectoryW(path.String().W_Str(), NULL);
    }

    bool FileSystem::RemoveFolder(const FileSystemPath &path)
    {
        return RemoveDirectoryW(path.String().W_Str());
    }

    HLString FileSystem::Rename(const FileSystemPath &path, const FileSystemPath &newName)
    {
        FileSystemWatcher::Get()->DisableWatchUntilNextAction();
        HLString newFilePath = path.ParentPath().String() + "/" + newName.String() + path.GetFile()->GetExtension();
        MoveFileA(*path.String(), *newFilePath);
        return newFilePath;
    }

    bool FileSystem::Move(const FileSystemPath &filePath, const FileSystemPath &dest)
    {
        FileSystemWatcher::Get()->DisableWatchUntilNextAction();
        HLString destFilePath = dest.String() + "/" + HLString(filePath.GetFile()->GetFullName());
        BOOL result = MoveFileA(*filePath.String(), *destFilePath);
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

        *outSize = size;
        return result ? buffer : nullptr;
    }

    HLString FileSystem::ReadTextFile(const FileSystemPath &path)
    {
        HANDLE file = utils::OpenFileInternal(path.String());
        uint32 size = (uint32)utils::GetFileSizeInternal(file);

        HLString result;
        result.Resize(size);

        bool success = utils::ReadFileInternal(file, &result[0], size);
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
        ShellExecuteW(0, L"open", path.String().W_Str(), 0, 0, SW_SHOWDEFAULT);
    }

    void FileSystem::OpenInBrowser(const HLString &url)
    {
        ShellExecuteW(0, L"explore", url.W_Str(), 0, 0, SW_SHOW);
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
}

#endif // HL_PLATFORM_WINDOWS
