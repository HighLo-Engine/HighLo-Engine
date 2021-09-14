// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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
