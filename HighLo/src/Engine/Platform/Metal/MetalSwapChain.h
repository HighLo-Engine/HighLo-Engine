// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/SwapChain.h"
#include "Engine/Graphics/RenderingContext.h"

namespace highlo
{
	class MetalSwapChain : public SwapChain
	{
	public:

		virtual void Init(const Ref<RenderingContext> &context) override;
		virtual void InitSurface(void *windowHandle) override;
		virtual void Create(uint32 *width, uint32 *height, bool vsync) override;
		virtual void Cleanup() override;

		virtual void OnResize(uint32 width, uint32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Present() override;

		virtual uint32 GetImageCount() const override { HL_ASSERT(false); return -1; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32 GetCurrentBufferIndex() const override { return m_CurrentBufferIndex; }

	private:

		Ref<RenderingContext> m_Context = nullptr;
		uint32 m_Width = 0, m_Height = 0;
		uint32 m_CurrentBufferIndex = 0;
		bool m_VSync;
		void *m_NativeHandle = nullptr;
	};
}

#endif // HIGHLO_API_METAL

