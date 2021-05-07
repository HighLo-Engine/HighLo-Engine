#pragma once

#include "Engine/Core/Defines/HLKeyCodes.h"
#include "Engine/Core/Defines/HLMouseButtonCodes.h"

namespace highlo
{
	class Input
	{
	public:

		HLAPI static bool IsKeyPressed(HLKeyCode keyCode);
		HLAPI static bool IsMouseButtonPressed(HLMouseButtonCode mouseButtonCode);

		HLAPI static std::pair<double, double> GetMousePosition();
		HLAPI static std::pair<double, double> GetAbsoluteMousePosition();

		HLAPI static double GetMouseX();
		HLAPI static double GetAbsoluteMouseX();

		HLAPI static double GetMouseY();
		HLAPI static double GetAbsoluteMouseY();
	};
}
