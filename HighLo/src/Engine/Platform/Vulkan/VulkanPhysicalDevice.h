// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	struct VulkanPhysicalDeviceQueueFamilyIndices
	{
		int32 Graphics = -1;
		int32 Compute  = -1;
		int32 Transfer = -1;
	};

	class VulkanPhysicalDevice : public IsSharedReference
	{
	public:

		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		bool IsExtensionSupported(const HLString &extension) const;
		uint32 GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags propertyFlags) const;

		VkPhysicalDevice GetNativePhysicalDevice() const { return m_PhysicalDevice; }
		const VulkanPhysicalDeviceQueueFamilyIndices &GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

		const VkPhysicalDeviceProperties &GetProperties() const { return m_Properties; }
		const VkPhysicalDeviceMemoryProperties &GetMemoryProperties() const { return m_MemoryProperties; }
		const VkPhysicalDeviceLimits &GetLimits() const { return m_Properties.limits; }

		VkFormat GetDepthFormat() const { return m_DepthFormat; }

		static Ref<VulkanPhysicalDevice> Select();

	private:

		VkFormat FindDepthFormat() const;
		VulkanPhysicalDeviceQueueFamilyIndices GetQueueFamilyIndices(int32 flags);

		VulkanPhysicalDeviceQueueFamilyIndices m_QueueFamilyIndices;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;
		std::unordered_set<HLString> m_SupportedExtensions;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;
		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;
		VkPhysicalDeviceFeatures m_Features;

		friend class VulkanDevice;
	};
}

#endif // HIGHLO_API_VULKAN

