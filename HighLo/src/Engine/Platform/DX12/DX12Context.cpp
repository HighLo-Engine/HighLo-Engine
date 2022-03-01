// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Context.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12Context::DX12Context(void *handle, WindowData &data)
		: m_DX12WindowHandle(handle), m_WindowData(data)
	{
	}
	
	DX12Context::~DX12Context()
	{
	}

	void DX12Context::Init()
	{
	}

	void DX12Context::SwapBuffers()
	{
	}

	void DX12Context::MakeCurrent()
	{
	}

	void DX12Context::SetSwapInterval(bool bEnabled)
	{
	}

	void *DX12Context::GetCurrentContext()
	{
		return nullptr;
	}
}

#endif // HIGHLO_API_DX12

