#include "OpenGLWindow.h"

#ifdef HIGHLO_API_OPENGL
#include <logging/Logger.h>
#include <api/opengl/rendering/OpenGLContext.h>
#include <events/Events.h>

namespace highlo
{
	static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        HL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
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
        m_Properties.m_EventCallback = callback;
    }
    
    void OpenGLWindow::Update()
    {
		glfwPollEvents();
		m_Context->SwapBuffers();
    }
    
    unsigned int OpenGLWindow::GetWidth()
    {
        return m_Properties.m_Width;
    }
    
    unsigned int OpenGLWindow::GetHeight()
    {
        return m_Properties.m_Height;
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
        return m_Properties.m_Focused;
    }
    
    void OpenGLWindow::Init()
    {
		if (!s_GLFWInitialized)
		{
			glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
		}

        m_NativeHandle = glfwCreateWindow((int)m_Properties.m_Width, (int)m_Properties.m_Height, m_Properties.m_Title, nullptr, nullptr);

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
			data.m_Width= width;
			data.m_Height = height;

			WindowResizeEvent event(width, height);
			data.m_EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_NativeHandle, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.m_EventCallback(event);
		});

		glfwSetKeyCallback(m_NativeHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

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

		glfwSetCharCallback(m_NativeHandle, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyReleasedEvent event(keycode);
			data.m_EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_NativeHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

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

		glfwSetScrollCallback(m_NativeHandle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.m_EventCallback(event);
		});

		glfwSetCursorPosCallback(m_NativeHandle, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			static float previous_x_pos = -1, previous_y_pos = -1;

			float dx = (float)xPos - previous_x_pos;
			float dy = (float)yPos - previous_y_pos;

			MouseMovedEvent event((float)xPos, (float)yPos, dx, dy);
			data.m_EventCallback(event);

			previous_x_pos = (float)xPos;
			previous_y_pos = (float)yPos;
		});

		glfwSetWindowFocusCallback(m_NativeHandle, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_Focused = (bool)focused;
		});
    }
}
#endif // HIGHLO_API_OPENGL
