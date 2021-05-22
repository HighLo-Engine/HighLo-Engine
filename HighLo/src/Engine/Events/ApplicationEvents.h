#pragma once

#include "EventBase.h"

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

	class FileSystemChangedEvent : public Event
	{
	public:

		HLAPI FileSystemChangedEvent() = default;

		HLAPI FileSystemChangedEvent(const HLString &oldName, const HLString &newName, const HLString &path, FileSystemAction action, bool isDirectory)
			: m_OldName(oldName), m_NewName(newName), m_FilePath(path), m_Action(action), m_IsDirectory(isDirectory)
		{
		}

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

		REGISTER_EVENT_CLASS_TYPE(FileSystemChanged)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		FileSystemAction m_Action = FileSystemAction::Added;
		HLString m_FilePath = "";
		HLString m_OldName = "";
		HLString m_NewName = "";
		bool m_IsDirectory = false;
	};
}