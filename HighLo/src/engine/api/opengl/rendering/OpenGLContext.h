#pragma once

#include "engine/rendering/RenderingContext.h"
#include "engine/core/Log.h"

#ifdef HIGHLO_API_OPENGL
#include "engine/window/Window.h"

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
