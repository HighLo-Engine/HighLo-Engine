#include "HighLoPch.h"
#include "VulkanDevice.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanDevice::VulkanDevice(const Ref<VulkanPhysicalDevice> &physicalDevice, VkPhysicalDeviceFeatures enabledFeatures)
    {
    }

    VulkanDevice::~VulkanDevice()
    {
    }
    
    void VulkanDevice::Destroy()
    {
    }
    
    VkCommandBuffer VulkanDevice::CreateCommandBuffer(bool begin, bool compute)
    {
        return VkCommandBuffer();
    }
    
    VkCommandBuffer VulkanDevice::CreateSecondaryCommandBuffer(const HLString &debugName) const
    {
        return VkCommandBuffer();
    }
    
    void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
    {
    }
    
    void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
    {
    }
}

#endif // HIGHLO_API_VULKAN

