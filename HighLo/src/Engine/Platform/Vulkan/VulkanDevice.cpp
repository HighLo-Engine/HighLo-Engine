#include "HighLoPch.h"
#include "VulkanDevice.h"

#define DEFAULT_FENCE_TIMEOUT 100000000000

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanDevice::VulkanDevice(const Ref<VulkanPhysicalDevice> &physicalDevice, VkPhysicalDeviceFeatures enabledFeatures)
        : m_PhysicalDevice(physicalDevice), m_Features(enabledFeatures)
    {
        std::vector<const char*> deviceExtensions;
        VkDeviceCreateInfo deviceCreateInfo = {};
        VkPhysicalDeviceFeatures2 nextPhysicalDeviceFeatures{};
        VkCommandPoolCreateInfo cmdPoolInfo = {};
        VkResult result;

        HL_ASSERT(m_PhysicalDevice->IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME));
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        if (m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME))
            deviceExtensions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);

        if (m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME))
            deviceExtensions.push_back(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME);

        if (m_PhysicalDevice->IsExtensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
        {
            deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
            m_EnableDebugMarkers = true;
        }

        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = (uint32)(m_PhysicalDevice->m_QueueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = m_PhysicalDevice->m_QueueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

        if (deviceExtensions.size() > 0)
        {
            deviceCreateInfo.enabledExtensionCount = (uint32)deviceExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        }

        result = vkCreateDevice(m_PhysicalDevice->GetNativePhysicalDevice(), &deviceCreateInfo, nullptr, &m_LogicalDevice);
        HL_ASSERT(result == VK_SUCCESS);

        // Create CommandBuffer
        cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolInfo.queueFamilyIndex = m_PhysicalDevice->m_QueueFamilyIndices.Graphics;
        cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VK_CHECK_RESULT(vkCreateCommandPool(m_LogicalDevice, &cmdPoolInfo, nullptr, &m_CommandPool));

        // Create Compute CommandPool
        cmdPoolInfo.queueFamilyIndex = m_PhysicalDevice->m_QueueFamilyIndices.Compute;
        VK_CHECK_RESULT(vkCreateCommandPool(m_LogicalDevice, &cmdPoolInfo, nullptr, &m_ComputeCommandPool));

        // Create Queues
        vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->m_QueueFamilyIndices.Graphics, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->m_QueueFamilyIndices.Compute, 0, &m_ComputeQueue);
    }

    VulkanDevice::~VulkanDevice()
    {
    }

    void VulkanDevice::Destroy()
    {
        vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
        vkDestroyCommandPool(m_LogicalDevice, m_ComputeCommandPool, nullptr);

        vkDeviceWaitIdle(m_LogicalDevice);
        vkDestroyDevice(m_LogicalDevice, nullptr);
    }

    VkCommandBuffer VulkanDevice::CreateCommandBuffer(bool begin, bool compute)
    {
        VkCommandBuffer cmdBuffer;
        VkCommandBufferAllocateInfo cmdBufAllocInfo = {};

        cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocInfo.commandPool = compute ? m_ComputeCommandPool : m_CommandPool;
        cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdBufAllocInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_LogicalDevice, &cmdBufAllocInfo, &cmdBuffer));

        if (begin)
        {
            VkCommandBufferBeginInfo cmdBufferBeginInfo{};
            cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo));
        }

        return cmdBuffer;
    }

    VkCommandBuffer VulkanDevice::CreateSecondayCommandBuffer()
    {
        VkCommandBuffer cmdBuffer;
        VkCommandBufferAllocateInfo cmdBufAllocInfo = {};

        cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocInfo.commandPool = m_CommandPool;
        cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        cmdBufAllocInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_LogicalDevice, &cmdBufAllocInfo, &cmdBuffer));
        return cmdBuffer;
    }

    void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
    {
        FlushCommandBuffer(commandBuffer, m_GraphicsQueue);
    }

    void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
    {
        VkSubmitInfo submitInfo = {};
        VkFenceCreateInfo fenceCreateInfo = {};
        VkFence fence;

        HL_ASSERT(commandBuffer != VK_NULL_HANDLE);
        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        // Create fence to mark the command buffer's execution endpoint
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = 0;
        VK_CHECK_RESULT(vkCreateFence(m_LogicalDevice, &fenceCreateInfo, nullptr, &fence));

        VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));                                   // Submit into the queue
        VK_CHECK_RESULT(vkWaitForFences(m_LogicalDevice, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));   // Wait for the fence to signal that command buffer has finished executing

        vkDestroyFence(m_LogicalDevice, fence, nullptr);
        vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, &commandBuffer);
    }
}

#endif // HIGHLO_API_VULKAN

