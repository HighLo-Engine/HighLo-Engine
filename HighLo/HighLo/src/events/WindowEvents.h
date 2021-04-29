#pragma once
#include "Event.h"

namespace highlo
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		REGISTER_EVT_CLASS_TYPE(WindowClose)
		REGISTER_EVT_CATEGORY(EventCategoryWindow)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Closed";
			return ss.str();
		}
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(WindowResize)
		REGISTER_EVT_CATEGORY(EventCategoryWindow)

	private:
		unsigned int m_Width, m_Height;
	};
}
