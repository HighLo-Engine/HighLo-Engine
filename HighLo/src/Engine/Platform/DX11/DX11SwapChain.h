#pragma once

#include "Engine/Renderer/SwapChain.h"

namespace highlo
{
	class DX11SwapChain : public SwapChain
	{
	public:

		DX11SwapChain();
		virtual ~DX11SwapChain();

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

