// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLSwapChain.h"

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
	}
	
	void OpenGLSwapChain::EndFrame()
	{
	}
	
	void OpenGLSwapChain::Present()
	{
		m_Context->SwapBuffers();
	}
}

#endif // HIGHLO_API_OPENGL

