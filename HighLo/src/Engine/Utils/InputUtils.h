#pragma once

#include "Engine/Core/Defines/KeyCodes.h"

namespace highlo
{
	namespace utils
	{
		/**
		  *
		  * Helper function to convert Windows KeyCodes to the Highlo-Engine specific keyCodes.
		  *
		  * @param keyCode The keyCode that should be converted.
		  * @return Returns the equivalent Highlo-Engine specific KeyCode.
		  */
		static int32 InputHelperWindowsKeyCodeToHL(int32 keyCode)
		{
			switch (keyCode)
			{
				case 0x41: return HL_KEY_A;				// A
				case 0x42: return HL_KEY_B;				// B
				case 0x43: return HL_KEY_C;				// C
				case 0x44: return HL_KEY_D;				// D
				case 0x45: return HL_KEY_E;				// E
				case 0x46: return HL_KEY_F;				// F
				case 0x47: return HL_KEY_G;				// G
				case 0x48: return HL_KEY_H;				// H
				case 0x49: return HL_KEY_I;				// I
				case 0x4A: return HL_KEY_J;				// J
				case 0x4B: return HL_KEY_K;				// K
				case 0x4C: return HL_KEY_L;				// L
				case 0x4D: return HL_KEY_M;				// M
				case 0x4E: return HL_KEY_N;				// N
				case 0x4F: return HL_KEY_O;				// O
				case 0x50: return HL_KEY_P;				// P
				case 0x51: return HL_KEY_Q;				// Q
				case 0x52: return HL_KEY_R;				// R
				case 0x53: return HL_KEY_S;				// S
				case 0x54: return HL_KEY_T;				// T
				case 0x55: return HL_KEY_U;				// U
				case 0x56: return HL_KEY_V;				// V
				case 0x57: return HL_KEY_W;				// W
				case 0x58: return HL_KEY_X;				// X
				case 0x59: return HL_KEY_Y;				// Y
				case 0x5A: return HL_KEY_Z;				// Z

				case 0x1B: return HL_KEY_ESCAPE;		// Escape
				case 0x09: return HL_KEY_TAB;			// Tab
				case 0x0D: return HL_KEY_ENTER;			// Enter
				case 0x10: return HL_KEY_LEFT_SHIFT;	// Shift
				case 0x11: return HL_KEY_LEFT_CONTROL;	// Control
				case 0x14: return HL_KEY_CAPS_LOCK;		// Caps Lock
				case 0x90: return HL_KEY_NUM_LOCK;		// Numpad Lock
				case 0x20: return HL_KEY_SPACE;			// Space
				case 0x08: return HL_KEY_BACKSPACE;		// Backspace
				case 0x21: return HL_KEY_PAGE_UP;		// Page up
				case 0x22: return HL_KEY_PAGE_DOWN;		// Page down
				case 0x2C: return HL_KEY_PRINT_SCREEN;	// Print Screen (Screenshot) KEy
				case 0x13: return HL_KEY_PAUSE;			// Pause Key
				case 0x23: return HL_KEY_END;			// End Key
				case 0x24: return HL_KEY_HOME;			// Home Key
				case 0x2D: return HL_KEY_INSERT;		// Insert Key
				case 0x2E: return HL_KEY_DELETE;		// Delete Key
				case 0x5B: return HL_KEY_LEFT_SUPER;	// Left Windows Key
				case 0x5C: return HL_KEY_RIGHT_SUPER;	// Right Windows Key
				case 0x25: return HL_KEY_LEFT;			// Left
				case 0x26: return HL_KEY_UP;			// Up
				case 0x27: return HL_KEY_RIGHT;			// Right
				case 0x28: return HL_KEY_DOWN;			// Down
				case 0xBC: return HL_KEY_COMMA;			// , Key
				case 0xBD: return HL_KEY_MINUS;			// - Key
				case 0xBE: return HL_KEY_PERIOD;		// . Key

				case 0x30: return HL_KEY_0;				// 0
				case 0x31: return HL_KEY_1;				// 1
				case 0x32: return HL_KEY_2;				// 2
				case 0x33: return HL_KEY_3;				// 3
				case 0x34: return HL_KEY_4;				// 4
				case 0x35: return HL_KEY_5;				// 5
				case 0x36: return HL_KEY_6;				// 6
				case 0x37: return HL_KEY_7;				// 7
				case 0x38: return HL_KEY_8;				// 8
				case 0x39: return HL_KEY_9;				// 9

				case 0x60: return HL_KEY_KP_0;			// Numpad 0
				case 0x61: return HL_KEY_KP_1;			// Numpad 1
				case 0x62: return HL_KEY_KP_2;			// Numpad 2
				case 0x63: return HL_KEY_KP_3;			// Numpad 3
				case 0x64: return HL_KEY_KP_4;			// Numpad 4
				case 0x65: return HL_KEY_KP_5;			// Numpad 5
				case 0x66: return HL_KEY_KP_6;			// Numpad 6
				case 0x67: return HL_KEY_KP_7;			// Numpad 7
				case 0x68: return HL_KEY_KP_8;			// Numpad 8
				case 0x69: return HL_KEY_KP_9;			// Numpad 9
				case 0x6A: return HL_KEY_KP_MULTIPLY;	// Numpad Multiply
				case 0x6B: return HL_KEY_KP_ADD;		// Numpad Add
				case 0x6D: return HL_KEY_KP_SUBTRACT;	// Numpad Subtract
				case 0x6E: return HL_KEY_KP_DECIMAL;	// Numpad Decimal
				case 0x6F: return HL_KEY_KP_DIVIDE;		// Numpad Divide

				case 0x70: return HL_KEY_F1;			// F1
				case 0x71: return HL_KEY_F2;			// F2
				case 0x72: return HL_KEY_F3;			// F3
				case 0x73: return HL_KEY_F4;			// F4
				case 0x74: return HL_KEY_F5;			// F5
				case 0x75: return HL_KEY_F6;			// F6
				case 0x76: return HL_KEY_F7;			// F7
				case 0x77: return HL_KEY_F8;			// F8
				case 0x78: return HL_KEY_F9;			// F9
				case 0x79: return HL_KEY_F10;			// F10
				case 0x7A: return HL_KEY_F11;			// F11
				case 0x7B: return HL_KEY_F12;			// F12
				case 0x7C: return HL_KEY_F13;			// F13
				case 0x7D: return HL_KEY_F14;			// F14
				case 0x7E: return HL_KEY_F15;			// F15
				case 0x7F: return HL_KEY_F16;			// F16
				case 0x80: return HL_KEY_F17;			// F17
				case 0x81: return HL_KEY_F18;			// F18
				case 0x82: return HL_KEY_F19;			// F19
				case 0x83: return HL_KEY_F20;			// F20
				case 0x84: return HL_KEY_F21;			// F21
				case 0x85: return HL_KEY_F22;			// F22
				case 0x86: return HL_KEY_F23;			// F23
				case 0x87: return HL_KEY_F24;			// F24
			}

			return -1;
		}

		/**
		 *
		 * Helper function to convert Highlo-Engine KeyCodes to Windows specific keyCodes.
		 *
		 * @param keyCode The keyCode that should be converted.
		 * @return Returns the equivalent Windows specific KeyCode.
		 */
		static int32 InputHelperHLKeyCodeToWindows(int32 keyCode)
		{
			switch (keyCode)
			{
				case HL_KEY_A: return 0x41;				// A
				case HL_KEY_B: return 0x42;				// B
				case HL_KEY_C: return 0x43;				// C
				case HL_KEY_D: return 0x44;				// D
				case HL_KEY_E: return 0x45;				// E
				case HL_KEY_F: return 0x46;				// F
				case HL_KEY_G: return 0x47;				// G
				case HL_KEY_H: return 0x48;				// H
				case HL_KEY_I: return 0x49;				// I
				case HL_KEY_J: return 0x4A;				// J
				case HL_KEY_K: return 0x4B;				// K
				case HL_KEY_L: return 0x4C;				// L
				case HL_KEY_M: return 0x4D;				// M
				case HL_KEY_N: return 0x4E;				// N
				case HL_KEY_O: return 0x4F;				// O
				case HL_KEY_P: return 0x50;				// P
				case HL_KEY_Q: return 0x51;				// Q
				case HL_KEY_R: return 0x52;				// R
				case HL_KEY_S: return 0x53;				// S
				case HL_KEY_T: return 0x54;				// T
				case HL_KEY_U: return 0x55;				// U
				case HL_KEY_V: return 0x56;				// V
				case HL_KEY_W: return 0x57;				// W
				case HL_KEY_X: return 0x58;				// X
				case HL_KEY_Y: return 0x59;				// Y
				case HL_KEY_Z: return 0x5A;				// Z

				case HL_KEY_ESCAPE: return 0x1B;		// Escape
				case HL_KEY_TAB: return 0x09;			// Tab
				case HL_KEY_ENTER: return 0x0D;			// Enter
				case HL_KEY_CAPS_LOCK: return 0x14;		// Caps Lock
				case HL_KEY_NUM_LOCK: return 0x90;		// Numpad Lock
				case HL_KEY_SPACE: return 0x20;			// Space
				case HL_KEY_BACKSPACE: return 0x08;		// Backspace
				case HL_KEY_PAGE_UP: return 0x21;		// Page up
				case HL_KEY_PAGE_DOWN: return 0x22;		// Page down
				case HL_KEY_PRINT_SCREEN: return 0x2C;	// Print Screen (Screenshot) KEy
				case HL_KEY_PAUSE: return 0x13;			// Pause Key
				case HL_KEY_END: return 0x23;			// End Key
				case HL_KEY_HOME: return 0x24;			// Home Key
				case HL_KEY_INSERT: return 0x2D;		// Insert Key
				case HL_KEY_DELETE: return 0x2E;		// Delete Key
				case HL_KEY_LEFT_SUPER: return 0x5B;	// Left Windows Key
				case HL_KEY_RIGHT_SUPER: return 0x5C;	// Right Windows Key
				case HL_KEY_LEFT: return 0x25;			// Left
				case HL_KEY_UP: return 0x26;			// Up
				case HL_KEY_RIGHT: return 0x27;			// Right
				case HL_KEY_DOWN: return 0x28;			// Down
				case HL_KEY_COMMA: return 0xBC;			// , Key
				case HL_KEY_MINUS: return 0xBD;			// - Key
				case HL_KEY_PERIOD: return 0xBE;		// . Key

				case HL_KEY_0: return 0x30;				// 0
				case HL_KEY_1: return 0x31;				// 1
				case HL_KEY_2: return 0x32;				// 2
				case HL_KEY_3: return 0x33;				// 3
				case HL_KEY_4: return 0x34;				// 4
				case HL_KEY_5: return 0x35;				// 5
				case HL_KEY_6: return 0x36;				// 6
				case HL_KEY_7: return 0x37;				// 7
				case HL_KEY_8: return 0x38;				// 8
				case HL_KEY_9: return 0x39;				// 9

				case HL_KEY_KP_0: return 0x60;			// Numpad 0
				case HL_KEY_KP_1: return 0x61;			// Numpad 1
				case HL_KEY_KP_2: return 0x62;			// Numpad 2
				case HL_KEY_KP_3: return 0x63;			// Numpad 3
				case HL_KEY_KP_4: return 0x64;			// Numpad 4
				case HL_KEY_KP_5: return 0x65;			// Numpad 5
				case HL_KEY_KP_6: return 0x66;			// Numpad 6
				case HL_KEY_KP_7: return 0x67;			// Numpad 7
				case HL_KEY_KP_8: return 0x68;			// Numpad 8
				case HL_KEY_KP_9: return 0x69;			// Numpad 9
				case HL_KEY_KP_MULTIPLY: return 0x6A;	// Numpad Multiply
				case HL_KEY_KP_ADD: return 0x6B;		// Numpad Add
				case HL_KEY_KP_SUBTRACT: return 0x6D;	// Numpad Subtract
				case HL_KEY_KP_DECIMAL: return 0x6E;	// Numpad Decimal
				case HL_KEY_KP_DIVIDE: return 0x6F;		// Numpad Divide

				case HL_KEY_F1: return 0x70;			// F1
				case HL_KEY_F2: return 0x71;			// F2
				case HL_KEY_F3: return 0x72;			// F3
				case HL_KEY_F4: return 0x73;			// F4
				case HL_KEY_F5: return 0x74;			// F5
				case HL_KEY_F6: return 0x75;			// F6
				case HL_KEY_F7: return 0x76;			// F7
				case HL_KEY_F8: return 0x77;			// F8
				case HL_KEY_F9: return 0x78;			// F9
				case HL_KEY_F10: return 0x79;			// F10
				case HL_KEY_F11: return 0x7A;			// F11
				case HL_KEY_F12: return 0x7B;			// F12
				case HL_KEY_F13: return 0x7C;			// F13
				case HL_KEY_F14: return 0x7D;			// F14
				case HL_KEY_F15: return 0x7E;			// F15
				case HL_KEY_F16: return 0x7F;			// F16
				case HL_KEY_F17: return 0x80;			// F17
				case HL_KEY_F18: return 0x81;			// F18
				case HL_KEY_F19: return 0x82;			// F19
				case HL_KEY_F20: return 0x83;			// F20
				case HL_KEY_F21: return 0x84;			// F21
				case HL_KEY_F22: return 0x85;			// F22
				case HL_KEY_F23: return 0x86;			// F23
				case HL_KEY_F24: return 0x87;			// F24

				case HL_KEY_LEFT_SHIFT:					// Shift
				case HL_KEY_RIGHT_SHIFT:
					return 0x10;

				case HL_KEY_LEFT_CONTROL:				// Control
				case HL_KEY_RIGHT_CONTROL:
					return 0x11;
			}

			return -1;
		}
	}
}