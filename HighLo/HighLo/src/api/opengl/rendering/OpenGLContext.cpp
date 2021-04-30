#include "OpenGLContext.h"

#ifdef HIGHLO_API_OPENGL
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

		std::stringstream ss;
		ss << "\nOpenGL Info:\n";
		ss << "    Vendor    :  " << glGetString(GL_VENDOR) << "\n";
		ss << "    Renderer  :  " << glGetString(GL_RENDERER) << "\n";
		ss << "    Version   :  " << glGetString(GL_VERSION) << "\n";
		HL_CORE_INFO(ss.str());
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers((GLFWwindow*)m_OpenGLWindowHandle);
	}
}
#endif // HIGHLO_API_OPENGL
