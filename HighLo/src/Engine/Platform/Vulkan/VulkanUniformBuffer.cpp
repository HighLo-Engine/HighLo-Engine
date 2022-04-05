#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    VulkanUniformBuffer::VulkanUniformBuffer(uint32 size, uint32 binding)
        : m_Size(size), m_Binding(binding)
    {
        Invalidate();
    }
    
    VulkanUniformBuffer::~VulkanUniformBuffer()
    {
        Release();
    }
    
    void VulkanUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
    {
        uint8 *dst = utils::MapMemory<uint8>(m_MemoryAllocation);
        memcpy(dst, (uint8*)data + offset, size);
        utils::UnmapMemory(m_MemoryAllocation);
    }

    void VulkanUniformBuffer::Release()
    {
        if (!m_MemoryAllocation)
            return;

        utils::DestroyBuffer(m_Buffer, m_MemoryAllocation);

        m_Buffer = nullptr;
        m_MemoryAllocation = nullptr;
    }
    
    void VulkanUniformBuffer::Invalidate()
    {
        Release();

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.size = m_Size;

        m_MemoryAllocation = utils::AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_Buffer);

        m_DescriptorBufferInfo.buffer = m_Buffer;
        m_DescriptorBufferInfo.offset = 0;
        m_DescriptorBufferInfo.range = m_Size;
    }
}

#endif // HIGHLO_API_VULKAN

