// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Window.h"

#ifdef HIGHLO_API_GLFW
#include "Engine/ThirdParty/GLFW/GLFWWindow.h"
#else
#include "Engine/Platform/Windows/WindowsWindow.h"
#endif // HIGHLO_API_GLFW

namespace highlo
{
	Window *Window::s_WindowInstance = nullptr;

	Window &Window::Get()
	{
		return *Window::s_WindowInstance;
	}

	UniqueRef<Window> Window::Create(const WindowData &properties)
	{
#ifdef HIGHLO_API_GLFW
		return UniqueRef<GLFWWindow>::Create(properties);
#else
		return UniqueRef<WindowsWindow>::Create(properties);
#endif
	}
}
