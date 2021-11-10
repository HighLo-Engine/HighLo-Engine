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
	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};

	struct Controller
	{
		int32 ID;
		HLString Name;
		std::map<int32, bool> ButtonStates;
		std::map<int32, float> AxisStates;
		std::map<int32, uint8> HatStates;
	};

	class Input
	{
	public:

		HLAPI static void Update();

		HLAPI static bool IsKeyPressed(int32 keyCode);
		HLAPI static bool IsMouseButtonPressed(int32 mouseButtonCode);

		HLAPI static std::pair<double, double> GetMousePosition();
		HLAPI static std::pair<double, double> GetAbsoluteMousePosition();

		HLAPI static double GetMouseX();
		HLAPI static double GetAbsoluteMouseX();

		HLAPI static double GetMouseY();
		HLAPI static double GetAbsoluteMouseY();

		HLAPI static void SetCursorMode(CursorMode mode);
		HLAPI static CursorMode GetCursorMode();

		// Controllers
		HLAPI static bool HasController(int32 id);
		HLAPI static std::vector<int32> GetConnectedControllerIDs();
		HLAPI static const Controller *GetController(int32 id);
		HLAPI static HLString GetControllerName(int32 id);
		HLAPI static bool IsControllerButtonPressed(int32 id, int32 button);
		HLAPI static float GetControllerAxis(int32 id, int32 axis);
		HLAPI static uint8 GetControllerHat(int32 id, int32 hat);

		HLAPI static const std::map<int32, Controller> &GetControllers() { return s_Controllers; }

	private:

		inline static std::map<int32, Controller> s_Controllers;
	};
}
