// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/FileSystemWatcher.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	FileSystemWatcher::FileSystemChangedCallbackFn FileSystemWatcher::s_Callback;

	static bool s_Watching = false;
    static bool s_IgnoreNextChange = false;
	static HANDLE s_WatcherThread;
    static HLString s_WatchPath = "";

	void FileSystemWatcher::Start(const HLString &filePath)
	{
		DWORD threadId;
        s_WatchPath = filePath;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadId);
		HL_ASSERT(s_WatcherThread != NULL);
	}

	void FileSystemWatcher::Stop()
	{
		s_Watching = false;
		DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
		if (result == WAIT_TIMEOUT)
			TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
	}

    void FileSystemWatcher::SetChangeCallback(const FileSystemChangedCallbackFn &callback)
    {
        s_Callback = callback;
    }

    void FileSystemWatcher::SetWatchPath(const HLString &filePath)
    {
        s_WatchPath = filePath;

        Stop();
        Start(filePath);
    }

    void FileSystemWatcher::DisableWatchUntilNextAction()
    {
        s_IgnoreNextChange = true;
    }

	ULONG FileSystemWatcher::Watch(void *param)
	{
        HLString filePath = s_WatchPath;
        std::vector<BYTE> buffer;
        buffer.resize(10 * 1024);
        OVERLAPPED overlapped = { 0 };
        HANDLE handle = NULL;
        DWORD bytesReturned = 0;

        handle = CreateFileW(
            filePath.W_Str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL);

        ZeroMemory(&overlapped, sizeof(overlapped));

        if (handle == INVALID_HANDLE_VALUE)
            HL_CORE_ERROR("Unable to accquire directory handle: {0}", GetLastError());

        overlapped.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

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
            {
                s_IgnoreNextChange = false;
                continue;
            }

            std::string oldName;
            char fileName[MAX_PATH * 10] = "";

            BYTE *buf = buffer.data();
            for (;;)
            {
                FILE_NOTIFY_INFORMATION &fni = *(FILE_NOTIFY_INFORMATION*)buf;
                ZeroMemory(fileName, sizeof(fileName));
                WideCharToMultiByte(CP_ACP, 0, fni.FileName, fni.FileNameLength / sizeof(WCHAR), fileName, sizeof(fileName), NULL, NULL);
                FileSystemPath filePath = "assets/" + HLString(fileName);

                FileSystemChangedEvent e;
                e.SetFilePath(filePath.String());
                e.SetNewName(filePath.GetFile()->GetName());
                e.SetOldName(filePath.GetFile()->GetName());
                e.SetIsDirectory(filePath.GetFile()->IsDirectory());

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
                        oldName = filePath.GetFile()->GetName();
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


}

#endif // HL_PLATFORM_WINDOWS

