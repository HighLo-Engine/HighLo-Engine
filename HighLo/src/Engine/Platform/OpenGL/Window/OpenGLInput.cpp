#include "HighLoPch.h"
#include "Engine/Core/HLInput.h"

#ifdef HIGHLO_API_OPENGL

#include <GLFW/glfw3.h>
#include "Engine/Application/HLApplication.h"

namespace highlo
{
	bool Input::IsKeyPressed(HLKeyCode keyCode)
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());
		int32 state = glfwGetKey(window, static_cast<int32>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(HLMouseButtonCode mouseButtonCode)
	{
		GLFWwindow *window = static_cast<GLFWwindow*>(HLApplication::Get().GetWindow().GetNativeHandle());
		int32 state = glfwGetMouseButton(window, static_cast<int32>(mouseButtonCode));
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
}

#endif // HIGHLO_API_OPENGL