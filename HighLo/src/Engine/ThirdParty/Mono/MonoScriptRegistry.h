// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-04) initial release
//

#pragma once

#include "Engine/Core/Input.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoReflectionType MonoReflectionType;
}

namespace highlo
{
	class MonoScriptRegistry
	{
	public:

		HLAPI static void MakeRegistry();

	private:

		static void RegisterInternalFunctions();
		static void RegisterECSComponents();

		static void RegisterComponent(const HLString &name);
	};
}

namespace highlo::InternalCalls
{
#pragma region Input

	bool Input_IsKeyPressed(int32 keyCode);
	bool Input_IsMouseButtonPressed(int32 mouseButton);
	void Input_GetMousePosition(glm::vec2 *outMousePos);

	bool Input_IsControllerPresent(int32 id);
	MonoArray *Input_GetControllerIds();
	MonoString *Input_GetControllerName(int32 id);
	bool Input_IsControllerButtonPressed(int32 id, int32 button);
	float Input_GetControlerAxis(int32 id, int32 axis);
	uint8 Input_GetControllerHat(int32 id, int32 hat);

	void Input_SetCursorMode(CursorMode mode);
	CursorMode Input_GetCursorMode();

#pragma endregion
}

#endif // HIGHLO_API_MONO_SCRIPTING

