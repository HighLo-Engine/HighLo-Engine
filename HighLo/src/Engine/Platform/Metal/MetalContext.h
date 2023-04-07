// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/RenderingContext.h"

namespace highlo
{
	class MetalContext : public RenderingContext
	{
	public:

		MetalContext(void *handle, WindowData &data);
		virtual ~MetalContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override { return m_MetalWindowHandle; }

	private:

		void *m_MetalWindowHandle;
	};
}

#endif // HIGHLO_API_METAL

