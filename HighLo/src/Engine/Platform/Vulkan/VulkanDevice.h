// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/Device.h"
#include "Engine/Graphics/PhysicalDevice.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

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

		// TODO: Maybe extract the begin parameter into a new function like CreateNewCommandBuffer? 
		// or GetCommandBuffer for this version and CreateCommandBuffer for the starting version?
		VkCommandBuffer CreateCommandBuffer(bool begin, bool compute = false);
		VkCommandBuffer CreateSecondaryCommandBuffer(const HLString &debugName = "") const;

		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);

		const Ref<VulkanPhysicalDevice> &GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetNativeDevice() const { return m_Device; }

	private:

		VkDevice m_Device = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		VkPhysicalDeviceFeatures m_EnabledFeatures; // TODO: Move this into physical device?

		VkCommandPool m_CommandPool = nullptr;
		VkCommandPool m_ComputeCommandPool = nullptr;

		VkQueue m_GraphicsQueue = nullptr;
		VkQueue m_ComputeQueue = nullptr;

		bool m_EnableDebugMarkers = false;
	};
}

#endif // HIGHLO_API_VULKAN

