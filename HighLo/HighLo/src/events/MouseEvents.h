#pragma once
#include "Event.h"

namespace highlo
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y, float dx, float dy)
			: m_MouseX(x), m_MouseY(y), m_MouseDX(dx), m_MouseDY(dy) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }
		inline float GetDX() const { return m_MouseDX; }
		inline float GetDY() const { return m_MouseDY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseDX << ", " << m_MouseDY;
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseMoved)
			REGISTER_EVT_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY, m_MouseDX, m_MouseDY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseScrolled)
			REGISTER_EVT_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	enum class MouseButton
	{
		Left, Right, Middle
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton GetMouseButton() const { return m_Button; }

		REGISTER_EVT_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(MouseButton button)
			: m_Button(button) {}

		MouseButton m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << (int)m_Button;
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << (int)m_Button;
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseButtonReleased)
	};
}
