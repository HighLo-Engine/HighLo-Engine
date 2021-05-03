#pragma once

#include "Engine/Core/HLCore.h"

namespace highlo
{
	class RenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void SetSwapInterval(bool bEnabled) = 0;
		virtual ~RenderingContext() = default;
	};
}