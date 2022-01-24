// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "GLFWWindow.h"

#ifdef HIGHLO_API_GLFW

#ifdef HIGHLO_API_DX11
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif // HIGHLO_API_DX11

#include <stb_image.h>
#include "Engine/Events/Events.h"
#include "Engine/ImGui/ImGui.h"
#include "Engine/Core/Input.h"

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
		s_GLFWInitialized = false;
	}

	void GLFWWindow::SetEventCallback(const EventCallbackFn &callback)
	{
		m_Properties.EventCallback = callback;
	}

	const EventCallbackFn &GLFWWindow::GetEventCallback() const
	{
		return m_Properties.EventCallback;
	}

	void GLFWWindow::Update()
	{
		glfwPollEvents();
		Input::Update();

		// Flushes the window buffer or presents the window for vulkan
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
		m_Properties.EventCallback(event);
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
		UI::EnableMenuBar();
	}

	bool GLFWWindow::SetProgress(WindowProgressState state)
	{
		TBPFLAG flag = TBPF_NORMAL;
		switch (state)
		{
			case WindowProgressState::NORMAL:
				flag = TBPF_NORMAL;
				break;

			case WindowProgressState::ERROR:
				flag = TBPF_ERROR;
				break;

			case WindowProgressState::PAUSED:
				flag = TBPF_PAUSED;
				break;

			case WindowProgressState::NOPROGRESS:
				flag = TBPF_NOPROGRESS;
				break;

			case WindowProgressState::INDETERMINATE:
				flag = TBPF_INDETERMINATE;
				break;
		}

		return m_Taskbar->SetProgressState(glfwGetWin32Window(m_NativeHandle), flag) == S_OK;
	}

	bool GLFWWindow::SetProgressValue(uint64 completed, uint64 total)
	{
		return m_Taskbar->SetProgressValue(glfwGetWin32Window(m_NativeHandle), completed, total) == S_OK;
	}

	void GLFWWindow::SetVSync(bool bEnabled)
	{
		m_Properties.VSync = bEnabled;
		m_Context->SetSwapInterval(bEnabled);
	}

	void GLFWWindow::SetVisible(bool bVisible)
	{
		m_Properties.Visible = bVisible;
		if (bVisible)
			glfwShowWindow(m_NativeHandle);
		else
			glfwHideWindow(m_NativeHandle);
	}

	void GLFWWindow::SetFocus(bool bEnabled)
	{
		m_Properties.Focused = bEnabled;
		if (bEnabled)
			glfwFocusWindow(m_NativeHandle);
		else
			glfwRestoreWindow(m_NativeHandle);
	}

	void GLFWWindow::SetFullscreen(bool bEnabled)
	{
		m_Properties.Fullscreen = bEnabled;

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
		m_Properties.CursorVisible = true;
		glfwSetInputMode(m_NativeHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void GLFWWindow::HideCursor()
	{
		m_Properties.CursorVisible = false;
		glfwSetInputMode(m_NativeHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void GLFWWindow::Maximize()
	{
		m_Properties.Maximized = true;
		glfwMaximizeWindow(m_NativeHandle);
	}

	void GLFWWindow::CenterWindow()
	{
		m_Properties.Centered = true;

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
		m_Properties.Title = title;
		glfwSetWindowTitle(m_NativeHandle, *title);
	}

	bool GLFWWindow::IsFocused()
	{
		return glfwGetWindowAttrib(m_NativeHandle, GLFW_FOCUSED);
	}

	void GLFWWindow::Init()
		{
		uint32 width, height;
		
		if (!s_GLFWInitialized)
		{
			glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		// In directX we need to disable the window wrapper to get the HWND window handle
	#ifdef HIGHLO_API_DX11
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	#endif

	#ifdef HIGHLO_API_VULKAN
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	#endif // HIGHLO_API_VULKAN

		if (m_Properties.Fullscreen)
		{
			GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
			m_NativeHandle = glfwCreateWindow(videoMode->width, videoMode->height, *m_Properties.Title, primaryMonitor, nullptr);
		}
		else
		{
			m_NativeHandle = glfwCreateWindow((int32) m_Properties.Width, (int32) m_Properties.Height, *m_Properties.Title, nullptr, nullptr);
		}
		
		glfwSetCursor(m_NativeHandle, m_NativeCursor);

		HRESULT hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, __uuidof(ITaskbarList4), reinterpret_cast<void**>(&m_Taskbar));
		if (SUCCEEDED(hr))
		{
			hr = m_Taskbar->HrInit();
			if (!SUCCEEDED(hr))
			{
				HL_CORE_ERROR("Error: Taskbar could not be created!");
			}
		}

		m_Context = RenderingContext::Create((void*)m_NativeHandle, m_Properties);
		m_Context->Init();

		m_SwapChain = SwapChain::Create();
		m_SwapChain->Init(m_Context);
		m_SwapChain->InitSurface((void*)m_NativeHandle);

		width = m_Properties.Width;
		height = m_Properties.Height;
		m_SwapChain->Create(&width, &height, m_Properties.VSync);

		if (width != m_Properties.Width || height != m_Properties.Height)
		{
			// Trigger WindowResizeEvent, if swapchain changed the width or the height
			WindowResizeEvent e(width, height);
			m_Properties.EventCallback(e);
		}

		glfwSetWindowUserPointer(m_NativeHandle, &m_Properties);
		m_Context->SetSwapInterval(false);

		SetCallbacks();
	}

	void GLFWWindow::SetCallbacks()
	{
		glfwSetWindowSizeCallback(m_NativeHandle, [](GLFWwindow *window, int width, int height)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_NativeHandle, [](GLFWwindow *window)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_NativeHandle, [](GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_NativeHandle, [](GLFWwindow *window, unsigned int keycode)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyReleasedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_NativeHandle, [](GLFWwindow *window, int button, int action, int mods)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_NativeHandle, [](GLFWwindow *window, double xOffset, double yOffset)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float) xOffset, (float) yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_NativeHandle, [](GLFWwindow *window, double xPos, double yPos)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

			static float previous_x_pos = -1, previous_y_pos = -1;

			float dx = (float) xPos - previous_x_pos;
			float dy = (float) yPos - previous_y_pos;

			MouseMovedEvent event((float) xPos, (float) yPos, dx, dy);
			data.EventCallback(event);

			previous_x_pos = (float) xPos;
			previous_y_pos = (float) yPos;
		});

		glfwSetWindowFocusCallback(m_NativeHandle, [](GLFWwindow *window, int focused)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Focused = (bool) focused;
		});
	}
}

#endif // HIGHLO_API_GLFW

