#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanUniformBuffer::VulkanUniformBuffer(uint32 size, uint32 binding)
    {
    }
    VulkanUniformBuffer::~VulkanUniformBuffer()
    {
    }
    void VulkanUniformBuffer::Bind() const
    {
    }
    void VulkanUniformBuffer::Unbind() const
    {
    }
    void VulkanUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
    {
    }
    uint32 VulkanUniformBuffer::GetBinding() const
    {
        return uint32();
    }
}

#endif // HIGHLO_API_VULKAN

