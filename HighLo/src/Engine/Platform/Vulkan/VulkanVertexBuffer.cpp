// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanVertexBuffer::VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
        : m_Usage(usage)
    {
    }

    VulkanVertexBuffer::VulkanVertexBuffer(uint32 size, VertexBufferUsage usage)
        : m_Usage(usage)
    {
    }
    
    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
    }
    
    void VulkanVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
    {
    }
}

#endif // HIGHLO_API_VULKAN

