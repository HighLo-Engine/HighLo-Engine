// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanPhysicalDevice.h"

#define VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "Device>       "

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanContext.h"

namespace highlo
{
    namespace utils
    {
        static VkPhysicalDevice SelectGPU(const std::vector<VkPhysicalDevice> &devices, VkPhysicalDeviceType type)
        {
            VkPhysicalDevice result = nullptr;

            for (VkPhysicalDevice device : devices)
            {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(device, &properties);
                if (properties.deviceType == type)
                {
                    result = device;
                    break;
                }
            }

            return result;
        }
    }

    static const float s_DefaultQueuePriority = 0.0f;

    VulkanPhysicalDevice::VulkanPhysicalDevice()
    {
        auto instance = VulkanContext::GetInstance();

        // Get the GPU count
        uint32 gpuCount = 0;
        vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
        HL_ASSERT(gpuCount > 0, "No GPUs could be found attached to your system!");

        // Get all gpus
        std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()));

        // Select the gpu we want to render on (try the best one first and then try to select integrated gpus and cpus)
        VkPhysicalDevice selectedPhysicalDevice = nullptr;
        selectedPhysicalDevice = utils::SelectGPU(physicalDevices, VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

        // set Fallback gpu
        if (!selectedPhysicalDevice)
        {
            HL_CORE_WARN(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[-] Could not find a discrete GPU! Trying to select an integrated GPU. [-]");
            selectedPhysicalDevice = utils::SelectGPU(physicalDevices, VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
            
            if (!selectedPhysicalDevice)
            {
                HL_CORE_WARN(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[-] Could not find an integrated GPU. Trying to select a CPU. [-]");
                selectedPhysicalDevice = utils::SelectGPU(physicalDevices, VK_PHYSICAL_DEVICE_TYPE_CPU);
                if (!selectedPhysicalDevice)
                {
                    HL_CORE_ERROR(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[-] Could not find cpu renderer. [-]");
                    selectedPhysicalDevice = physicalDevices.back();
                }
                else
                {
                    HL_CORE_INFO(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[+] Selected a CPU successfully. [+]");
                }
            }
            else
            {
                HL_CORE_INFO(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[+] Selected an integrated GPU successfully. [+]");
            }
        }
        else
        {
            HL_CORE_INFO(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[+] Selected a discrete GPU successfully. [+]");
        }

        // we have a huge problem if selectedPhysicalDevice is still nullptr, as of there are no registered gpus on the system then...
        HL_ASSERT(selectedPhysicalDevice, "You should never come here, but if you do you probably don't have any hardware you can render on!");

        // Get the properties of the selected gpu
        m_PhysicalDevice = selectedPhysicalDevice;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
        vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

        uint32 queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
        HL_ASSERT(queueFamilyCount > 0);

        m_QueueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());

        uint32 extCount = 0;
        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, nullptr);
        if (extCount > 0)
        {
            std::vector<VkExtensionProperties> extensions(extCount);
            if (vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
            {
                HL_CORE_TRACE(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[+] Selected physical device has {0} extensions [+]", extensions.size());
                for (const auto &ext : extensions)
                {
                    m_SupportedExtensions.emplace(ext.extensionName);
                    HL_CORE_TRACE(VULKAN_PHYSICAL_DEVICE_LOG_PREFIX "[+]    {0} [+]", ext.extensionName);
                }
            }
        }

        int32 requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
        m_QueueFamilyIndices = GetQueueFamilyIndices(requestedQueueTypes);

        // Graphics queue
        if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
        {
            VkDeviceQueueCreateInfo queueInfo{};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &s_DefaultQueuePriority;
            m_QueueCreateInfos.push_back(queueInfo);
        }

        // Dedicated compute queue
        if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
        {
            if (m_QueueFamilyIndices.Compute != m_QueueFamilyIndices.Graphics)
            {
                // If compute family index differs, we need an additional queue create info for the compute queue
                VkDeviceQueueCreateInfo queueInfo{};
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &s_DefaultQueuePriority;
                m_QueueCreateInfos.push_back(queueInfo);
            }
        }

        // Dedicated transfer queue
        if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
        {
            if ((m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Graphics) && (m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Compute))
            {
                // If compute family index differs, we need an additional queue create info for the compute queue
                VkDeviceQueueCreateInfo queueInfo{};
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Transfer;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &s_DefaultQueuePriority;
                m_QueueCreateInfos.push_back(queueInfo);
            }
        }

        m_DepthFormat = FindDepthFormat();
        HL_ASSERT(m_DepthFormat);
    }

    VulkanPhysicalDevice::~VulkanPhysicalDevice()
    {
    }
    
    uint32 VulkanPhysicalDevice::GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags properties) const
    {
        // Iterate over all memory types available for the device used in this example
        for (uint32 i = 0; i < m_MemoryProperties.memoryTypeCount; ++i)
        {
            if ((typeBits & 1) == 1)
            {
                if ((m_MemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                    return i;
            }
            typeBits >>= 1;
        }

        HL_ASSERT(false, "Could not find a suitable memory type!");
        return UINT32_MAX;
    }

    // Since all depth formats may be optional, we need to find a suitable depth format to use
    VkFormat VulkanPhysicalDevice::FindDepthFormat() const
    {
        // Start with the highest precision packed format
        std::vector<VkFormat> depthFormats = {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM
        };

        for (auto &format : depthFormats)
        {
            VkFormatProperties formatProps;
            vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &formatProps);
            // Format must support depth stencil attachment for optimal tiling
            if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
                return format;
        }

        return VK_FORMAT_UNDEFINED;
    }
    
    QueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices(int32 queueFlags)
    {
        QueueFamilyIndices result;

        // Dedicated queue for compute
        // Try to find a queue family index that supports compute but not graphics
        if (queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for (uint32 i = 0; i < m_QueueFamilyProperties.size(); ++i)
            {
                auto &queueFamilyProperties = m_QueueFamilyProperties[i];
                if ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
                {
                    result.Compute = i;
                    break;
                }
            }
        }

        // Dedicated queue for transfer
        // Try to find a queue family index that supports transfer but not graphics and compute
        if (queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            for (uint32 i = 0; i < m_QueueFamilyProperties.size(); ++i)
            {
                auto &queueFamilyProperties = m_QueueFamilyProperties[i];
                if ((queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
                {
                    result.Transfer = i;
                    break;
                }
            }
        }

        // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
        for (uint32 i = 0; i < m_QueueFamilyProperties.size(); ++i)
        {
            if ((queueFlags & VK_QUEUE_TRANSFER_BIT) && result.Transfer == -1)
            {
                if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
                    result.Transfer = i;
            }

            if ((queueFlags & VK_QUEUE_COMPUTE_BIT) && result.Compute == -1)
            {
                if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                    result.Compute = i;
            }

            if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                    result.Graphics = i;
            }
        }

        return result;
    }
}

#endif // HIGHLO_API_VULKAN

