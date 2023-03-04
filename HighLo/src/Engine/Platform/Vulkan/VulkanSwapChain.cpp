// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanSwapChain.h"
#include "VulkanContext.h"

#ifdef HIGHLO_API_VULKAN

#define HL_GET_INSTANCE_PROC_ADDR(inst, entrypoint)                        \
{                                                                       \
	fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk"#entrypoint)); \
	HL_ASSERT(fp##entrypoint);                                     \
}

#define HL_GET_DEVICE_PROC_ADDR(dev, entrypoint)                           \
{                                                                       \
	fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk"#entrypoint));   \
	HL_ASSERT(fp##entrypoint);                                     \
}

static PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
static PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
static PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
static PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
static PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
static PFN_vkQueuePresentKHR fpQueuePresentKHR;

// Nvidia extensions
PFN_vkCmdSetCheckpointNV fpCmdSetCheckpointNV;
PFN_vkGetQueueCheckpointDataNV fpGetQueueCheckpointDataNV;

VKAPI_ATTR void VKAPI_CALL vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void *pCheckpointMarker)
{
	fpCmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}

VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointDataNV(VkQueue queue, uint32 *pCheckpointDataCount, VkCheckpointDataNV *pCheckpointData)
{
	fpGetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}

namespace highlo
{	
	void VulkanSwapChain::Init(const Ref<RenderingContext> &context)
	{
		m_CurrentContext = context;
		m_Instance = VulkanContext::GetInstance();
		m_Device = VulkanContext::GetCurrentDevice();
		VkDevice vulkanDevice = m_Device->GetVulkanDevice();

		HL_GET_DEVICE_PROC_ADDR(vulkanDevice, CreateSwapchainKHR);
		HL_GET_DEVICE_PROC_ADDR(vulkanDevice, DestroySwapchainKHR);
		HL_GET_DEVICE_PROC_ADDR(vulkanDevice, GetSwapchainImagesKHR);
		HL_GET_DEVICE_PROC_ADDR(vulkanDevice, AcquireNextImageKHR);
		HL_GET_DEVICE_PROC_ADDR(vulkanDevice, QueuePresentKHR);

		HL_GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceSupportKHR);
		HL_GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
		HL_GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceFormatsKHR);
		HL_GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfacePresentModesKHR);

		HL_GET_INSTANCE_PROC_ADDR(m_Instance, CmdSetCheckpointNV);
		HL_GET_INSTANCE_PROC_ADDR(m_Instance, GetQueueCheckpointDataNV);
	}
	
	void VulkanSwapChain::InitSurface(void *windowHandle)
	{
		m_NativeWindowHandle = windowHandle;
	}
	
	void VulkanSwapChain::Create(uint32 *width, uint32 *height, bool vsync)
	{
	}
	
	void VulkanSwapChain::Cleanup()
	{
	}
	
	void VulkanSwapChain::OnResize(uint32 width, uint32 height)
	{
	}
	
	void VulkanSwapChain::BeginFrame()
	{
	}
	
	void VulkanSwapChain::EndFrame()
	{
		Present();
	}
	
	void VulkanSwapChain::Present()
	{
	}
	
	uint32 VulkanSwapChain::AcquireNextImage()
	{
		return 0;
	}

	void VulkanSwapChain::FindImageFormatAndColorSpace()
	{
	}
}

#endif // HIGHLO_API_VULKAN

