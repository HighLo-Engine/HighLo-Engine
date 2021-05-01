#include "HighLoPch.h"
#include "Engine/Core/HLFileSystem.h"

#ifdef HL_PLATFORM_WINDOWS

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
        return DeleteFileW(path.W_Str());
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
        int64 size = utils::GetFileSizeInternal(file);

        HLString result;
        result.Resize(size);

        bool success = utils::ReadFileInternal(file, &result[0], size);
        CloseHandle(file);
        return success ? result : HLString("");
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
}

#endif // HL_PLATFORM_WINDOWS
