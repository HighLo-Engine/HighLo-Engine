// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanDevice.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanCommandPool::VulkanCommandPool()
	{
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
	}

	VkCommandBuffer VulkanCommandPool::AllocateCommandBuffer(bool begin, bool compute)
	{
		return VkCommandBuffer();
	}

	void VulkanCommandPool::FlushCommandBuffer(VkCommandBuffer commandBuffer)
	{
	}

	void VulkanCommandPool::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
	{
	}



	VulkanDevice::VulkanDevice(const Ref<PhysicalDevice> &physicalDevice)
		: m_PhysicalDevice(physicalDevice)
	{

	}

	VulkanDevice::~VulkanDevice()
	{
		Destroy();
	}

	void VulkanDevice::Destroy()
	{
		m_PhysicalDevice = nullptr; // decrement ref count


	}

	VkCommandBuffer VulkanDevice::GetCommandBuffer(bool begin, bool compute)
	{
		return VkCommandBuffer();
	}

	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
	{
	}

	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
	{
	}

	VkCommandBuffer VulkanDevice::CreateSecondaryCommandBuffer(const HLString &debugName)
	{
		return VkCommandBuffer();
	}

	Ref<VulkanCommandPool> VulkanDevice::GetThreadLocalCommandPool()
	{
		return Ref<VulkanCommandPool>();
	}

	Ref<VulkanCommandPool> VulkanDevice::GetOrCreateThreadLocalCommandPool()
	{
		return Ref<VulkanCommandPool>();
	}



	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{

	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}

	bool VulkanPhysicalDevice::IsExtensionSupported(const HLString &name) const
	{
		return true;
	}
}

#endif // HIGHLO_API_VULKAN

