// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-11-08) Added FileMenuChanged Event
//     - 1.1 (2021-10-21) Added Threading Events
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "EventBase.h"

#include "Engine/Threading/Thread.h"
#include "Engine/Window/MenuItem.h"

namespace highlo
{
	class WindowCloseEvent : public Event
	{
	public:

		HLAPI WindowCloseEvent() {}

		HLAPI HLString ToString() const override
		{
			return HLString("WindowCloseEvent");
		}

		REGISTER_EVENT_CLASS_TYPE(WindowClose)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowResizeEvent : public Event
	{
	public:

		HLAPI WindowResizeEvent(uint32 width, uint32 height)
			: m_Width(width), m_Height(height) {}

		HLAPI inline uint32 GetWidth() const { return m_Width; }
		HLAPI inline uint32 GetHeight() const { return m_Height; }

		HLAPI HLString ToString() const override
		{
			return HLString("WindowResizeEvent: ") << m_Width << ", " << m_Height;
		}

		REGISTER_EVENT_CLASS_TYPE(WindowResize)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		uint32 m_Width, m_Height;
	};

	class FileMenuEvent : public Event
	{
	public:

		HLAPI FileMenuEvent(int32 id)
			: m_ID(id) {}

		HLAPI inline int32 GetID() const { return m_ID; }

		HLAPI HLString ToString() const override
		{
			return HLString("FileMenuEvent: ") << m_ID;
		}

		REGISTER_EVENT_CLASS_TYPE(FileMenu)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		int32 m_ID;
	};

	class FileMenuChangedEvent : public Event
	{
	public:

		HLAPI FileMenuChangedEvent(Ref<MenuItem> item)
			: m_Item(item) {}

		HLAPI Ref<MenuItem> &GetItem() { return m_Item; }
		HLAPI const Ref<MenuItem> &GetItem() const { return m_Item; }

		HLAPI HLString ToString() const override
		{
			return HLString("FileMenuChangedEvent: ") << m_Item->Name;
		}

		REGISTER_EVENT_CLASS_TYPE(FileMenuChanged)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		Ref<MenuItem> m_Item;
	};

	class FileSystemChangedEvent : public Event
	{
	public:

		HLAPI FileSystemChangedEvent() = default;

		HLAPI FileSystemChangedEvent(const HLString &oldName, const HLString &newName, const HLString &path, FileSystemAction action, bool isDirectory)
			: m_OldName(oldName), m_NewName(newName), m_FilePath(path), m_Action(action), m_IsDirectory(isDirectory) {}

		HLAPI const HLString &GetOldName() const
		{
			return m_OldName;
		}

		HLAPI const HLString &GetNewName() const
		{
			return m_NewName;
		}

		HLAPI const HLString &GetPath() const
		{
			return m_FilePath;
		}

		HLAPI FileSystemAction GetAction()
		{
			return m_Action;
		}

		HLAPI bool IsDirectory() const
		{
			return m_IsDirectory;
		}

		HLAPI void SetAction(FileSystemAction action)
		{
			m_Action = action;
		}

		HLAPI void SetIsDirectory(bool isDirectory)
		{
			m_IsDirectory = isDirectory;
		}

		HLAPI void SetFilePath(const HLString &path)
		{
			m_FilePath = path;
		}

		HLAPI void SetOldName(const HLString &oldName)
		{
			m_OldName = oldName;
		}

		HLAPI void SetNewName(const HLString &newName)
		{
			m_NewName = newName;
		}

		HLAPI HLString ToString() const override
		{
			return HLString("FileSystemChangedEvent: ") << m_OldName << " -> " << m_NewName;
		}

		HLAPI void SetTracking(bool value = true)
		{
			m_Tracking = value;
		}

		HLAPI bool IsTracking() const
		{
			return m_Tracking;
		}

		REGISTER_EVENT_CLASS_TYPE(FileSystemChanged)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		FileSystemAction m_Action = FileSystemAction::Added;
		HLString m_FilePath = "";
		HLString m_OldName = "";
		HLString m_NewName = "";
		bool m_IsDirectory = false;
		bool m_Tracking = false;
	};

	class ThreadStartingEvent : public Event
	{
	public:

		HLAPI ThreadStartingEvent(Thread *thread)
			: m_Thread(thread) {}

		HLAPI HLString ToString() const override
		{
			return HLString("ThreadStarting: " + m_Thread->GetName());
		}

		REGISTER_EVENT_CLASS_TYPE(ThreadStarting)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		Thread *m_Thread;
	};

	class ThreadEndingEvent : public Event
	{
	public:

		HLAPI ThreadEndingEvent(Thread *thread, int32 exitCode)
			: m_Thread(thread), m_ExitCode(exitCode) {}

		HLAPI HLString ToString() const override
		{
			return HLString("ThreadEnding: " + m_Thread->GetName() + " with exit code: " + m_ExitCode);
		}

		REGISTER_EVENT_CLASS_TYPE(ThreadEnding)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		int32 m_ExitCode;
		Thread *m_Thread;
	};
}

