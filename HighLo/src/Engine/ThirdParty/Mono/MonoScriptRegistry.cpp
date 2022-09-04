// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MonoScriptRegistry.h"

#include "Engine/ECS/Entity.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/loader.h>
#include <mono/jit/jit.h>

namespace highlo
{
#define HL_REGISTER_INTERNAL_FUNC(func) mono_add_internal_call("highlo.InternalCalls::"#func, (void*)InternalCalls::func)

	std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
	std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(Entity&)>> s_RemoveComponentFuncs;

	void MonoScriptRegistry::MakeRegistry()
	{
		if (s_CreateComponentFuncs.size())
		{
			s_CreateComponentFuncs.clear();
			s_HasComponentFuncs.clear();
			s_RemoveComponentFuncs.clear();
		}

		RegisterECSComponents();
		RegisterInternalFunctions();
	}
	
	void MonoScriptRegistry::RegisterInternalFunctions()
	{


		// Input
		HL_REGISTER_INTERNAL_FUNC(Input_IsKeyPressed);
		HL_REGISTER_INTERNAL_FUNC(Input_IsMouseButtonPressed);
		HL_REGISTER_INTERNAL_FUNC(Input_GetMousePosition);
		HL_REGISTER_INTERNAL_FUNC(Input_IsControllerPresent);
		HL_REGISTER_INTERNAL_FUNC(Input_GetControllerIds);
		HL_REGISTER_INTERNAL_FUNC(Input_GetControllerName);
		HL_REGISTER_INTERNAL_FUNC(Input_IsControllerButtonPressed);
		HL_REGISTER_INTERNAL_FUNC(Input_GetControlerAxis);
		HL_REGISTER_INTERNAL_FUNC(Input_GetControllerHat);
		HL_REGISTER_INTERNAL_FUNC(Input_SetCursorMode);
		HL_REGISTER_INTERNAL_FUNC(Input_GetCursorMode);
	}
	
	void MonoScriptRegistry::RegisterECSComponents()
	{

	}
}

namespace highlo::InternalCalls
{
#pragma region Input

	bool Input_IsKeyPressed(int32 keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}
	
	bool Input_IsMouseButtonPressed(int32 mouseButton)
	{
		bool wasPressed = Input::IsMouseButtonPressed(mouseButton);

		// TODO: add imgui viewport support here too

		return wasPressed;
	}
	
	void Input_GetMousePosition(glm::vec2 *outMousePos)
	{
		auto [x, y] = Input::GetMousePosition();
		*outMousePos = { (float)x, (float)y };
	}
	
	bool Input_IsControllerPresent(int32 id)
	{
		// TODO
		return false;
	}
	
	MonoArray *Input_GetControllerIds()
	{
		std::vector<int32> connectedControllers = Input::GetConnectedControllerIDs();
		// TODO: convert connectedControllers to MonoArray*
		return nullptr;
	}
	
	MonoString *Input_GetControllerName(int32 id)
	{
		return nullptr;
	}
	
	bool Input_IsControllerButtonPressed(int32 id, int32 button)
	{
		return false;
	}
	
	float Input_GetControlerAxis(int32 id, int32 axis)
	{
		return 0.0f;
	}
	
	uint8 Input_GetControllerHat(int32 id, int32 hat)
	{
		return uint8();
	}
	
	void Input_SetCursorMode(CursorMode mode)
	{
	}
	
	CursorMode Input_GetCursorMode()
	{
		return CursorMode();
	}

#pragma endregion
}

#endif // HIGHLO_API_MONO_SCRIPTING

