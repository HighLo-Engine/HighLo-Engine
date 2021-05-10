#pragma once

#include "EventBase.h"

namespace highlo
{
	class HLAPI WindowCloseEvent : public Event
	{
	public:

		WindowCloseEvent() {}

		HLString ToString() const override
		{
			return HLString("WindowCloseEvent");
		}

		REGISTER_EVENT_CLASS_TYPE(WindowClose)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class HLAPI WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(uint32 width, uint32 height)
			: m_Width(width), m_Height(height) {}

		inline uint32 GetWidth() const { return m_Width; }
		inline uint32 GetHeight() const { return m_Height; }

		HLString ToString() const override
		{
			return HLString("WindowResizeEvent: ") << m_Width << ", " << m_Height;
		}

		REGISTER_EVENT_CLASS_TYPE(WindowResize)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		uint32 m_Width, m_Height;
	};

	class HLAPI FileMenuEvent : public Event
	{
	public:

		FileMenuEvent(int32 id)
			: m_ID(id) {}

		inline int32 GetID() const { return m_ID; }

		HLString ToString() const override
		{
			return HLString("FileMenuEvent: ") << m_ID;
		}

		REGISTER_EVENT_CLASS_TYPE(FileMenu)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		int32 m_ID;
	};

	class HLAPI FileSystemChangedEvent : public Event
	{
	public:

		FileSystemChangedEvent() = default;

		FileSystemChangedEvent(const HLString &oldName, const HLString &newName, const HLString &path, FileSystemAction action, bool isDirectory)
			: m_OldName(oldName), m_NewName(newName), m_FilePath(path), m_Action(action), m_IsDirectory(isDirectory)
		{
		}

		const HLString &GetOldName() const
		{
			return m_OldName;
		}

		const HLString &GetNewName() const
		{
			return m_NewName;
		}

		const HLString &GetPath() const
		{
			return m_FilePath;
		}

		FileSystemAction GetAction()
		{
			return m_Action;
		}

		bool IsDirectory() const
		{
			return m_IsDirectory;
		}

		void SetAction(FileSystemAction action)
		{
			m_Action = action;
		}

		void SetIsDirectory(bool isDirectory)
		{
			m_IsDirectory = isDirectory;
		}

		void SetFilePath(const HLString &path)
		{
			m_FilePath = path;
		}

		void SetOldName(const HLString &oldName)
		{
			m_OldName = oldName;
		}

		void SetNewName(const HLString &newName)
		{
			m_NewName = newName;
		}

		HLString ToString() const override
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