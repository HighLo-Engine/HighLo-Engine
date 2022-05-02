// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanDevice.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    VulkanDevice::VulkanDevice(const Ref<PhysicalDevice> &physicalDevice)
        : m_PhysicalDevice(physicalDevice.As<VulkanPhysicalDevice>())
    {
    }

    VulkanDevice::~VulkanDevice()
    {
    }
    
    void VulkanDevice::Destroy()
    {
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
        vkDestroyCommandPool(m_Device, m_ComputeCommandPool, nullptr);

        vkDeviceWaitIdle(m_Device);
        vkDestroyDevice(m_Device, nullptr);
    }

    void VulkanDevice::InitDevice(VkPhysicalDeviceFeatures enabledFeatures)
    {
        m_EnabledFeatures = enabledFeatures;

        const bool enableAftermath = true;

        // Do we need to enable any other extensions (eg. NV_RAYTRACING?)
        std::vector<const char *> deviceExtensions;
        // If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
        HL_ASSERT(m_PhysicalDevice->IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME));
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        if (m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME))
            deviceExtensions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
        if (m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME))
            deviceExtensions.push_back(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME);

        VkDeviceDiagnosticsConfigCreateInfoNV aftermathInfo = {};
        bool canEnableAftermath = enableAftermath
            && m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
            && m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME);

        if (canEnableAftermath)
        {
            // Must be initialized ~before~ device has been created
            // TODO: Add Nvidia aftermath lib here

            VkDeviceDiagnosticsConfigFlagBitsNV aftermathFlags = (VkDeviceDiagnosticsConfigFlagBitsNV)(VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_RESOURCE_TRACKING_BIT_NV |
                                                                                                       VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_AUTOMATIC_CHECKPOINTS_BIT_NV |
                                                                                                       VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_DEBUG_INFO_BIT_NV);

            aftermathInfo.sType = VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV;
            aftermathInfo.flags = aftermathFlags;
        }

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(m_PhysicalDevice->m_QueueCreateInfos.size());;
        deviceCreateInfo.pQueueCreateInfos = m_PhysicalDevice->m_QueueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
        if (canEnableAftermath)
            deviceCreateInfo.pNext = &aftermathInfo;

        // Enable the debug marker extension if it is present (likely meaning a debugging tool is present)
        if (m_PhysicalDevice->IsExtensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
        {
            deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
            m_EnableDebugMarkers = true;
        }

        if (deviceExtensions.size() > 0)
        {
            deviceCreateInfo.enabledExtensionCount = (uint32)deviceExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        }

        VkResult result = vkCreateDevice(m_PhysicalDevice->GetNativeDevice(), &deviceCreateInfo, nullptr, &m_Device);
        HL_ASSERT(result == VK_SUCCESS);

        VkCommandPoolCreateInfo cmdPoolInfo = {};
        cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolInfo.queueFamilyIndex = m_PhysicalDevice->m_QueueFamilyIndices.Graphics;
        cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VK_CHECK_RESULT(vkCreateCommandPool(m_Device, &cmdPoolInfo, nullptr, &m_CommandPool));

        cmdPoolInfo.queueFamilyIndex = m_PhysicalDevice->m_QueueFamilyIndices.Compute;
        VK_CHECK_RESULT(vkCreateCommandPool(m_Device, &cmdPoolInfo, nullptr, &m_ComputeCommandPool));

        // Get a graphics queue from the device
        vkGetDeviceQueue(m_Device, m_PhysicalDevice->m_QueueFamilyIndices.Graphics, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, m_PhysicalDevice->m_QueueFamilyIndices.Compute, 0, &m_ComputeQueue);
    }
    
    VkCommandBuffer VulkanDevice::CreateCommandBuffer(bool begin, bool compute)
    {
        VkCommandBuffer result;

        VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
        cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocateInfo.commandPool = compute ? m_ComputeCommandPool : m_CommandPool;
        cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdBufAllocateInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_Device, &cmdBufAllocateInfo, &result));

        // If requested, also start the new command buffer
        if (begin)
        {
            VkCommandBufferBeginInfo cmdBufferBeginInfo{};
            cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            VK_CHECK_RESULT(vkBeginCommandBuffer(result, &cmdBufferBeginInfo));
        }

        return result;
    }
    
    VkCommandBuffer VulkanDevice::CreateSecondaryCommandBuffer(const HLString &debugName) const
    {
        VkCommandBuffer result;

        VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
        cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocateInfo.commandPool = m_CommandPool;
        cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        cmdBufAllocateInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_Device, &cmdBufAllocateInfo, &result));
        utils::SetDebugUtilsObjectName(m_Device, VK_OBJECT_TYPE_COMMAND_BUFFER, debugName, result);

        return result;
    }
    
    void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
    {
        FlushCommandBuffer(commandBuffer, m_GraphicsQueue);
    }
    
    void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
    {
        const uint64 DEFAULT_FENCE_TIMEOUT = 100000000000;

        HL_ASSERT(commandBuffer != VK_NULL_HANDLE);

        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        // Create fence to ensure that the command buffer has finished executing
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = 0;
        VkFence fence;
        VK_CHECK_RESULT(vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &fence));

        // Submit to the queue
        VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
        // Wait for the fence to signal that command buffer has finished executing
        VK_CHECK_RESULT(vkWaitForFences(m_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));

        vkDestroyFence(m_Device, fence, nullptr);
        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
    }
}

#endif // HIGHLO_API_VULKAN

