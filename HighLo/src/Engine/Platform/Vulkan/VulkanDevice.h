// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#include "Vulkan.h"
#include "VulkanPhysicalDevice.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanDevice : public IsSharedReference
	{
	public:

		VulkanDevice(const Ref<VulkanPhysicalDevice> &physicalDevice, VkPhysicalDeviceFeatures enabledFeatures);
		~VulkanDevice();

		void Destroy();

		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetComputeQueue() { return m_ComputeQueue; }

		VkCommandBuffer CreateCommandBuffer(bool begin, bool compute = false);
		VkCommandBuffer CreateSecondaryCommandBuffer();

		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);

		const Ref<VulkanPhysicalDevice> &GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetNativeDevice() const { return m_LogicalDevice; }

	private:

		VkDevice m_LogicalDevice = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;

		VkPhysicalDeviceFeatures m_Features;
		VkCommandPool m_CommandPool, m_ComputeCommandPool;

		bool m_EnableDebugMarkers = false;
		VkQueue m_GraphicsQueue, m_ComputeQueue;
	};
}

#endif // HIGHLO_API_VULKAN

