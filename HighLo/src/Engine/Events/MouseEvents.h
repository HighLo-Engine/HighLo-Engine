// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "EventBase.h"

namespace highlo
{
	class MouseMovedEvent : public Event
	{
	public:

		HLAPI MouseMovedEvent(float xPos, float yPos, float dxPos, float dyPos)
			: m_MouseX(xPos), m_MouseY(yPos), m_MouseDX(dxPos), m_MouseDY(dyPos) {}

		HLAPI inline float GetMouseX() const { return m_MouseX; }
		HLAPI inline float GetMouseY() const { return m_MouseY; }
		HLAPI inline float GetMouseDX() const { return m_MouseDX; }
		HLAPI inline float GetMouseDY() const { return m_MouseDY; }

		HLAPI HLString ToString() const override
		{
			return HLString("MouseMovedEvent: ") + HLString::ToString(m_MouseDX) + ", " + HLString::ToString(m_MouseDY);
		}

		REGISTER_EVENT_CLASS_TYPE(MouseMoved)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:

		float m_MouseX, m_MouseY;
		float m_MouseDX, m_MouseDY;
	};

	class MouseScrolledEvent : public Event
	{
	public:

		HLAPI MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		HLAPI inline float GetXOffset() const { return m_XOffset; }
		HLAPI inline float GetYOffset() const { return m_YOffset; }

		HLAPI HLString ToString() const override
		{
			return HLString("MouseScrolledEvent: ") + HLString::ToString(m_XOffset) + ", " + HLString::ToString(m_YOffset);
		}

		REGISTER_EVENT_CLASS_TYPE(MouseScrolled)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:

		float m_XOffset, m_YOffset;
	};

	class MouseButton : public Event
	{
	public:
		
		HLAPI inline int32 GetMouseButton() const { return m_MouseButton; }

		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)

	protected:

		HLAPI MouseButton(int32 mouseButton)
			: m_MouseButton(mouseButton) {}

		int32 m_MouseButton;
	};

	class MouseButtonPressedEvent : public MouseButton
	{
	public:
		
		HLAPI MouseButtonPressedEvent(int32 mouseButton)
			: MouseButton(mouseButton) {}

		HLAPI HLString ToString() const override
		{
			return HLString("MouseButtonPressedEvent: ") + HLString::ToString(m_MouseButton);
		}

		REGISTER_EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButton
	{
	public:

		HLAPI MouseButtonReleasedEvent(int32 mouseButton)
			: MouseButton(mouseButton) {}

		HLAPI HLString ToString() const override
		{
			return HLString("MouseButtonReleasedEvent") + HLString::ToString(m_MouseButton);
		}

		REGISTER_EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
