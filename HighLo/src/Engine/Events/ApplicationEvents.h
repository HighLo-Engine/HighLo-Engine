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
}