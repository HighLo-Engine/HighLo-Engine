#pragma once

#include <iostream>
#include "HLBaseTypes.h"

typedef enum class HLKeyCode : uint16
{
	/* Printable keys */
	SPACE = 32,
	APOSTROPHE = 39,  /* ' */
	COMMA = 44,  /* , */
	MINUS = 45,  /* - */
	PERIOD = 46,  /* . */
	SLASH = 47,  /* / */
	SEMICOLON = 59,  /* ; */
	EQUAL = 61,  /* = */
	LEFT_BRACKET = 91,  /* [ */
	BACKSLASH = 92,  /* \ */
	RIGHT_BRACKET = 93,  /* ] */
	GRAVE_ACCENT = 96,  /* ` */
	WORLD_1 = 161, /* non-US #1 */
	WORLD_2 = 162, /* non-US #2 */

	K_0 = 48,
	K_1 = 49,
	K_2 = 50,
	K_3 = 51,
	K_4 = 52,
	K_5 = 53,
	K_6 = 54,
	K_7 = 55,
	K_8 = 56,
	K_9 = 57,

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	/* Function keys */
	ESCAPE = 256,
	ENTER = 257,
	TAB = 258,
	BACKSPACE = 259,
	INSERT = 260,
	KEY_DELETE = 261,
	RIGHT = 262,
	LEFT = 263,
	DOWN = 264,
	UP = 265,
	PAGE_UP = 266,
	PAGE_DOWN = 267,
	HOME = 268,
	END = 269,
	CAPS_LOCK = 280,
	SCROLL_LOCK = 281,
	NUM_LOCK = 282,
	PRINT_SCREEN = 283,
	PAUSE = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,

	KP_0 = 320,
	KP_1 = 321,
	KP_2 = 322,
	KP_3 = 323,
	KP_4 = 324,
	KP_5 = 325,
	KP_6 = 326,
	KP_7 = 327,
	KP_8 = 328,
	KP_9 = 329,
	KP_DECIMAL = 330,
	KP_DIVIDE = 331,
	KP_MULTIPLY = 332,
	KP_SUBTRACT = 333,
	KP_ADD = 334,
	KP_ENTER = 335,
	KP_EQUAL = 336,
	LEFT_SHIFT = 340,
	LEFT_CONTROL = 341,
	LEFT_ALT = 342,
	LEFT_SUPER = 343,
	RIGHT_SHIFT = 344,
	RIGHT_CONTROL = 345,
	RIGHT_ALT = 346,
	RIGHT_SUPER = 347,
	MENU = 348
} HLKey;

inline std::ostream &operator<<(std::ostream &stream, HLKeyCode code)
{
	return stream << static_cast<int32>(code);
}

/* Printable keys */
#define HL_KEY_SPACE              ::HLKey::SPACE
#define HL_KEY_APOSTROPHE         ::HLKey::APOSTROPHE   /* ' */
#define HL_KEY_COMMA              ::HLKey::COMMA		/* , */
#define HL_KEY_MINUS              ::HLKey::MINUS		/* - */
#define HL_KEY_PERIOD             ::HLKey::PERIOD       /* . */
#define HL_KEY_SLASH              ::HLKey::SLASH        /* / */
#define HL_KEY_0                  ::HLKey::K_0
#define HL_KEY_1                  ::HLKey::K_1
#define HL_KEY_2                  ::HLKey::K_2
#define HL_KEY_3                  ::HLKey::K_3
#define HL_KEY_4                  ::HLKey::K_4
#define HL_KEY_5                  ::HLKey::K_5
#define HL_KEY_6                  ::HLKey::K_6
#define HL_KEY_7                  ::HLKey::K_7
#define HL_KEY_8                  ::HLKey::K_8
#define HL_KEY_9                  ::HLKey::K_9
#define HL_KEY_SEMICOLON          ::HLKey::SEMICOLON     /* ; */
#define HL_KEY_EQUAL              ::HLKey::EQUAL         /* = */
#define HL_KEY_A                  ::HLKey::A
#define HL_KEY_B                  ::HLKey::B
#define HL_KEY_C                  ::HLKey::C
#define HL_KEY_D                  ::HLKey::D
#define HL_KEY_E                  ::HLKey::E
#define HL_KEY_F                  ::HLKey::F
#define HL_KEY_G                  ::HLKey::G
#define HL_KEY_H                  ::HLKey::H
#define HL_KEY_I                  ::HLKey::I
#define HL_KEY_J                  ::HLKey::J
#define HL_KEY_K                  ::HLKey::K
#define HL_KEY_L                  ::HLKey::L
#define HL_KEY_M                  ::HLKey::M
#define HL_KEY_N                  ::HLKey::N
#define HL_KEY_O                  ::HLKey::O
#define HL_KEY_P                  ::HLKey::P
#define HL_KEY_Q                  ::HLKey::Q
#define HL_KEY_R                  ::HLKey::R
#define HL_KEY_S                  ::HLKey::S
#define HL_KEY_T                  ::HLKey::T
#define HL_KEY_U                  ::HLKey::U
#define HL_KEY_V                  ::HLKey::V
#define HL_KEY_W                  ::HLKey::W
#define HL_KEY_X                  ::HLKey::X
#define HL_KEY_Y                  ::HLKey::Y
#define HL_KEY_Z                  ::HLKey::Z
#define HL_KEY_LEFT_BRACKET       ::HLKey::LEFT_BRACKET  /* [ */
#define HL_KEY_BACKSLASH          ::HLKey::BACKSLASH  /* \ */
#define HL_KEY_RIGHT_BRACKET      ::HLKey::RIGHT_BRACKET  /* ] */
#define HL_KEY_GRAVE_ACCENT       ::HLKey::GRAVE_ACCENT  /* ` */
#define HL_KEY_WORLD_1            ::HLKey::WORLD_1 /* non-US #1 */
#define HL_KEY_WORLD_2            ::HLKey::WORLD_2 /* non-US #2 */

/* Function keys */
#define HL_KEY_ESCAPE             ::HLKey::ESCAPE
#define HL_KEY_ENTER              ::HLKey::ENTER
#define HL_KEY_TAB                ::HLKey::TAB
#define HL_KEY_BACKSPACE          ::HLKey::BACKSPACE
#define HL_KEY_INSERT             ::HLKey::INSERT
#define HL_KEY_DELETE             ::HLKey::KEY_DELETE
#define HL_KEY_RIGHT              ::HLKey::RIGHT
#define HL_KEY_LEFT               ::HLKey::LEFT
#define HL_KEY_DOWN               ::HLKey::DOWN
#define HL_KEY_UP                 ::HLKey::UP
#define HL_KEY_PAGE_UP            ::HLKey::PAGE_UP
#define HL_KEY_PAGE_DOWN          ::HLKey::PAGE_DOWN
#define HL_KEY_HOME               ::HLKey::HOME
#define HL_KEY_END                ::HLKey::END
#define HL_KEY_CAPS_LOCK          ::HLKey::CAPS_LOCK
#define HL_KEY_SCROLL_LOCK        ::HLKey::SCROLL_LOCK
#define HL_KEY_NUM_LOCK           ::HLKey::NUM_LOCK
#define HL_KEY_PRINT_SCREEN       ::HLKey::PRINT_SCREEN
#define HL_KEY_PAUSE              ::HLKey::PAUSE
#define HL_KEY_F1                 ::HLKey::F1
#define HL_KEY_F2                 ::HLKey::F2
#define HL_KEY_F3                 ::HLKey::F3
#define HL_KEY_F4                 ::HLKey::F4
#define HL_KEY_F5                 ::HLKey::F5
#define HL_KEY_F6                 ::HLKey::F6
#define HL_KEY_F7                 ::HLKey::F7
#define HL_KEY_F8                 ::HLKey::F8
#define HL_KEY_F9                 ::HLKey::F9
#define HL_KEY_F10                ::HLKey::F10
#define HL_KEY_F11                ::HLKey::F11
#define HL_KEY_F12                ::HLKey::F12
#define HL_KEY_F13                ::HLKey::F13
#define HL_KEY_F14                ::HLKey::F14
#define HL_KEY_F15                ::HLKey::F15
#define HL_KEY_F16                ::HLKey::F16
#define HL_KEY_F17                ::HLKey::F17
#define HL_KEY_F18                ::HLKey::F18
#define HL_KEY_F19                ::HLKey::F19
#define HL_KEY_F20                ::HLKey::F20
#define HL_KEY_F21                ::HLKey::F21
#define HL_KEY_F22                ::HLKey::F22
#define HL_KEY_F23                ::HLKey::F23
#define HL_KEY_F24                ::HLKey::F24
#define HL_KEY_F25                ::HLKey::F25
#define HL_KEY_KP_0               ::HLKey::KP_0
#define HL_KEY_KP_1               ::HLKey::KP_1
#define HL_KEY_KP_2               ::HLKey::KP_2
#define HL_KEY_KP_3               ::HLKey::KP_3
#define HL_KEY_KP_4               ::HLKey::KP_4
#define HL_KEY_KP_5               ::HLKey::KP_5
#define HL_KEY_KP_6               ::HLKey::KP_6
#define HL_KEY_KP_7               ::HLKey::KP_7
#define HL_KEY_KP_8               ::HLKey::KP_8
#define HL_KEY_KP_9               ::HLKey::KP_9
#define HL_KEY_KP_DECIMAL         ::HLKey::KP_DECIMAL
#define HL_KEY_KP_DIVIDE          ::HLKey::KP_DIVIDE
#define HL_KEY_KP_MULTIPLY        ::HLKey::KP_MULTIPLY
#define HL_KEY_KP_SUBTRACT        ::HLKey::KP_SUBTRACT
#define HL_KEY_KP_ADD             ::HLKey::KP_ADD
#define HL_KEY_KP_ENTER           ::HLKey::KP_ENTER
#define HL_KEY_KP_EQUAL           ::HLKey::KP_EQUAL
#define HL_KEY_LEFT_SHIFT         ::HLKey::LEFT_SHIFT
#define HL_KEY_LEFT_CONTROL       ::HLKey::LEFT_CONTROL
#define HL_KEY_LEFT_ALT           ::HLKey::LEFT_ALT
#define HL_KEY_LEFT_SUPER         ::HLKey::LEFT_SUPER
#define HL_KEY_RIGHT_SHIFT        ::HLKey::RIGHT_SHIFT
#define HL_KEY_RIGHT_CONTROL      ::HLKey::RIGHT_CONTROL
#define HL_KEY_RIGHT_ALT          ::HLKey::RIGHT_ALT
#define HL_KEY_RIGHT_SUPER        ::HLKey::RIGHT_SUPER
#define HL_KEY_MENU               ::HLKey::MENU

