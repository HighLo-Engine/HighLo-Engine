// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLSwapChain.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	void OpenGLSwapChain::Init(const Ref<RenderingContext> &context)
	{
		m_Context = context;
	}
	
	void OpenGLSwapChain::InitSurface(void *windowHandle)
	{
		m_NativeHandle = windowHandle;
	}
	
	void OpenGLSwapChain::Create(uint32 *width, uint32 *height, bool vsync)
	{
		m_Width = *width;
		m_Height = *height;
		m_VSync = vsync;
		m_Context->SetSwapInterval(vsync);
	}
	
	void OpenGLSwapChain::Cleanup()
	{
	}
	
	void OpenGLSwapChain::OnResize(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;
	}
	
	void OpenGLSwapChain::BeginFrame()
	{
		auto &queue = Renderer::GetRenderResourceReleaseQueue(m_CurrentBufferIndex);
		queue.Execute();
	}
	
	void OpenGLSwapChain::EndFrame()
	{
		Present();
	}
	
	void OpenGLSwapChain::Present()
	{
		m_Context->SwapBuffers();

		const auto &config = Renderer::GetConfig();
		m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % config.FramesInFlight;
	}
}

#endif // HIGHLO_API_OPENGL

