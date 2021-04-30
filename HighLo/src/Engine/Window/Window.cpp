#include "HighLoPch.h"
#include "Window.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/Window/OpenGLWindow.h"
#endif // HIGHLO_API_OPENGL

#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/Window/DX11Window.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
	Window* Window::s_WindowInstance = nullptr;

	Window& Window::Get()
	{
		return *Window::s_WindowInstance;
	}

	Window* Window::Create(const WindowData& properties)
	{
#ifdef HIGHLO_API_OPENGL
		return new OpenGLWindow(properties);
#endif
#ifdef HIGHLO_API_DX11
		return new DX11Window(properties);
#endif
	}
}
