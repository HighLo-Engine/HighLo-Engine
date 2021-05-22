#include "HighLoPch.h"
#include "Engine/Core/HLInput.h"

#ifndef HIGHLO_API_GLFW

#include <Windows.h>
#include "Engine/Application/HLApplication.h"
#include "Engine/Utils/InputUtils.h"

namespace highlo
{
	bool Input::IsKeyPressed(int32 keyCode)
	{
		return (GetKeyState(utils::InputHelperHLKeyCodeToWindows(keyCode)) & 0x8000);
	}

	bool Input::IsMouseButtonPressed(int32 mouseButtonCode)
	{
		if (mouseButtonCode == HL_MOUSE_BUTTON_LEFT) return (GetKeyState(VK_LBUTTON) & 0x8000);
		else if (mouseButtonCode == HL_MOUSE_BUTTON_RIGHT) return (GetKeyState(VK_RBUTTON) & 0x8000);
		else if (mouseButtonCode == HL_MOUSE_BUTTON_MIDDLE) return (GetKeyState(VK_MBUTTON) & 0x8000);

		return false;
	}

	std::pair<double, double> Input::GetMousePosition()
	{
		HWND window = static_cast<HWND>(HLApplication::Get().GetWindow().GetNativeHandle());
		int32 width = (int32) HLApplication::Get().GetWindow().GetWidth();
		int32 height = (int32) HLApplication::Get().GetWindow().GetHeight();

		POINT pos;
		GetCursorPos(&pos);

		if (window)
			ScreenToClient(window, &pos);

		if (pos.x > width)
			pos.x = (LONG) width;
		else if (pos.x < 0L)
			pos.x = 0L;

		if (pos.y > height)
			pos.y = (LONG) height;
		else if (pos.y < 0L)
			pos.y = 0L;

		return { (double) pos.x, (double) pos.y };
	}

	std::pair<double, double> Input::GetAbsoluteMousePosition()
	{
		POINT pos;
		GetCursorPos(&pos);
		return { (double) pos.x, (double) pos.y };
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

#endif // HIGHLO_API_GLFW