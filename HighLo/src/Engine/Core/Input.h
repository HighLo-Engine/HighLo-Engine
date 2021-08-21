#pragma once

#include "Engine/Core/Defines/KeyCodes.h"
#include "Engine/Core/Defines/MouseButtonCodes.h"

namespace highlo
{
	class Input
	{
	public:

		HLAPI static bool IsKeyPressed(int32 keyCode);
		HLAPI static bool IsMouseButtonPressed(int32 mouseButtonCode);

		HLAPI static std::pair<double, double> GetMousePosition();
		HLAPI static std::pair<double, double> GetAbsoluteMousePosition();

		HLAPI static double GetMouseX();
		HLAPI static double GetAbsoluteMouseX();

		HLAPI static double GetMouseY();
		HLAPI static double GetAbsoluteMouseY();
	};
}
