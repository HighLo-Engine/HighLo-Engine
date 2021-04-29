#include "OpenGLContext.h"
#include <logging/Logger.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace highlo
{
	OpenGLContext::OpenGLContext(void* handle)
	{
		m_OpenGLWindowHandle = handle;
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_OpenGLWindowHandle);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			HL_CORE_FATAL("Failed to initialize OpenGL context");

		HL_CORE_TRACE("OpenGL Info:");
		HL_CORE_TRACE("    Vendor    : {0}", glGetString(GL_VENDOR));
		HL_CORE_TRACE("    Renderer  : {0}", glGetString(GL_RENDERER));
		HL_CORE_TRACE("    Version   : {0}\n\n", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers((GLFWwindow*)m_OpenGLWindowHandle);
	}
}
