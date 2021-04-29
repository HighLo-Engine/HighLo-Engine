#include "OpenGLWindow.h"
#include <logging/Logger.h>
#include <api/opengl/rendering/OpenGLContext.h>
#include <events/Events.h>

namespace highlo
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        HL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

	Window* Window::s_WindowInstance = nullptr;

    Window& Window::Get()
    {
        return *Window::s_WindowInstance;
    }

    Window* Window::Create(const WindowData& properties)
    {
        return new OpenGLWindow(properties);
    }

    OpenGLWindow::OpenGLWindow(const WindowData& properties)
    {
        Window::s_WindowInstance = this;
        m_Properties = properties;
        Init();
    }

    OpenGLWindow::~OpenGLWindow()
    {
        glfwDestroyWindow(m_NativeHandle);
        glfwTerminate();
    }
    
    void OpenGLWindow::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Properties.event_callback = callback;
    }
    
    void OpenGLWindow::Update()
    {
		glfwPollEvents();
		m_Context->SwapBuffers();
    }
    
    unsigned int OpenGLWindow::GetWidth()
    {
        return m_Properties.width;
    }
    
    unsigned int OpenGLWindow::GetHeight()
    {
        return m_Properties.height;
    }
    
    void OpenGLWindow::SetCursorLocked(bool bLocked)
    {
		glfwSetInputMode(m_NativeHandle, GLFW_CURSOR, bLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		m_CursorLocked = bLocked;
    }
    
    bool OpenGLWindow::IsCursorLocked()
    {
        return m_CursorLocked;
    }
    
    bool OpenGLWindow::IsFocused()
    {
        return  m_Properties.focused;
    }
    
    void OpenGLWindow::Init()
    {
		glfwInit();
        glfwSetErrorCallback(GLFWErrorCallback);

        m_NativeHandle = glfwCreateWindow((int)m_Properties.width, (int)m_Properties.height, m_Properties.title, nullptr, nullptr);

        m_Context = UniqueRef<RenderingContext>(new OpenGLContext((void*)m_NativeHandle));
        m_Context->Init();

        glfwSetWindowUserPointer(m_NativeHandle, &m_Properties);
        glfwSwapInterval(0);

		SetCallbacks();
    }

    void OpenGLWindow::SetCallbacks()
    {
		glfwSetWindowSizeCallback(m_NativeHandle, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.event_callback(event);
		});

		glfwSetWindowCloseCallback(m_NativeHandle, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.event_callback(event);
		});

		/*glfwSetKeyCallback(m_NativeHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.event_callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.event_callback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.event_callback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_NativeHandle, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyReleasedEvent event(keycode);
			data.event_callback(event);
		});

		glfwSetMouseButtonCallback(m_NativeHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			auto hl_button = MouseButton::Left;
			if (button == GLFW_MOUSE_BUTTON_2)
				hl_button = MouseButton::Right;
			if (button == GLFW_MOUSE_BUTTON_3)
				hl_button = MouseButton::Middle;

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(hl_button);
					data.event_callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(hl_button);
					data.event_callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_NativeHandle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.event_callback(event);
		});

		glfwSetCursorPosCallback(m_NativeHandle, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			static float previous_x_pos = -1, previous_y_pos = -1;

			float dx = (float)xPos - previous_x_pos;
			float dy = (float)yPos - previous_y_pos;

			MouseMovedEvent event((float)xPos, (float)yPos, dx, dy);
			data.event_callback(event);

			previous_x_pos = (float)xPos;
			previous_y_pos = (float)yPos;
		});

		glfwSetWindowFocusCallback(m_NativeHandle, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.focused = (bool)focused;
		});*/
    }
}
