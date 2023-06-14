// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12SwapChain.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12SwapChain::DX12SwapChain()
	{
	}

	DX12SwapChain::~DX12SwapChain()
	{
	}
	
	void DX12SwapChain::Init(const Ref<RenderingContext> &context)
	{
	}
	
	void DX12SwapChain::InitSurface(void *windowHandle)
	{
	}
	
	void DX12SwapChain::Create(uint32 *width, uint32 *height, bool vsync)
	{
	}
	
	void DX12SwapChain::Cleanup()
	{
	}
	
	void DX12SwapChain::OnResize(uint32 width, uint32 height)
	{
	}
	
	void DX12SwapChain::BeginFrame()
	{
	}
	
	void DX12SwapChain::EndFrame()
	{
	}
	
	void DX12SwapChain::Present()
	{
	}
	
	uint32 DX12SwapChain::GetImageCount() const
	{
		return uint32();
	}
	
	uint32 DX12SwapChain::GetWidth() const
	{
		return uint32();
	}
	
	uint32 DX12SwapChain::GetHeight() const
	{
		return uint32();
	}
	
	uint32 DX12SwapChain::GetCurrentBufferIndex() const
	{
		return uint32();
	}
}

#endif // HIGHLO_API_DX12

