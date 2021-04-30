#pragma once
#include <rendering/RenderingContext.h>

#ifdef HIGHLO_API_OPENGL
#include <window/Window.h>

namespace highlo
{
	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(void* handle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		void* m_OpenGLWindowHandle;
	};
}
#endif // #ifdef HIGHLO_API_OPENGL
