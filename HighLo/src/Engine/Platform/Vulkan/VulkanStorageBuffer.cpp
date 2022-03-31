#include "HighLoPch.h"
#include "VulkanStorageBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanStorageBuffer::VulkanStorageBuffer(uint32 size, uint32 binding)
    {
    }

    VulkanStorageBuffer::~VulkanStorageBuffer()
    {
    }
    
    void VulkanStorageBuffer::Bind() const
    {
    }
    
    void VulkanStorageBuffer::Unbind() const
    {
    }
    
    void VulkanStorageBuffer::SetData(const void *data, uint32 size, uint32 offset)
    {
    }
    
    void VulkanStorageBuffer::Resize(uint32 size)
    {
    }
    
    uint32 VulkanStorageBuffer::GetBinding() const
    {
        return uint32();
    }
}

#endif // HIGHLO_API_VULKAN

