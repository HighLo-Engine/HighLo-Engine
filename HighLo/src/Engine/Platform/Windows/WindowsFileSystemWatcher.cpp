// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/FileSystemWatcher.h"
#include "Engine/Application/Application.h"
#include "Engine/Scene/Project.h"
#include "Engine/Core/FileSystem.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
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
		FileSystemPath assetDir = Project::GetActive()->GetAssetDirectory();
		wchar_t *wString = assetDir.String().W_Str();
		HL_CORE_TRACE("Listening on path {0}", **assetDir);

		char buffer[2048];
		DWORD bytesReturned;
		FileSystemPath filePath;
		BOOL result = true;

		HANDLE dirHandle = CreateFile(wString, GENERIC_READ | FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
		if (dirHandle == INVALID_HANDLE_VALUE)
		{
			HL_ASSERT(false, "failed to open directory!");
			return 0;
		}

		OVERLAPPED pollingOverlap;
		pollingOverlap.OffsetHigh = 0;
		pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		while (s_Watching && result)
		{
			result = ReadDirectoryChangesW(dirHandle, &buffer, sizeof(buffer), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE, &bytesReturned, &pollingOverlap, NULL);

			WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

			if (s_IgnoreNextChange)
			{
				s_IgnoreNextChange = false;
				result = false;
				continue;
			}

			FILE_NOTIFY_INFORMATION *pNotify;
			int32 offset = 0;
			HLString oldName;

			do
			{
				pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buffer + offset);
				uint32 fileNameLength = pNotify->FileNameLength / (sizeof(wchar_t));

				FileSystemChangedEvent e;
				e.FilePath = FileSystemPath(HLString(pNotify->FileName, fileNameLength));
				e.IsDirectory = FileSystem::Get()->FolderExists(e.FilePath);

				switch (pNotify->Action)
				{
					case FILE_ACTION_ADDED:
					{
						e.Action = FileSystemAction::Added;
						break;
					}

					case FILE_ACTION_REMOVED:
					{
						e.Action = FileSystemAction::Deleted;
						break;
					}

					case FILE_ACTION_MODIFIED:
					{
						e.Action = FileSystemAction::Modified;
						break;
					}

					case FILE_ACTION_RENAMED_OLD_NAME:
					{
						oldName = e.FilePath.Filename();
						break;
					}

					case FILE_ACTION_RENAMED_NEW_NAME:
					{
						e.OldName = oldName;
						e.Action = FileSystemAction::Renamed;
						break;
					}
				}

				if (pNotify->Action != FILE_ACTION_RENAMED_OLD_NAME)
				{
					HL_CORE_TRACE("Triggering FileSystemChangedEvent {0}", *e.ToString());
					HLApplication::Get().GetWindow().GetEventCallback()(e);
				}

				offset += pNotify->NextEntryOffset;

			} while (pNotify->NextEntryOffset);
		}

		CloseHandle(dirHandle);
		return 0;
	}


}

#endif // HL_PLATFORM_WINDOWS

