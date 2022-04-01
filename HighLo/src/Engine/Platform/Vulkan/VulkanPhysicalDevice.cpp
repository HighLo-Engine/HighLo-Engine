#include "HighLoPch.h"
#include "VulkanPhysicalDevice.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanPhysicalDevice::VulkanPhysicalDevice()
    {
    }

    VulkanPhysicalDevice::~VulkanPhysicalDevice()
    {
    }
    
    uint32 VulkanPhysicalDevice::GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags properties) const
    {
        return uint32();
    }
    
    Ref<VulkanPhysicalDevice> VulkanPhysicalDevice::Create()
    {
        return Ref<VulkanPhysicalDevice>::Create();
    }
}

#endif // HIGHLO_API_VULKAN

