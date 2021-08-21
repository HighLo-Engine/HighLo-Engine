#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	class RenderingContext
	{
	public:
		
		virtual ~RenderingContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void MakeCurrent() = 0;
		virtual void SetSwapInterval(bool bEnabled) = 0;

		virtual void *GetCurrentContext() = 0;
	};
}
