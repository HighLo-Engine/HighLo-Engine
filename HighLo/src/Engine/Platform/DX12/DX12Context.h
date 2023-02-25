// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Graphics/RenderingContext.h"
#include "Engine/Window/Window.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12Context : public RenderingContext
	{
	public:

		DX12Context(void *handle, WindowData &data);
		virtual ~DX12Context();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override;

	private:

		void *m_DX12WindowHandle;
		WindowData m_WindowData;
	};
}

#endif // HIGHLO_API_DX12

