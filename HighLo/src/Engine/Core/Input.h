// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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
