#pragma once
#include <rendering/RenderingContext.h>
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
