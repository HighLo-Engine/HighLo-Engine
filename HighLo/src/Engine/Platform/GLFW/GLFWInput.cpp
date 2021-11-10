// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/Input.h"

#ifdef HIGHLO_API_GLFW

#include <GLFW/glfw3.h>
#include "Engine/Application/Application.h"

namespace highlo
{
	bool Input::IsKeyPressed(int32 keyCode)
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());
		int32 state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int32 mouseButtonCode)
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());
		int32 state = glfwGetMouseButton(window, mouseButtonCode);
		return state == GLFW_PRESS;
	}

	std::pair<double, double> Input::GetMousePosition()
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		uint32 width = HLApplication::Get().GetWindow().GetWidth();
		uint32 height = HLApplication::Get().GetWindow().GetHeight();

		if (xPos > width)
			xPos = width;
		else if (xPos < 0.0)
			xPos = 0.0;

		if (yPos > height)
			yPos = height;
		else if (yPos < 0.0)
			yPos = 0.0;

		return { xPos, yPos };
	}

	std::pair<double, double> Input::GetAbsoluteMousePosition()
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { xPos, yPos };
	}

	double Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	double Input::GetAbsoluteMouseX()
	{
		auto [x, y] = GetAbsoluteMousePosition();
		return x;
	}

	double Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

	double Input::GetAbsoluteMouseY()
	{
		auto [x, y] = GetAbsoluteMousePosition();
		return y;
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int32)mode);
	}

	CursorMode Input::GetCursorMode()
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());
		return (CursorMode)(glfwGetInputMode(window, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
	}

	void Input::Update()
	{
		// Cleanup disconnected controller
		for (auto it = s_Controllers.begin(); it != s_Controllers.end();)
		{
			int32 id = it->first;
			if (glfwJoystickPresent(id) != GLFW_TRUE)
				it = s_Controllers.erase(it);
			else
				++it;
		}

		// Update Controllers
		for (int32 id = GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; ++id)
		{
			if (glfwJoystickPresent(id) == GLFW_TRUE)
			{
				Controller &controller = s_Controllers[id];
				controller.ID = id;
				controller.Name = glfwGetJoystickName(id);

				int32 buttonCount;
				const Byte *buttons = glfwGetJoystickButtons(id, &buttonCount);
				for (int32 i = 0; i < buttonCount; ++i)
					controller.ButtonStates[i] = buttons[i] == GLFW_PRESS;

				int32 axisCount;
				const float *axes = glfwGetJoystickAxes(id, &axisCount);
				for (int32 i = 0; i < axisCount; ++i)
					controller.AxisStates[i] = axes[i];

				int32 hatCount;
				const Byte *hats = glfwGetJoystickHats(id, &hatCount);
				for (int32 i = 0; i < hatCount; ++i)
					controller.HatStates[i] = hats[i];
			}
		}
	}

	bool Input::HasController(int32 id)
	{
		return s_Controllers.find(id) != s_Controllers.end();
	}

	std::vector<int32> Input::GetConnectedControllerIDs()
	{
		std::vector<int32> ids;
		ids.reserve(s_Controllers.size());
		for (auto [id, controller] : s_Controllers)
			ids.emplace_back(id);

		return ids;
	}

	const Controller *Input::GetController(int32 id)
	{
		if (!Input::HasController(id))
			return nullptr;

		return &s_Controllers.at(id);
	}

	HLString Input::GetControllerName(int32 id)
	{
		if (!Input::HasController(id))
			return {};

		return s_Controllers.at(id).Name;
	}

	bool Input::IsControllerButtonPressed(int32 id, int32 button)
	{
		if (!Input::HasController(id))
			return false;

		const Controller &controller = s_Controllers.at(id);
		if (controller.ButtonStates.find(button) == controller.ButtonStates.end())
			return false;

		return controller.ButtonStates.at(button);
	}

	float Input::GetControllerAxis(int32 id, int32 axis)
	{
		if (!Input::HasController(id))
			return 0.0f;

		const Controller &controller = s_Controllers.at(id);
		if (controller.AxisStates.find(axis) == controller.AxisStates.end())
			return 0.0f;

		return controller.AxisStates.at(axis);
	}

	uint8 Input::GetControllerHat(int32 id, int32 hat)
	{
		if (!Input::HasController(id))
			return 0;

		const Controller &controller = s_Controllers.at(id);
		if (controller.HatStates.find(hat) == controller.HatStates.end())
			return 0;

		return controller.HatStates.at(hat);
	}
}

#endif // HIGHLO_API_GLFW