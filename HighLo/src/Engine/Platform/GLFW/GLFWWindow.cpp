#include "HighLoPch.h"
#include "GLFWWindow.h"

#ifdef HIGHLO_API_GLFW

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLContext.h"
#elif HIGHLO_API_DX11
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Engine/Platform/DX11/DX11Context.h"
#include <GLFW/glfw3native.h>
#endif // HIGHLO_API_OPENGL

#include <stb_image.h>
#include "Engine/Events/Events.h"
#include "Engine/Renderer/ImGuiRenderer.h"

namespace highlo
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char *description)
	{
		HL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GLFWWindow::GLFWWindow(const WindowData &properties)
	{
		Window::s_WindowInstance = this;
		m_Properties = properties;
		Init();
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_NativeHandle);
		glfwTerminate();
	}

	void GLFWWindow::SetEventCallback(const EventCallbackFn &callback)
	{
		m_Properties.m_EventCallback = callback;
	}

	void GLFWWindow::Update()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void GLFWWindow::SetWindowIcon(const HLString &path, bool flip)
	{
		HL_CORE_INFO("Loading Window Icon: {0}", *path);

		GLFWimage images[1];
		stbi_set_flip_vertically_on_load(flip);
		images[0].pixels = stbi_load(path.C_Str(), &images[0].width, &images[0].height, 0, STBI_rgb_alpha);
		glfwSetWindowIcon(m_NativeHandle, 1, images);
		stbi_image_free(images[0].pixels);
	}

	std::pair<int32, int32> GLFWWindow::GetWindowDimensions()
	{
		const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return { mode->width, mode->height };
	}

	std::pair<int32, int32> GLFWWindow::GetWindowPosition()
	{
		int32 monitorX, monitorY;
		glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);
		return { monitorX, monitorY };
	}

	void GLFWWindow::CloseWindow()
	{
		WindowCloseEvent event;
		m_Properties.m_EventCallback(event);
		glfwDestroyWindow(m_NativeHandle);
		glfwTerminate();
	}

	int32 GLFWWindow::ShowMessageBox(const HLString &title, const HLString &msg, WindowMessageButtonType btnType, WindowMessageIcon icon)
	{
		HL_ASSERT(false, "Not supported with this platform!");
		return -1;
	}

	void GLFWWindow::SetMenuBar(const Ref<MenuBar> &bar)
	{
		m_MenuBar = bar;
		ImGuiRenderer::EnableMenuBar();
	}

	void GLFWWindow::SetVSync(bool bEnabled)
	{
		m_Properties.m_VSync = bEnabled;
		m_Context->SetSwapInterval(bEnabled);
	}

	void GLFWWindow::SetVisible(bool bVisible)
	{
		m_Properties.m_Visible = bVisible;
		if (bVisible)
			glfwShowWindow(m_NativeHandle);
		else
			glfwHideWindow(m_NativeHandle);
	}

	void GLFWWindow::SetFocus(bool bEnabled)
	{
		m_Properties.m_Focused = bEnabled;
		if (bEnabled)
			glfwFocusWindow(m_NativeHandle);
		else
			glfwRestoreWindow(m_NativeHandle);
	}

	void GLFWWindow::SetFullscreen(bool bEnabled)
	{
		m_Properties.m_Fullscreen = bEnabled;

		if (bEnabled)
		{
			// backup window position and window size
			glfwGetWindowPos(m_NativeHandle, &m_Placement.WindowPosX, &m_Placement.WindowPosY);
			glfwGetWindowSize(m_NativeHandle, &m_Placement.WindowWidth, &m_Placement.WindowHeight);

			// get resolution of monitor
			const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			// switch to full screen
			glfwSetWindowMonitor(m_NativeHandle, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
		}
		else
		{
			// restore last window size and position
			glfwSetWindowMonitor(m_NativeHandle, nullptr, m_Placement.WindowPosX, m_Placement.WindowPosY, m_Placement.WindowWidth, m_Placement.WindowHeight, 0);
		}
	}

	void GLFWWindow::ShowCursor()
	{
		m_Properties.m_CursorVisible = true;
		glfwSetInputMode(m_NativeHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void GLFWWindow::HideCursor()
	{
		m_Properties.m_CursorVisible = false;
		glfwSetInputMode(m_NativeHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void GLFWWindow::Maximize()
	{
		m_Properties.m_Maximized = true;
		glfwMaximizeWindow(m_NativeHandle);
	}

	void GLFWWindow::CenterWindow()
	{
		m_Properties.m_Centered = true;

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);

		int32 monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int32 width, height;
		glfwGetWindowSize(m_NativeHandle, &width, &height);

		int32 windowXPos = monitorX + (mode->width - width) / 2;
		int32 windowYPos = monitorY + (mode->height - height) / 2;
		glfwSetWindowPos(m_NativeHandle, windowXPos, windowYPos);
		m_Placement.WindowPosX = windowXPos;
		m_Placement.WindowPosY = windowYPos;
	}

	void GLFWWindow::SetTitle(const HLString &title)
	{
		m_Properties.m_Title = title;
		glfwSetWindowTitle(m_NativeHandle, *title);
	}

	bool GLFWWindow::IsFocused()
	{
		return glfwGetWindowAttrib(m_NativeHandle, GLFW_FOCUSED);
	}

	void GLFWWindow::Init()
		{
		if (!s_GLFWInitialized)
		{
			glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		// In directX we need to disable the window wrapper to get the HWND window handle
	#ifdef HIGHLO_API_DX11
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	#endif

		m_NativeHandle = glfwCreateWindow((int) m_Properties.m_Width, (int) m_Properties.m_Height, m_Properties.m_Title, nullptr, nullptr);

	#ifdef HIGHLO_API_OPENGL
		m_Context = UniqueRef<RenderingContext>(new OpenGLContext((void*)m_NativeHandle));
		m_Context->Init();
	#elif HIGHLO_API_DX11
		m_Context = UniqueRef<RenderingContext>(new DX11Context(m_Properties, glfwGetWin32Window(m_NativeHandle)));
		m_Context->Init();
	#endif

		glfwSetWindowUserPointer(m_NativeHandle, &m_Properties);
		m_Context->SetSwapInterval(false);

		SetCallbacks();
	}

	void GLFWWindow::SetCallbacks()
	{
		glfwSetWindowSizeCallback(m_NativeHandle, [](GLFWwindow *window, int width, int height)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_Width = width;
			data.m_Height = height;

			WindowResizeEvent event(width, height);
			data.m_EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_NativeHandle, [](GLFWwindow *window)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.m_EventCallback(event);
		});

		glfwSetKeyCallback(m_NativeHandle, [](GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.m_EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.m_EventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.m_EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_NativeHandle, [](GLFWwindow *window, unsigned int keycode)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyReleasedEvent event(keycode);
			data.m_EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_NativeHandle, [](GLFWwindow *window, int button, int action, int mods)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.m_EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.m_EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_NativeHandle, [](GLFWwindow *window, double xOffset, double yOffset)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float) xOffset, (float) yOffset);
			data.m_EventCallback(event);
		});

		glfwSetCursorPosCallback(m_NativeHandle, [](GLFWwindow *window, double xPos, double yPos)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			static float previous_x_pos = -1, previous_y_pos = -1;

			float dx = (float) xPos - previous_x_pos;
			float dy = (float) yPos - previous_y_pos;

			MouseMovedEvent event((float) xPos, (float) yPos, dx, dy);
			data.m_EventCallback(event);

			previous_x_pos = (float) xPos;
			previous_y_pos = (float) yPos;
		});

		glfwSetWindowFocusCallback(m_NativeHandle, [](GLFWwindow *window, int focused)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_Focused = (bool) focused;
		});
	}
}

#endif // HIGHLO_API_GLFW