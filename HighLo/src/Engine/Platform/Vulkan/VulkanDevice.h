#pragma once

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "VulkanPhysicalDevice.h"

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

		// TODO: Maybe extract the begin parameter into a new function like CreateNewCommandBuffer? 
		// or GetCommandBuffer for this version and CreateCommandBuffer for the starting version?
		VkCommandBuffer CreateCommandBuffer(bool begin, bool compute = false);
		VkCommandBuffer CreateSecondaryCommandBuffer(const HLString &debugName) const;

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

