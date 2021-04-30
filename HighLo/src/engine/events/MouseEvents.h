#pragma once

#include "EventBase.h"

namespace highlo
{
	class HLAPI MouseMovedEvent : public Event
	{
	public:

		MouseMovedEvent(float xPos, float yPos, float dxPos, float dyPos)
			: m_MouseX(xPos), m_MouseY(yPos), m_MouseDX(dxPos), m_MouseDY(dyPos) {}

		inline float GetMouseX() const { return m_MouseX; }
		inline float GetMouseY() const { return m_MouseY; }
		inline float GetMouseDX() const { return m_MouseDX; }
		inline float GetMouseDY() const { return m_MouseDY; }

		HLString ToString() const override
		{
			return HLString("MouseMovedEvent: ") << m_MouseDX << ", " << m_MouseDY;
		}

		REGISTER_EVENT_CLASS_TYPE(MouseMoved)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY;
		float m_MouseDX, m_MouseDY;
	};

	class HLAPI MouseScrolledEvent : public Event
	{
	public:

		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		HLString ToString() const override
		{
			return HLString("MouseScrolledEvent: ") << m_XOffset << ", " << m_YOffset;
		}

		REGISTER_EVENT_CLASS_TYPE(MouseScrolled)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	class HLAPI MouseButton : public Event
	{
	public:
		
		inline int32_t GetMouseButton() const { return m_MouseButton; }

		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)

	protected:

		MouseButton(int32_t mouseButton)
			: m_MouseButton(mouseButton) {}

		int32_t m_MouseButton;
	};

	class HLAPI MouseButtonPressedEvent : public MouseButton
	{
	public:
		
		MouseButtonPressedEvent(int32 mouseButton)
			: MouseButton(mouseButton) {}

		HLString ToString() const override
		{
			return HLString("MouseButtonPressedEvent: ") << m_MouseButton;
		}

		REGISTER_EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class HLAPI MouseButtonReleasedEvent : public MouseButton
	{
	public:

		MouseButtonReleasedEvent(int32_t mouseButton)
			: MouseButton(mouseButton) {}

		HLString ToString() const override
		{
			return HLString("MouseButtonReleasedEvent") << m_MouseButton;
		}

		REGISTER_EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}