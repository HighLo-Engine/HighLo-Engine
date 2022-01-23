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
#include "Engine/Core/FileSystemPath.h"

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

		FileSystemAction Action = FileSystemAction::None;
		FileSystemPath FilePath;
		HLString OldName;
		HLString NewName;
		bool IsDirectory = false;
		bool Tracking = false;

		HLAPI FileSystemChangedEvent() = default;

		HLAPI FileSystemChangedEvent(const HLString &oldName, const HLString &newName, const HLString &path, FileSystemAction action, bool isDirectory)
			: OldName(oldName), NewName(newName), FilePath(path), Action(action), IsDirectory(isDirectory) {}

		HLAPI HLString ToString() const override
		{
			if (!OldName.IsEmpty())
			{
				return HLString("FileSystemChangedEvent: ") << OldName << " -> " << NewName;
			}

			return HLString("FileSystemChangedEvent: ") << NewName;
		}

		REGISTER_EVENT_CLASS_TYPE(FileSystemChanged)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)
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

		HLAPI Thread *GetThread() const
		{
			return m_Thread;
		}

		REGISTER_EVENT_CLASS_TYPE(ThreadStarting)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		Thread *m_Thread = nullptr;
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

		HLAPI Thread *GetThread() const
		{
			return m_Thread;
		}

		HLAPI int32 GetExitCode() const
		{
			return m_ExitCode;
		}

		REGISTER_EVENT_CLASS_TYPE(ThreadEnding)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		int32 m_ExitCode = 0;
		Thread *m_Thread = nullptr;
	};
}

