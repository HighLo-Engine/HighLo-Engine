#pragma once
#include "Event.h"

namespace highlo
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		REGISTER_EVT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(KeyReleased)
	};
}

// =========================================================================================== //
// =========================================================================================== //
// ======================================== KEY CODES ======================================== //
// =========================================================================================== //
// =========================================================================================== //

#define HL_KEY_SPACE              32
#define HL_KEY_APOSTROPHE         39  /* ' */
#define HL_KEY_COMMA              44  /* , */
#define HL_KEY_MINUS              45  /* - */
#define HL_KEY_PERIOD             46  /* . */
#define HL_KEY_SLASH              47  /* / */
#define HL_KEY_0                  48
#define HL_KEY_1                  49
#define HL_KEY_2                  50
#define HL_KEY_3                  51
#define HL_KEY_4                  52
#define HL_KEY_5                  53
#define HL_KEY_6                  54
#define HL_KEY_7                  55
#define HL_KEY_8                  56
#define HL_KEY_9                  57
#define HL_KEY_SEMICOLON          59  /* ; */
#define HL_KEY_EQUAL              61  /* = */
#define HL_KEY_A                  65
#define HL_KEY_B                  66
#define HL_KEY_C                  67
#define HL_KEY_D                  68
#define HL_KEY_E                  69
#define HL_KEY_F                  70
#define HL_KEY_G                  71
#define HL_KEY_H                  72
#define HL_KEY_I                  73
#define HL_KEY_J                  74
#define HL_KEY_K                  75
#define HL_KEY_L                  76
#define HL_KEY_M                  77
#define HL_KEY_N                  78
#define HL_KEY_O                  79
#define HL_KEY_P                  80
#define HL_KEY_Q                  81
#define HL_KEY_R                  82
#define HL_KEY_S                  83
#define HL_KEY_T                  84
#define HL_KEY_U                  85
#define HL_KEY_V                  86
#define HL_KEY_W                  87
#define HL_KEY_X                  88
#define HL_KEY_Y                  89
#define HL_KEY_Z                  90
#define HL_KEY_LEFT_BRACKET       91  /* [ */
#define HL_KEY_BACKSLASH          92  /* \ */
#define HL_KEY_RIGHT_BRACKET      93  /* ] */
#define HL_KEY_GRAVE_ACCENT       96  /* ` */
#define HL_KEY_WORLD_1            161 /* non-US #1 */
#define HL_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define HL_KEY_ESCAPE             0x1B
#define HL_KEY_ENTER              0x0D
#define HL_KEY_TAB                0x09
#define HL_KEY_BACKSPACE          0x08
#define HL_KEY_INSERT             0x2D
#define HL_KEY_DELETE             0x2E
#define HL_KEY_RIGHT              0x27
#define HL_KEY_LEFT               0x25
#define HL_KEY_DOWN               0x28
#define HL_KEY_UP                 0x26
#define HL_KEY_PAGE_UP            266
#define HL_KEY_PAGE_DOWN          267
#define HL_KEY_HOME               268
#define HL_KEY_END                269
#define HL_KEY_CAPS_LOCK          0x14
#define HL_KEY_SCROLL_LOCK        281
#define HL_KEY_NUM_LOCK           282
#define HL_KEY_PRINT_SCREEN       283
#define HL_KEY_PAUSE              284
#define HL_KEY_F1                 290
#define HL_KEY_F2                 291
#define HL_KEY_F3                 292
#define HL_KEY_F4                 293
#define HL_KEY_F5                 294
#define HL_KEY_F6                 295
#define HL_KEY_F7                 296
#define HL_KEY_F8                 297
#define HL_KEY_F9                 298
#define HL_KEY_F10                299
#define HL_KEY_F11                300
#define HL_KEY_F12                301
#define HL_KEY_F13                302
#define HL_KEY_F14                303
#define HL_KEY_F15                304
#define HL_KEY_F16                305
#define HL_KEY_F17                306
#define HL_KEY_F18                307
#define HL_KEY_F19                308
#define HL_KEY_F20                309
#define HL_KEY_F21                310
#define HL_KEY_F22                311
#define HL_KEY_F23                312
#define HL_KEY_F24                313
#define HL_KEY_F25                314
#define HL_KEY_KP_0               320
#define HL_KEY_KP_1               321
#define HL_KEY_KP_2               322
#define HL_KEY_KP_3               323
#define HL_KEY_KP_4               324
#define HL_KEY_KP_5               325
#define HL_KEY_KP_6               326
#define HL_KEY_KP_7               327
#define HL_KEY_KP_8               328
#define HL_KEY_KP_9               329
#define HL_KEY_KP_DECIMAL         330
#define HL_KEY_KP_DIVIDE          331
#define HL_KEY_KP_MULTIPLY        332
#define HL_KEY_KP_SUBTRACT        333
#define HL_KEY_KP_ADD             334
#define HL_KEY_KP_ENTER           335
#define HL_KEY_KP_EQUAL           336
#define HL_KEY_LEFT_SHIFT         0xA0
#define HL_KEY_LEFT_CONTROL       17
#define HL_KEY_LEFT_ALT           0x12
#define HL_KEY_LEFT_SUPER         343
#define HL_KEY_RIGHT_SHIFT        0xA1
#define HL_KEY_RIGHT_CONTROL      0xA3
#define HL_KEY_RIGHT_ALT          0xA5
#define HL_KEY_RIGHT_SUPER        347
#define HL_KEY_MENU               348