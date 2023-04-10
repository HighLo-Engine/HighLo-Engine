// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	struct WindowData;

	class RenderingContext : public IsSharedReference
	{
	public:
		
		HLAPI virtual ~RenderingContext() = default;

		HLAPI virtual void Init() = 0;
		HLAPI virtual void SwapBuffers() = 0;
		HLAPI virtual void MakeCurrent() = 0;
		HLAPI virtual void ForgetContext() = 0;
		HLAPI virtual void SetSwapInterval(bool bEnabled) = 0;

		HLAPI virtual void *GetCurrentContext() = 0;

		HLAPI static Ref<RenderingContext> Create(void *handle, WindowData &data);
	};
}

