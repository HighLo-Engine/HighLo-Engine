// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "EventBase.h"

namespace highlo
{
	class KeyEvent : public Event
	{
	public:

		HLAPI inline int32 GetKeyCode() const { return m_KeyCode; }

		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		HLAPI KeyEvent(int32_t keyCode)
			: m_KeyCode(keyCode) {}

		int32 m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:

		HLAPI KeyPressedEvent(int32 keyCode, int32 repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		HLAPI inline int32 GetRepeatCount() { return m_RepeatCount; }

		HLAPI HLString ToString() const override
		{
			return HLString("KeyPressedEvent: ") + HLString::ToString(m_KeyCode) + " (" + HLString::ToString(m_RepeatCount) + " repeats)";
		}

		REGISTER_EVENT_CLASS_TYPE(KeyPressed)

	private:
		int32 m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		
		HLAPI KeyReleasedEvent(int32 keyCode)
			: KeyEvent(keyCode) {}

		HLAPI HLString ToString() const override
		{
			return HLString("KeyReleasedEvent: ") + HLString::ToString(m_KeyCode);
		}

		REGISTER_EVENT_CLASS_TYPE(KeyReleased)
	};
}
