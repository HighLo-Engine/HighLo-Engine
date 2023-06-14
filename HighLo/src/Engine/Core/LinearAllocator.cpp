// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "LinearAllocator.h"

namespace highlo
{
	void LinearAllocator::Init(uint64 maxSize, void *memory)
	{
		m_TotalSize = maxSize;
		m_Allocated = 0;
		m_OwnsMemory = memory == nullptr;

		if (memory)
		{
			m_Memory = memory;
		}
		else
		{
			m_Memory = malloc(maxSize);
		}
	}
	
	void LinearAllocator::Shutdown()
	{
		if (m_OwnsMemory)
		{
			free(m_Memory);
			m_Memory = nullptr;
		}

		m_Allocated = 0;
		m_TotalSize = 0;
		m_OwnsMemory = false;
	}
	
	void *LinearAllocator::Allocate(uint64 size)
	{
		if (!m_Memory)
		{
			HL_CORE_ERROR("LinearAllocator: Allocator not initialized!");
			return nullptr;
		}

		if ((m_Allocated + size) > m_TotalSize)
		{
			uint32 remaining = m_TotalSize - m_Allocated;
			HL_CORE_ERROR("LinearAllocator: Tried to allocate {0}, only {1} remaining.", size, remaining);
			return nullptr;
		}

		void *block = ((uint8*)m_Memory) + m_Allocated;
		m_Allocated += size;
		return block;
	}
	
	void LinearAllocator::FreeAll()
	{
		m_Allocated = 0;
		memset(m_Memory, 0, m_TotalSize);
	}
}

