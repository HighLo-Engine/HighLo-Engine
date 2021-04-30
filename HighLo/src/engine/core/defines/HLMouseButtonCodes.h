#pragma once

#include <iostream>
#include "HLBaseTypes.h"

typedef enum class HLMouseButtonCode : uint16
{
	BUTTON_1 = 0,
	BUTTON_2 = 1,
	BUTTON_3 = 2,
	BUTTON_4 = 3,
	BUTTON_5 = 4,
	BUTTON_6 = 5,
	BUTTON_7 = 6,
	BUTTON_8 = 7,
	BUTTON_LAST = 7,
	BUTTON_LEFT = 0,
	BUTTON_RIGHT = 1,
	BUTTON_MIDDLE = 2
} HLMouse;

inline std::ostream &operator<<(std::ostream &stream, HLMouseButtonCode code)
{
	return stream << static_cast<int32>(code);
}

#define HL_MOUSE_BUTTON_1 ::HLMouse::BUTTON_1
#define HL_MOUSE_BUTTON_2 ::HLMouse::BUTTON_2
#define HL_MOUSE_BUTTON_3 ::HLMouse::BUTTON_3
#define HL_MOUSE_BUTTON_4 ::HLMouse::BUTTON_4
#define HL_MOUSE_BUTTON_5 ::HLMouse::BUTTON_5
#define HL_MOUSE_BUTTON_6 ::HLMouse::BUTTON_6
#define HL_MOUSE_BUTTON_7 ::HLMouse::BUTTON_7
#define HL_MOUSE_BUTTON_8 ::HLMouse::BUTTON_8
#define HL_MOUSE_BUTTON_LAST ::HLMouse::BUTTON_LAST
#define HL_MOUSE_BUTTON_LEFT ::HLMouse::BUTTON_LEFT
#define HL_MOUSE_BUTTON_RIGHT ::HLMouse::BUTTON_RIGHT
#define HL_MOUSE_BUTTON_MIDDLE ::HLMouse::BUTTON_MIDDLE

