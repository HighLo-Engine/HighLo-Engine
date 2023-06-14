// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalContext.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalContext::MetalContext(void *handle, WindowData &data)
		: m_MetalWindowHandle(handle)
	{
	}

	MetalContext::~MetalContext()
	{
	}
	
	void MetalContext::Init()
	{
	}
	
	void MetalContext::SwapBuffers()
	{
	}
	
	void MetalContext::MakeCurrent()
	{
	}
	
	void MetalContext::SetSwapInterval(bool bEnabled)
	{
	}
}

#endif // HIGHLO_API_METAL

