// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Renderer/SwapChain.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12SwapChain : public SwapChain
	{
	public:

		DX12SwapChain();
		virtual ~DX12SwapChain();

		virtual void Init(const Ref<RenderingContext> &context) override;
		virtual void InitSurface(void *windowHandle) override;
		virtual void Create(uint32 *width, uint32 *height, bool vsync) override;
		virtual void Cleanup() override;

		virtual void OnResize(uint32 width, uint32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Present() override;

		virtual uint32 GetImageCount() const override;
		virtual uint32 GetWidth() const override;
		virtual uint32 GetHeight() const override;
		virtual uint32 GetCurrentBufferIndex() const override;

	private:


	};
}

#endif // HIGHLO_API_DX12

