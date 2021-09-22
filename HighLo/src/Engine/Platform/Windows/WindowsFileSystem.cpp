// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/FileSystem.h"

#ifdef HL_PLATFORM_WINDOWS

#include <filesystem>
#include <Windows.h>
#include <shellapi.h>
#include <fileapi.h>

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

    FileSystem::FileSystemChangedCallbackFn FileSystem::s_Callback;

    static bool s_Watching = false;
    static bool s_IgnoreNextChange = false;
    static HANDLE s_WatcherThread;

    bool FileSystem::FileExists(const HLString &path)
    {
        DWORD result = GetFileAttributesW(path.W_Str());
        return !(result == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
    }

    bool FileSystem::PathExists(const HLString &path)
    {
        DWORD result = GetFileAttributesW(path.W_Str());
        return (result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool FileSystem::RemoveFile(const HLString &path)
    {
        s_IgnoreNextChange = true;
        std::string fp = *path;
        fp.append(1, '\0');
        SHFILEOPSTRUCTA file_op;
        file_op.hwnd = NULL;
        file_op.wFunc = FO_DELETE;
        file_op.pFrom = fp.c_str();
        file_op.pTo = "";
        file_op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
        file_op.fAnyOperationsAborted = false;
        file_op.hNameMappings = 0;
        file_op.lpszProgressTitle = "";
        int result = SHFileOperationA(&file_op);
        s_IgnoreNextChange = false;
        return result == 0;
    }

    int64 FileSystem::GetFileSize(const HLString &path)
    {
        HANDLE file = utils::OpenFileInternal(path);
        if (file == INVALID_HANDLE_VALUE)
            return -1;

        int64 size = utils::GetFileSizeInternal(file);
        CloseHandle(file);
        return size;
    }

    bool FileSystem::CreateFolder(const HLString &path)
    {
        return CreateDirectoryW(path.W_Str(), NULL);
    }

    bool FileSystem::RemoveFolder(const HLString &path)
    {
        return RemoveDirectoryW(path.W_Str());
    }

    HLString FileSystem::Rename(const HLString &path, const HLString &newName)
    {
        s_IgnoreNextChange = true;
        std::filesystem::path p = *path;
        HLString newFilePath = p.parent_path().string() + "/" + newName + p.extension().string();
        MoveFileA(*path, *newFilePath);
        s_IgnoreNextChange = false;
        return newFilePath;
    }

    bool FileSystem::Move(const HLString &filePath, const HLString &dest)
    {
        s_IgnoreNextChange = true;
        std::filesystem::path p = *filePath;
        HLString destFilePath = dest + "/" + HLString(p.filename().string());
        BOOL result = MoveFileA(*filePath, *destFilePath);
        s_IgnoreNextChange = false;
        return result != 0;
    }

    Byte *FileSystem::ReadFile(const HLString &path, int64 *outSize)
    {
        HANDLE file = utils::OpenFileInternal(path);
        int64 size = utils::GetFileSizeInternal(file);
        Byte *buffer = new Byte[size];
        bool result = utils::ReadFileInternal(file, buffer, size);
        CloseHandle(file);

        if (!result)
            delete[] buffer;

        *outSize = size;
        return result ? buffer : nullptr;
    }

    HLString FileSystem::ReadTextFile(const HLString &path)
    {
        HANDLE file = utils::OpenFileInternal(path);
        uint32 size = (uint32)utils::GetFileSizeInternal(file);

        HLString result;
        result.Resize(size);

        bool success = utils::ReadFileInternal(file, &result[0], size);
        CloseHandle(file);
        return success ? result : "";
    }

    bool FileSystem::WriteFile(const HLString &path, Byte *buffer, int64 size)
    {
        HANDLE file = CreateFileW(path.W_Str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE)
            return false;

        DWORD written;
        bool result = ::WriteFile(file, buffer, (DWORD)size, &written, NULL);
        CloseHandle(file);
        return result;
    }

    bool FileSystem::WriteTextFile(const HLString &path, const HLString &text)
    {
        return WriteFile(path, (Byte*)&text[0], (int64)text.Length());
    }

    void FileSystem::OpenInExplorer(const HLString &path)
    {
        ShellExecuteW(0, L"open", path.W_Str(), 0, 0, SW_SHOWDEFAULT);
    }

    void FileSystem::OpenInBrowser(const HLString &url)
    {
        ShellExecuteW(0, 0, url.W_Str(), 0, 0, SW_SHOW);
    }

    void FileSystem::SetChangeCallback(const FileSystemChangedCallbackFn &callback)
    {
        s_Callback = callback;
    }

    void FileSystem::StartWatching()
    {
        DWORD threadId;
        s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadId);
        HL_ASSERT(s_WatcherThread != NULL);
    }

    void FileSystem::StopWatching()
    {
        s_Watching = false;
        DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
        if (result == WAIT_TIMEOUT)
            TerminateThread(s_WatcherThread, 0);
        CloseHandle(s_WatcherThread);
    }

    unsigned long FileSystem::Watch(void *param)
    {
        LPCWSTR	filepath = L"assets"; // TODO: make this a parameter to be able to watch different folders as well
        std::vector<BYTE> buffer;
        buffer.resize(10 * 1024);
        OVERLAPPED overlapped = { 0 };
        HANDLE handle = NULL;
        DWORD bytesReturned = 0;

        handle = CreateFileW(
            filepath,
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL);

        ZeroMemory(&overlapped, sizeof(overlapped));

        if (handle == INVALID_HANDLE_VALUE)
            HL_CORE_ERROR("Unable to accquire directory handle: {0}", GetLastError());

        overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        if (overlapped.hEvent == NULL)
        {
            HL_CORE_ERROR("CreateEvent failed!");
            return 0;
        }

        while (s_Watching)
        {
            DWORD status = ReadDirectoryChangesW(
                handle,
                &buffer[0],
                (DWORD)buffer.size(),
                TRUE,
                FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME,
                &bytesReturned,
                &overlapped,
                NULL);

            if (!status)
                HL_CORE_ERROR(GetLastError());

            DWORD waitOperation = WaitForSingleObject(overlapped.hEvent, 5000);
            if (waitOperation != WAIT_OBJECT_0)
                continue;

            if (s_IgnoreNextChange)
                continue;

            std::string oldName;
            char fileName[MAX_PATH * 10] = "";

            BYTE *buf = buffer.data();
            for (;;)
            {
                FILE_NOTIFY_INFORMATION &fni = *(FILE_NOTIFY_INFORMATION *) buf;
                ZeroMemory(fileName, sizeof(fileName));
                WideCharToMultiByte(CP_ACP, 0, fni.FileName, fni.FileNameLength / sizeof(WCHAR), fileName, sizeof(fileName), NULL, NULL);
                std::filesystem::path filepath = "assets/" + std::string(fileName);

                FileSystemChangedEvent e;
                e.SetFilePath(filepath.string());
                e.SetNewName(filepath.filename().string());
                e.SetOldName(filepath.filename().string());
                e.SetIsDirectory(std::filesystem::is_directory(filepath));

                switch (fni.Action)
                {
                    case FILE_ACTION_ADDED:
                    {
                        e.SetAction(FileSystemAction::Added);
                        s_Callback(e);
                        break;
                    }

                    case FILE_ACTION_REMOVED:
                    {
                    //  e.SetIsDirectory(AssetManager::IsDirectory(e.GetFilePath())); // @TODO
                        e.SetAction(FileSystemAction::Deleted);
                        s_Callback(e);
                        break;
                    }

                    case FILE_ACTION_MODIFIED:
                    {
                        e.SetAction(FileSystemAction::Modified);
                        s_Callback(e);
                        break;
                    }

                    case FILE_ACTION_RENAMED_OLD_NAME:
                    {
                        oldName = filepath.filename().string();
                        break;
                    }

                    case FILE_ACTION_RENAMED_NEW_NAME:
                    {
                        e.SetOldName(oldName);
                        e.SetAction(FileSystemAction::Renamed);
                        s_Callback(e);
                        break;
                    }
                }

                if (!fni.NextEntryOffset)
                    break;

                buf += fni.NextEntryOffset;
            }
        }

        return 0;
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
