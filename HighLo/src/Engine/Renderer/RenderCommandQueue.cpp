// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderCommandQueue.h"

namespace highlo
{
	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandBuffer = new uint8[10 * 1024 * 1024];
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
		m_CommandBufferPtr = m_CommandBuffer;
	}
	
	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_CommandBuffer;
	}
	
	void *RenderCommandQueue::Allocate(RenderCommandFn func, uint32 size)
	{
		*(RenderCommandFn*)m_CommandBufferPtr = func;
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		*(uint32*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(uint32);

		void *memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		++m_CommandCount;
		return memory;
	}
	
	void RenderCommandQueue::Execute()
	{
		Byte *buffer = m_CommandBuffer;
		for (uint32 i = 0; i < m_CommandCount; ++i)
		{
			RenderCommandFn function = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			uint32 size = *(uint32*)buffer;
			buffer += sizeof(uint32);

			function(buffer);
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_CommandCount = 0;
	}
}

