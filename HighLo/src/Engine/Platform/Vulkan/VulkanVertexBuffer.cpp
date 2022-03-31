#include "HighLoPch.h"
#include "VulkanVertexBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanVertexBuffer::VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
        : m_Usage(usage), m_Size(size)
    {
    }

    VulkanVertexBuffer::VulkanVertexBuffer(uint32 size, VertexBufferUsage usage)
        : m_Usage(usage), m_Size(size)
    {
    }
    
    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
    }
    
    void VulkanVertexBuffer::Bind() const
    {
    }
    
    void VulkanVertexBuffer::Unbind() const
    {
    }
    
    void VulkanVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
    {
    }
}

#endif // HIGHLO_API_VULKAN

