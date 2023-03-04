// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/PhysicalDevice.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

namespace highlo
{
	struct QueueFamilyIndices
	{
		int32 Graphics = -1;
		int32 Transfer = -1;
		int32 Compute = -1;
	};

	class VulkanPhysicalDevice : public PhysicalDevice
	{
	public:

		VulkanPhysicalDevice();
		virtual ~VulkanPhysicalDevice();

		virtual bool IsExtensionSupported(const HLString &name) const override { return m_SupportedExtensions.find(name) != m_SupportedExtensions.end(); }
		uint32 GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags properties) const;

		VkPhysicalDevice GetNativeDevice() const { return m_PhysicalDevice; }
		const QueueFamilyIndices &GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

		const VkPhysicalDeviceProperties &GetProperties() const { return m_Properties; }
		const VkPhysicalDeviceLimits &GetLimits() const { return m_Properties.limits; }
		const VkPhysicalDeviceMemoryProperties &GetMemoryProperties() const { return m_MemoryProperties; }

		VkFormat GetDepthFormat() const { return m_DepthFormat; }

	private:

		VkFormat FindDepthFormat() const;
		QueueFamilyIndices GetQueueFamilyIndices(int32 queueFlags);

	private:

		QueueFamilyIndices m_QueueFamilyIndices;

		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;
		std::unordered_set<HLString> m_SupportedExtensions;

		friend class VulkanDevice;
	};
}

#endif // HIGHLO_API_VULKAN

