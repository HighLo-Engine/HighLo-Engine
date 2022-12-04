// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Device.h"

#include "Vulkan.h"
#include "VulkanPhysicalDevice.h"

namespace highlo
{
	class VulkanDevice : public Device
	{
	public:

		VulkanDevice(const Ref<PhysicalDevice> &physicalDevice);
		virtual ~VulkanDevice();

		virtual void Destroy() override;

		// Vulkan-specific

		void InitDevice(VkPhysicalDeviceFeatures enabledFeatures);

		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetComputeQueue() { return m_ComputeQueue; }

		VkCommandBuffer GetCommandBuffer(bool begin, bool compute = false);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);

		VkCommandBuffer CreateSecondaryCommandBuffer(const HLString &debugName) const;

		const Ref<VulkanPhysicalDevice> &GetVulkanPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetNativeDevice() const { return m_LogicalDevice; }

	private:

		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;
		VkDevice m_LogicalDevice = nullptr;
		VkPhysicalDeviceFeatures m_EnabledFeatures;
		VkCommandPool m_CommandPool = nullptr, m_ComputeCommandPool = nullptr;

		VkQueue m_GraphicsQueue = nullptr;
		VkQueue m_ComputeQueue = nullptr;

		bool m_EnableDebugMarkers = false;
	};
}

#endif // HIGHLO_API_VULKAN

