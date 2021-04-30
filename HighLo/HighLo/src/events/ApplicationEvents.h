#pragma once

#include "EventBase.h"

namespace highlo
{
	class HLAPI WindowCloseEvent : public Event
	{
	public:

		WindowCloseEvent() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		REGISTER_EVENT_CLASS_TYPE(WindowClose)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class HLAPI WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		REGISTER_EVENT_CLASS_TYPE(WindowResize)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		uint32_t m_Width, m_Height;
	};
}