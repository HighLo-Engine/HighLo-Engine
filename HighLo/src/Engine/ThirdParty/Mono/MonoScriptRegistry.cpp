// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MonoScriptRegistry.h"

#include "Engine/ECS/Entity.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/loader.h>
#include <mono/jit/jit.h>

#include "MonoUtils.h"

namespace highlo
{
#define HL_REGISTER_INTERNAL_FUNC(func) mono_add_internal_call("highlo.InternalCalls::"#func, (void*)InternalCalls::func)

	namespace utils
	{
		static void AddComponentForType(Entity &e, const HLString &type)
		{
			if (type == "RelationshipComponent")
			{
				e.AddComponent<RelationshipComponent>();
			}
			else if (type == "PrefabComponent")
			{
				e.AddComponent<PrefabComponent>();
			}
			else if (type == "SceneComponent")
			{
				e.AddComponent<SceneComponent>();
			}
			else if (type == "CameraComponent")
			{
				e.AddComponent<CameraComponent>();
			}
			else if (type == "SpriteComponent")
			{
				e.AddComponent<SpriteComponent>();
			}
			else if (type == "StaticModelComponent")
			{
				e.AddComponent<StaticModelComponent>();
			}
			else if (type == "DynamicModelComponent")
			{
				e.AddComponent<DynamicModelComponent>();
			}
			else if (type == "DirectionalLightComponent")
			{
				e.AddComponent<DirectionalLightComponent>();
			}
			else if (type == "PointLightComponent")
			{
				e.AddComponent<PointLightComponent>();
			}
			else if (type == "SkyLightComponent")
			{
				e.AddComponent<SkyLightComponent>();
			}
			else if (type == "TextComponent")
			{
				e.AddComponent<TextComponent>();
			}
			else if (type == "ScriptComponent")
			{
				e.AddComponent<ScriptComponent>();
			}
			else
			{
				HL_ASSERT(false, "Could not find matching component!");
			}
		}

		static bool HasComponentFromType(Entity &e, const HLString &type)
		{
			if (type == "RelationshipComponent")
			{
				return e.HasComponent<RelationshipComponent>();
			}
			else if (type == "PrefabComponent")
			{
				return e.HasComponent<PrefabComponent>();
			}
			else if (type == "SceneComponent")
			{
				return e.HasComponent<SceneComponent>();
			}
			else if (type == "CameraComponent")
			{
				return e.HasComponent<CameraComponent>();
			}
			else if (type == "SpriteComponent")
			{
				return e.HasComponent<SpriteComponent>();
			}
			else if (type == "StaticModelComponent")
			{
				return e.HasComponent<StaticModelComponent>();
			}
			else if (type == "DynamicModelComponent")
			{
				return e.HasComponent<DynamicModelComponent>();
			}
			else if (type == "DirectionalLightComponent")
			{
				return e.HasComponent<DirectionalLightComponent>();
			}
			else if (type == "PointLightComponent")
			{
				return e.HasComponent<PointLightComponent>();
			}
			else if (type == "SkyLightComponent")
			{
				return e.HasComponent<SkyLightComponent>();
			}
			else if (type == "TextComponent")
			{
				return e.HasComponent<TextComponent>();
			}
			else if (type == "ScriptComponent")
			{
				return e.HasComponent<ScriptComponent>();
			}

			HL_ASSERT(false, "Could not find matching component!");
			return false;
		}

		static void RemoveComponentFromType(Entity &e, const HLString &type)
		{
			if (type == "RelationshipComponent")
			{
				e.RemoveComponent<RelationshipComponent>();
			}
			else if (type == "PrefabComponent")
			{
				e.RemoveComponent<PrefabComponent>();
			}
			else if (type == "SceneComponent")
			{
				e.RemoveComponent<SceneComponent>();
			}
			else if (type == "CameraComponent")
			{
				e.RemoveComponent<CameraComponent>();
			}
			else if (type == "SpriteComponent")
			{
				e.RemoveComponent<SpriteComponent>();
			}
			else if (type == "StaticModelComponent")
			{
				e.RemoveComponent<StaticModelComponent>();
			}
			else if (type == "DynamicModelComponent")
			{
				e.RemoveComponent<DynamicModelComponent>();
			}
			else if (type == "DirectionalLightComponent")
			{
				e.RemoveComponent<DirectionalLightComponent>();
			}
			else if (type == "PointLightComponent")
			{
				e.RemoveComponent<PointLightComponent>();
			}
			else if (type == "SkyLightComponent")
			{
				e.RemoveComponent<SkyLightComponent>();
			}
			else if (type == "TextComponent")
			{
				e.RemoveComponent<TextComponent>();
			}
			else if (type == "ScriptComponent")
			{
				e.RemoveComponent<ScriptComponent>();
			}
			else
			{
				HL_ASSERT(false, "Could not find matching component!");
			}
		}
	}

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
		RegisterComponent("RelationshipComponent");
		RegisterComponent("PrefabComponent");
		RegisterComponent("SceneComponent");
		RegisterComponent("CameraComponent");
		RegisterComponent("SpriteComponent");
		RegisterComponent("StaticModelComponent");
		RegisterComponent("DynamicModelComponent");
		RegisterComponent("DirectionalLightComponent");
		RegisterComponent("PointLightComponent");
		RegisterComponent("SkyLightComponent");
		RegisterComponent("TextComponent");
		RegisterComponent("ScriptComponent");
	}
	
	void MonoScriptRegistry::RegisterComponent(const HLString &name)
	{
		HLString fullName = "highlo." + name;
		MonoType *managedType = mono_reflection_type_from_name(*fullName, ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage);
		if (!managedType)
		{
			HL_CORE_ERROR("Could not find C# component class for {}", *fullName);
			return;
		}

		s_CreateComponentFuncs[managedType] = [=](Entity &e) { utils::AddComponentForType(e, name); };
		s_HasComponentFuncs[managedType] = [=](Entity &e) { return utils::HasComponentFromType(e, name); };
		s_RemoveComponentFuncs[managedType] = [=](Entity &e) { utils::RemoveComponentFromType(e, name); };
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
		return Input::HasController(id);
	}
	
	MonoArray *Input_GetControllerIds()
	{
		std::vector<int32> connectedControllers = Input::GetConnectedControllerIDs();
		return utils::StdToMonoArr(connectedControllers);
	}
	
	MonoString *Input_GetControllerName(int32 id)
	{
		HLString controllerName = Input::GetControllerName(id);
		return utils::StrToMonoStr(controllerName);
	}
	
	bool Input_IsControllerButtonPressed(int32 id, int32 button)
	{
		return Input::IsControllerButtonPressed(id, button);
	}
	
	float Input_GetControlerAxis(int32 id, int32 axis)
	{
		return Input::GetControllerAxis(id, axis);
	}
	
	uint8 Input_GetControllerHat(int32 id, int32 hat)
	{
		return Input::GetControllerHat(id, hat);
	}
	
	void Input_SetCursorMode(CursorMode mode)
	{
		Input::SetCursorMode(mode);
	}
	
	CursorMode Input_GetCursorMode()
	{
		return Input::GetCursorMode();
	}

#pragma endregion
}

#endif // HIGHLO_API_MONO_SCRIPTING

