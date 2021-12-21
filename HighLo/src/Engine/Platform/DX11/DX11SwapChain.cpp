#include "HighLoPch.h"
#include "DX11SwapChain.h"

namespace highlo
{
	DX11SwapChain::DX11SwapChain()
	{
	}
	
	DX11SwapChain::~DX11SwapChain()
	{
	}
	
	void DX11SwapChain::Init(const Ref<RenderingContext> &context)
	{
	}
	
	void DX11SwapChain::InitSurface(void *windowHandle)
	{
	}
	
	void DX11SwapChain::Create(uint32 *width, uint32 *height, bool vsync)
	{
	}
	
	void DX11SwapChain::Cleanup()
	{
	}
	
	void DX11SwapChain::OnResize(uint32 width, uint32 height)
	{
	}
	
	void DX11SwapChain::BeginFrame()
	{
	}
	
	void DX11SwapChain::EndFrame()
	{
	}
	
	void DX11SwapChain::Present()
	{
	}
	
	uint32 DX11SwapChain::GetImageCount() const
	{
		return 0;
	}
	
	uint32 DX11SwapChain::GetWidth() const
	{
		return 0;
	}
	
	uint32 DX11SwapChain::GetHeight() const
	{
		return 0;
	}
	
	uint32 DX11SwapChain::GetCurrentBufferIndex() const
	{
		return 0;
	}
}

