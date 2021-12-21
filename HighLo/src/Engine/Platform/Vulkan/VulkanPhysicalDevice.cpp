// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanPhysicalDevice.h"

#include "VulkanContext.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{
		static const float defaultQueuePriority(0.0f);
		auto vkInstance = VulkanContext::GetInstance();
		std::vector<VkPhysicalDevice> physicalDevices;
		VkPhysicalDevice selectedPhysicalDevice;
		VkPhysicalDevice fallbackPhysicalDevice;
		uint32 gpuCount = 0;
		uint32 queueFamilyCount = 0;
		uint32 extCount = 0;
		int32 requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

		// Get Total count of gpus connected to the os
		vkEnumeratePhysicalDevices(vkInstance, &gpuCount, nullptr);
		HL_ASSERT(gpuCount > 0);
		physicalDevices.resize(gpuCount);
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(vkInstance, &gpuCount, physicalDevices.data()));

		// Select the best gpu for rendering
		for (VkPhysicalDevice physicalDevice : physicalDevices)
		{
			vkGetPhysicalDeviceProperties(physicalDevice, &m_Properties);
			if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				HL_CORE_INFO("Preferred GPU found - Selected discrete rendering GPU device {0}", m_Properties.deviceName);
				selectedPhysicalDevice = physicalDevice;
				break;
			}
			else if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
			{
				HL_CORE_INFO("No GPU found, rendering with CPU device {0}", m_Properties.deviceName);
			}
			else if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			{
				HL_CORE_INFO("Found a integrated GPU device {0}", m_Properties.deviceName);
				fallbackPhysicalDevice = physicalDevice;
			}
		}

		if (!selectedPhysicalDevice && fallbackPhysicalDevice)
		{
			HL_CORE_WARN("Could not select the preferred GPU! Falling back to the integrated GPU Renderer!");
			selectedPhysicalDevice = fallbackPhysicalDevice;
		}
		else if (!selectedPhysicalDevice && !fallbackPhysicalDevice)
		{
			HL_CORE_WARN("Could not select either the preferred GPU nor the fallback GPU! Falling back to the last registered GPU!");
			selectedPhysicalDevice = physicalDevices.back();
		}
		
		HL_ASSERT(selectedPhysicalDevice, "Could not find any GPUs!");
		m_PhysicalDevice = selectedPhysicalDevice;

		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		HL_ASSERT(queueFamilyCount > 0);
		m_QueueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());

		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, nullptr);
		if (extCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
			{
				HL_CORE_TRACE("Selected GPU has {0} extensions!", extensions.size());
				for (const auto &ext : extensions)
				{
					m_SupportedExtensions.emplace(ext.extensionName);
					HL_CORE_TRACE("   {0}", ext.extensionName);
				}
			}
		}

		// Queue Families

		m_QueueFamilyIndices = GetQueueFamilyIndices(requestedQueueTypes);

		if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			m_QueueCreateInfos.push_back(queueInfo);
		}

		if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			if (m_QueueFamilyIndices.Compute != m_QueueFamilyIndices.Graphics)
			{
				// If compute family index differs, we need an additional queue create info for the compute queue
				VkDeviceQueueCreateInfo queueInfo{};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;
				m_QueueCreateInfos.push_back(queueInfo);
			}
		}

		if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			if ((m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Graphics) && (m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Compute))
			{
				// If compute family index differs, we need an additional queue create info for the compute queue
				VkDeviceQueueCreateInfo queueInfo{};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Transfer;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;
				m_QueueCreateInfos.push_back(queueInfo);
			}
		}

		m_DepthFormat = FindDepthFormat();
		HL_ASSERT(m_DepthFormat);
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{
	}

	bool VulkanPhysicalDevice::IsExtensionSupported(const HLString &extension) const
	{
		return m_SupportedExtensions.find(extension) != m_SupportedExtensions.end();
	}

	uint32 VulkanPhysicalDevice::GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags propertyFlags) const
	{
		for (uint32 i = 0; i < m_MemoryProperties.memoryTypeCount; ++i)
		{
			if ((typeBits & 1) == 1)
			{
				if ((m_MemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
					return i;
			}
			typeBits >>= 1;
		}

		return UINT32_MAX;
	}

	VkFormat VulkanPhysicalDevice::FindDepthFormat() const
	{
		// Since all depth formats may be optional, we need to find a suitable depth format to use
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

			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				return format;
		}

		return VK_FORMAT_UNDEFINED;
	}

	VulkanPhysicalDeviceQueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices(int32 flags)
	{
		VulkanPhysicalDeviceQueueFamilyIndices result;

		if (flags & VK_QUEUE_COMPUTE_BIT)
		{
			for (uint32 i = 0; i < m_QueueFamilyProperties.size(); ++i)
			{
				auto &queueFamilyProps = m_QueueFamilyProperties[i];
				if ((queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
				{
					result.Compute = 1;
					break;
				}
			}
		}

		if (flags & VK_QUEUE_TRANSFER_BIT)
		{
			for (uint32 i = 0; i < m_QueueFamilyProperties.size(); ++i)
			{
				auto &queueFamilyProps = m_QueueFamilyProperties[i];
				if (queueFamilyProps.queueFlags & VK_QUEUE_TRANSFER_BIT && ((queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
				{
					result.Transfer = 1;
					break;
				}
			}
		}

		for (uint32 i = 0; i < m_QueueFamilyProperties.size(); ++i)
		{
			if ((flags & VK_QUEUE_TRANSFER_BIT) && result.Transfer == -1)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
					result.Transfer = i;
			}

			if ((flags & VK_QUEUE_COMPUTE_BIT) && result.Compute == -1)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
					result.Compute = i;
			}

			if (flags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					result.Graphics = i;
			}
		}

		return result;
	}

	Ref<VulkanPhysicalDevice> VulkanPhysicalDevice::Select()
	{
		return Ref<VulkanPhysicalDevice>::Create();
	}
}

#endif // HIGHLO_API_VULKAN

