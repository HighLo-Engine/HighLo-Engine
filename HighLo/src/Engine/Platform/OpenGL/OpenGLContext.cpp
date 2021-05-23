#include "HighLoPch.h"
#include "OpenGLContext.h"

#ifdef HIGHLO_API_OPENGL

#ifdef HIGHLO_API_GLFW

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace highlo
{
	OpenGLContext::OpenGLContext(void* handle)
	{
		m_OpenGLWindowHandle = handle;
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::MakeCurrent()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_OpenGLWindowHandle);
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_OpenGLWindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			HL_CORE_FATAL("Failed to load Context!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers((GLFWwindow*)m_OpenGLWindowHandle);
	}

	void OpenGLContext::SetSwapInterval(bool bEnabled)
	{
		glfwSwapInterval(bEnabled);
	}

	void *OpenGLContext::GetCurrentContext()
	{
		return (void*)glfwGetCurrentContext();
	}
}

#else

#include <Windows.h>
#include <glad/glad.h>

namespace highlo
{
	static HDC dc = nullptr;
	static HGLRC rc = nullptr;

	OpenGLContext::OpenGLContext(void *handle)
	{
		m_OpenGLWindowHandle = handle;
	}

	OpenGLContext::~OpenGLContext()
	{
		ReleaseDC((HWND)m_OpenGLWindowHandle, dc);
		wglDeleteContext(rc);
	}

	void OpenGLContext::Init()
	{
		PIXELFORMATDESCRIPTOR pfd =
			{
			sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1,							   // version number
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
			24,							   // 24-bit color depth
			0, 0, 0, 0, 0, 0,			   // color bits ignored
			0,							   // no alpha buffer
			0,							   // shift bit ignored
			0,							   // no accumulation buffer
			0, 0, 0, 0,					   // accum bits ignored
			32,							   // 32-bit z-buffer
			0,							   // no stencil buffer
			0,							   // no auxiliary buffer
			PFD_MAIN_PLANE,				   // main layer
			0,							   // reserved
			0, 0, 0						   // layer masks ignored
			};

		dc = GetDC((HWND)m_OpenGLWindowHandle);
		SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
		rc = wglCreateContext(dc);

		wglMakeCurrent(dc, rc);
		if (!gladLoadGL())
			HL_CORE_FATAL("Failed to load Context!");

		glEnable(GL_DEPTH_TEST);
		SetSwapInterval(false);

		std::stringstream ss;
		ss << "\nOpenGL Info:\n";
		ss << "    Vendor    :  " << glGetString(GL_VENDOR) << "\n";
		ss << "    Renderer  :  " << glGetString(GL_RENDERER) << "\n";
		ss << "    Version   :  " << glGetString(GL_VERSION) << "\n";
		HL_CORE_INFO(ss.str());
	}

	void OpenGLContext::MakeCurrent()
	{
		wglMakeCurrent(dc, rc);
	}

	void OpenGLContext::SwapBuffers()
	{
		::SwapBuffers(dc);
	}

	void OpenGLContext::SetSwapInterval(bool bEnabled)
	{
		((BOOL(WINAPI*)(int32))wglGetProcAddress("wglSwapIntervalEXT"))(bEnabled);
	}

	void *OpenGLContext::GetCurrentContext()
	{
		return (void*)rc;
	}
}

#endif // HIGHLO_API_GLFW

#endif // HIGHLO_API_OPENGL
