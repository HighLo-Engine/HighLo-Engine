// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalSwapChain.h"

#ifdef HIGHLO_API_METAL

#include "Engine/Renderer/Renderer.h"

namespace highlo
{	
	void MetalSwapChain::Init(const Ref<RenderingContext> &context)
	{
		m_Context = context;
	}
	
	void MetalSwapChain::InitSurface(void *windowHandle)
	{
		m_NativeHandle = windowHandle;
	}
	
	void MetalSwapChain::Create(uint32 *width, uint32 *height, bool vsync)
	{
		m_Width = *width;
		m_Height = *height;
		m_VSync = vsync;
		m_Context->SetSwapInterval(vsync);
	}
	
	void MetalSwapChain::Cleanup()
	{
	}
	
	void MetalSwapChain::OnResize(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;
	}
	
	void MetalSwapChain::BeginFrame()
	{
		auto &queue = Renderer::GetRenderResourceReleaseQueue(m_CurrentBufferIndex);
		queue.Execute();
	}
	
	void MetalSwapChain::EndFrame()
	{
		Present();
	}
	
	void MetalSwapChain::Present()
	{
		m_Context->SwapBuffers();

		const auto &config = Renderer::GetConfig();
		m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % config.FramesInFlight;
	}
}

#endif // HIGHLO_API_METAL

