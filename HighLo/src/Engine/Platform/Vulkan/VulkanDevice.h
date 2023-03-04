// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Device.h"
#include "Engine/Graphics/PhysicalDevice.h"
#include "Vulkan.h"

namespace highlo
{
	class VulkanCommandPool : public IsSharedReference
	{
	public:

		VulkanCommandPool();
		virtual ~VulkanCommandPool();

		VkCommandBuffer AllocateCommandBuffer(bool begin, bool compute = false);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);

		VkCommandPool GetGraphicsCommandPool() const { return m_GraphicsCommandPool; }
		VkCommandPool GetComputeCommandPool() const { return m_ComputeCommandPool; }

	private:
		
		VkCommandPool m_GraphicsCommandPool = VK_NULL_HANDLE;
		VkCommandPool m_ComputeCommandPool = VK_NULL_HANDLE;
	};

	class VulkanPhysicalDevice;

	class VulkanDevice : public Device
	{
	public:

		VulkanDevice(const Ref<PhysicalDevice> &physicalDevice);
		virtual ~VulkanDevice();

		virtual void Destroy() override;

		// Vulkan-specific
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetComputeQueue() { return m_ComputeQueue; }

		VkCommandBuffer GetCommandBuffer(bool begin, bool compute = false);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);

		VkCommandBuffer CreateSecondaryCommandBuffer(const HLString &debugName = "");

		const Ref<VulkanPhysicalDevice> &GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetVulkanDevice() const { return m_LogicalDevice; }
	
	private:
		
		Ref<VulkanCommandPool> GetThreadLocalCommandPool();
		Ref<VulkanCommandPool> GetOrCreateThreadLocalCommandPool();

	private:

		VkDevice m_LogicalDevice = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;
		VkPhysicalDeviceFeatures m_EnabledFeatures;

		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;

		// TODO: Refactor to own threading api
		std::map<std::thread::id, Ref<VulkanCommandPool>> m_CommandPools;
		bool m_EnableDebugMarkers = false;
	};

	class VulkanPhysicalDevice : public PhysicalDevice
	{
	public:

		struct QueueFamilyIndices
		{
			int32 Graphics = -1;
			int32 Compute = -1;
			int32 Transfer = -1;
		};

	public:

		VulkanPhysicalDevice();
		virtual ~VulkanPhysicalDevice();

		virtual bool IsExtensionSupported(const HLString &name) const override;

		// Vulkan-specific
		uint32 GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags properties) const;

		VkPhysicalDevice GetVulkanPhysicalDevice() const { return m_PhysicalDevice; }
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
		std::unordered_set<HLString> m_SupportedExtensions;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;

		friend class VulkanDevice;
	};
}

#endif // HIGHLO_API_VULKAN

