#pragma once

#ifdef HIGHLO_API_OPENGL

#include "Engine/Window/Window.h"
#include "Engine/Renderer/RenderingContext.h"
#include "Engine/Core/HLLog.h"

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
