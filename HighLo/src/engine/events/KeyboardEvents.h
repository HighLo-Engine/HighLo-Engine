#pragma once

#include "EventBase.h"

namespace highlo
{
	class HLAPI KeyEvent : public Event
	{
	public:

		inline int32_t GetKeyCode() const { return m_KeyCode; }

		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int32_t keyCode)
			: m_KeyCode(keyCode) {}

		int32_t m_KeyCode;
	};

	class HLAPI KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int32_t keyCode, int32_t repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int32_t GetRepeatCount() { return m_RepeatCount; }

		HLString ToString() const override
		{
			return HLString("KeyPressedEvent: ") << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		}

		REGISTER_EVENT_CLASS_TYPE(KeyPressed)

	private:
		int32_t m_RepeatCount;
	};

	class HLAPI KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32_t keyCode)
			: KeyEvent(keyCode) {}

		HLString ToString() const override
		{
			return HLString("KeyReleasedEvent") << m_KeyCode;
		}

		REGISTER_EVENT_CLASS_TYPE(KeyReleased)
	};
}
