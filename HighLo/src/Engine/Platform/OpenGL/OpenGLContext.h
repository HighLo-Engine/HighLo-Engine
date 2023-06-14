// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HIGHLO_API_OPENGL

#include "Engine/Window/Window.h"
#include "Engine/Graphics/RenderingContext.h"
#include "Engine/Core/Log.h"

namespace highlo
{
	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(void *handle, WindowData &data);
		virtual ~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override;

	private:
		void* m_OpenGLWindowHandle;
	};
}
#endif // #ifdef HIGHLO_API_OPENGL
