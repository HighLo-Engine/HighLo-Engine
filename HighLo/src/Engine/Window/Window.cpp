// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

#include "HighLoPch.h"
#include "Window.h"

#ifdef HIGHLO_API_GLFW
#include "Engine/Platform/GLFW/GLFWWindow.h"
#else
#include "Engine/Platform/Windows/WindowsWindow.h"
#endif // HIGHLO_API_GLFW

namespace highlo
{
	Window* Window::s_WindowInstance = nullptr;

	Window& Window::Get()
	{
		return *Window::s_WindowInstance;
	}

	Window* Window::Create(const WindowData& properties)
	{
#ifdef HIGHLO_API_GLFW
		return new GLFWWindow(properties);
#else
		return new WindowsWindow(properties);
#endif
	}
}
