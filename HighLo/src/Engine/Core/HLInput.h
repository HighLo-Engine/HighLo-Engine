#pragma once

#include "Engine/Core/Defines/HLKeyCodes.h"
#include "Engine/Core/Defines/HLMouseButtonCodes.h"

namespace highlo
{
	class Input
	{
	public:

		static bool IsKeyPressed(HLKeyCode keyCode);
		static bool IsMouseButtonPressed(HLMouseButtonCode mouseButtonCode);

		static std::pair<double, double> GetMousePosition();
		static std::pair<double, double> GetAbsoluteMousePosition();

		static double GetMouseX();
		static double GetAbsoluteMouseX();

		static double GetMouseY();
		static double GetAbsoluteMouseY();
	};
}