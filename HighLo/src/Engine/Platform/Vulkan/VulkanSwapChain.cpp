// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanSwapChain.h"

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#endif // HIGHLO_API_GLFW

#ifdef HIGHLO_API_VULKAN

static PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
static PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
static PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
static PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
static PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
static PFN_vkQueuePresentKHR fpQueuePresentKHR;

#include "VulkanContext.h"

#define GET_INSTANCE_PROC_ADDR(inst, entryPoint) \
{\
	fp##entryPoint = reinterpret_cast<PFN_vk##entryPoint>(vkGetInstanceProcAddr(inst, "vk"#entryPoint));\
	HL_ASSERT(fp##entryPoint);\
}

#define GET_DEVICE_PROC_ADDR(dev, entryPoint) \
{\
	fp##entryPoint = reinterpret_cast<PFN_vk##entryPoint>(vkGetDeviceProcAddr(dev, "vk"#entryPoint));\
	HL_ASSERT(fp##entryPoint);\
}

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
	VulkanSwapChain::VulkanSwapChain()
	{
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
	}
	
	void VulkanSwapChain::Init(const Ref<RenderingContext> &context)
	{
		m_Instance = VulkanContext::GetInstance();
		Ref<VulkanContext> vkContext = context.As<VulkanContext>();
		m_Device = vkContext->GetDevice();

		VkDevice vkDevice = m_Device->GetNativeDevice();

		GET_DEVICE_PROC_ADDR(vkDevice, CreateSwapchainKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, DestroySwapchainKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, GetSwapchainImagesKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, AcquireNextImageKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, QueuePresentKHR);

		GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceSupportKHR);
		GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
		GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceFormatsKHR);
		GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfacePresentModesKHR);

		GET_INSTANCE_PROC_ADDR(m_Instance, CmdSetCheckpointNV);
		GET_INSTANCE_PROC_ADDR(m_Instance, GetQueueCheckpointDataNV);
	}
	
	void VulkanSwapChain::InitSurface(void *windowHandle)
	{
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetNativePhysicalDevice();
		uint32 queueCount;
		uint32 i;
		uint32 graphicsQueueNodeIndex = UINT32_MAX;
		uint32 presentQueueNodeIndex = UINT32_MAX;
		std::vector<VkQueueFamilyProperties> queueProps;
		std::vector<VkBool32> supportsPresent;

	#ifdef HIGHLO_API_GLFW
		glfwCreateWindowSurface(m_Instance, (GLFWwindow*)windowHandle, nullptr, &m_Surface);
	#endif // HIGHLO_API_GLFW

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, NULL);
		HL_ASSERT(queueCount > 0);
		queueProps.resize(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());

		supportsPresent.resize(queueCount);
		for (i = 0; i < queueCount; ++i)
			fpGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supportsPresent[i]);

		for (i = 0; i < queueCount; ++i)
		{
			if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
			{
				if (graphicsQueueNodeIndex == UINT32_MAX)
					graphicsQueueNodeIndex = i;

				if (supportsPresent[i] == VK_TRUE)
				{
					graphicsQueueNodeIndex = i;
					presentQueueNodeIndex = i;
					break;
				}
			}
		}

		if (presentQueueNodeIndex == UINT32_MAX)
		{
			// No queue was found, which supports presenting and graphics flag
			// so try to find another present queue
			for (i = 0; i < queueCount; ++i)
			{
				if (supportsPresent[i] == VK_TRUE)
				{
					presentQueueNodeIndex = i;
					break;
				}
			}
		}

		// Both indices should be filled with data by now
		HL_ASSERT(graphicsQueueNodeIndex != UINT32_MAX);
		HL_ASSERT(presentQueueNodeIndex != UINT32_MAX);

		m_QueueNodeIndex = graphicsQueueNodeIndex;
		FindImageFormatAndColorSpace();
	}

	
	void VulkanSwapChain::Create(uint32 *width, uint32 *height, bool vsync)
	{
		m_VSync = vsync;

		VkDevice device = m_Device->GetNativeDevice();
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetNativePhysicalDevice();
		VkSwapchainKHR lastSwapChain = m_SwapChain;
		VkSurfaceCapabilitiesKHR surfaceCaps;
		uint32 presentModeCount;
		uint32 desiredNumberOfSwapchainImages;
		uint32 i;
		std::vector<VkPresentModeKHR> presentModes;
		VkExtent2D swapChainExtent = {};
		VkSwapchainCreateInfoKHR swapchainCI = {};
		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR; // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
		VkSurfaceTransformFlagsKHR preTransform;
		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags =
		{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		VkFenceCreateInfo fenceCreateInfo{};
		VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkFormat depthFormat = m_Device->GetPhysicalDevice()->GetDepthFormat();
		std::array<VkAttachmentDescription, 2> attachments = {};
		VkAttachmentReference colorReference = {};
		VkAttachmentReference depthReference = {};
		VkSubpassDescription subpassDescription = {};
		VkSubpassDependency dependency = {};
		VkRenderPassCreateInfo renderPassInfo = {};

		// Get physical device surface properties and formats
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &surfaceCaps));

		// Get available presentation modes
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, NULL));
		HL_ASSERT(presentModeCount > 0);
		presentModes.resize(presentModeCount);
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, presentModes.data()));

		// If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
		if (surfaceCaps.currentExtent.width == (uint32)-1)
		{
			swapChainExtent.width = *width;
			swapChainExtent.height = *height;
		}
		else
		{
			swapChainExtent = surfaceCaps.currentExtent;
			*width = surfaceCaps.currentExtent.width;
			*height = surfaceCaps.currentExtent.height;
		}

		m_Width = *width;
		m_Height = *height;

		// Select a present mode for the swapchain

		if (!vsync)
		{
			for (i = 0; i < presentModeCount; ++i)
			{
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}

				if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}

		// Determine the number of images
		desiredNumberOfSwapchainImages = surfaceCaps.minImageCount + 1;
		if ((surfaceCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfaceCaps.maxImageCount))
		{
			desiredNumberOfSwapchainImages = surfaceCaps.maxImageCount;
		}

		// Find the transformation of the surface
		if (surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = surfaceCaps.currentTransform;
		}

		// Find a supported composite alpha format (not all devices support alpha opaque)
		for (auto &compositeAlphaFlag : compositeAlphaFlags)
		{
			if (surfaceCaps.supportedCompositeAlpha & compositeAlphaFlag)
			{
				compositeAlpha = compositeAlphaFlag;
				break;
			}
		}

		swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCI.pNext = NULL;
		swapchainCI.surface = m_Surface;
		swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
		swapchainCI.imageFormat = m_ColorFormat;
		swapchainCI.imageColorSpace = m_ColorSpace;
		swapchainCI.imageExtent = { swapChainExtent.width, swapChainExtent.height };
		swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR) preTransform;
		swapchainCI.imageArrayLayers = 1;
		swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCI.queueFamilyIndexCount = 0;
		swapchainCI.pQueueFamilyIndices = NULL;
		swapchainCI.presentMode = swapchainPresentMode;
		swapchainCI.oldSwapchain = lastSwapChain;
		swapchainCI.clipped = VK_TRUE;		// Setting clipped to true allows the implementation to discard rendering outside of the surface area
		swapchainCI.compositeAlpha = compositeAlpha;

		// Enable transfer source on swap chain images if supported
		if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		// Enable transfer destination on swap chain images if supported
		if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VK_CHECK_RESULT(fpCreateSwapchainKHR(device, &swapchainCI, nullptr, &m_SwapChain));

		if (lastSwapChain != VK_NULL_HANDLE)
		{
			for (i = 0; i < m_ImageCount; ++i)
				vkDestroyImageView(device, m_Buffers[i].View, nullptr);
			fpDestroySwapchainKHR(device, lastSwapChain, nullptr);
		}

		// Get the current image count
		VK_CHECK_RESULT(fpGetSwapchainImagesKHR(device, m_SwapChain, &m_ImageCount, NULL));
		HL_ASSERT(m_ImageCount > 0);

		// Get the swap chain images
		m_Images.resize(m_ImageCount);
		m_Buffers.resize(m_ImageCount);
		VK_CHECK_RESULT(fpGetSwapchainImagesKHR(device, m_SwapChain, &m_ImageCount, m_Images.data()));

		// Geth the swap chain buffers containing the image and the view
		for (i = 0; i < m_ImageCount; ++i)
		{
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.pNext = NULL;
			colorAttachmentView.format = m_ColorFormat;
			colorAttachmentView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;
			colorAttachmentView.image = m_Images[i];

			m_Buffers[i].Image = m_Images[i];
			VK_CHECK_RESULT(vkCreateImageView(device, &colorAttachmentView, nullptr, &m_Buffers[i].View));
		}

		// Create command buffers
		{
			VkCommandPoolCreateInfo cmdPoolInfo = {};
			VkCommandBufferAllocateInfo cmdBufAllocInfo{};
			uint32 count = m_ImageCount;

			m_CommandBuffers.resize(count);

			cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmdPoolInfo.queueFamilyIndex = m_QueueNodeIndex;
			cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &m_CommandPool));

			cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmdBufAllocInfo.commandPool = m_CommandPool;
			cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cmdBufAllocInfo.commandBufferCount = count;
			VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocInfo, m_CommandBuffers.data()));
		}

		// Synchronization Objects
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreateSemaphore(m_Device->GetNativeDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphores.PresentComplete));
		VK_CHECK_RESULT(vkCreateSemaphore(m_Device->GetNativeDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphores.RenderComplete));

		m_SubmitInfo = {};
		m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_SubmitInfo.pWaitDstStageMask = &pipelineStageFlags;
		m_SubmitInfo.waitSemaphoreCount = 1;
		m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		m_SubmitInfo.signalSemaphoreCount = 1;
		m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

		// Wait fences to sync command buffer access
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		
		// HINT: Renderer::GetConfig should not be able to be called here, because the Renderer gets initialized after the Window
		//m_WaitFences.resize(Renderer::GetConfig().FramesInFlight);
		m_WaitFences.resize(3);
		for (auto &fence : m_WaitFences)
			VK_CHECK_RESULT(vkCreateFence(m_Device->GetNativeDevice(), &fenceCreateInfo, nullptr, &fence));

		// Render Pass

		// Color Attachment
		attachments[0].format = m_ColorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		 
		// Depth Attachment
		attachments[1].format = depthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
		//subpassDescription.pDepthStencilAttachment = &depthReference;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;

		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;
		VK_CHECK_RESULT(vkCreateRenderPass(m_Device->GetNativeDevice(), &renderPassInfo, nullptr, &m_RenderPass));

		CreateFramebuffer();
	}
	
	void VulkanSwapChain::Cleanup()
	{
		VkDevice device = m_Device->GetNativeDevice();
		uint32 i;

		if (m_SwapChain)
		{
			for (i = 0; i < m_ImageCount; ++i)
				vkDestroyImageView(device, m_Buffers[i].View, nullptr);
		}

		if (m_Surface)
		{
			fpDestroySwapchainKHR(device, m_SwapChain, nullptr);
			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		}

		m_Surface = VK_NULL_HANDLE;
		m_SwapChain = VK_NULL_HANDLE;
	}
	
	void VulkanSwapChain::OnResize(uint32 width, uint32 height)
	{
		HL_CORE_WARN("VulkanSwapChain::OnResize");
		VkDevice device = m_Device->GetNativeDevice();

		vkDeviceWaitIdle(device);
		Create(&width, &height, m_VSync);

		for (auto &framebuffer : m_Framebuffers)
			vkDestroyFramebuffer(device, framebuffer, nullptr);

		CreateFramebuffer();
		vkDeviceWaitIdle(device);
	}
	
	void VulkanSwapChain::BeginFrame()
	{
		auto &queue = Renderer::GetRenderResourceReleaseQueue(m_CurrentBufferIndex);
		queue.Execute();

		VK_CHECK_RESULT(AcquireNextImage(m_Semaphores.PresentComplete, &m_CurrentImageIndex));
	}
	
	void VulkanSwapChain::EndFrame()
	{
	}

	void VulkanSwapChain::Present()
	{
		// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		const auto &config = Renderer::GetConfig();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitStageMask;
		submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentBufferIndex];
		submitInfo.commandBufferCount = 1;

		VK_CHECK_RESULT(vkResetFences(m_Device->GetNativeDevice(), 1, &m_WaitFences[m_CurrentBufferIndex]));
		VK_CHECK_RESULT(vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]));

		// Present the current buffer to the swap chain
		// Pass the semaphore signaled by the command buffer submission from the submit info as the wait semaphore for swap chain presentation
		// This ensures that the image is not presented to the windowing system until all commands have been submitted
		VkResult result = QueuePresent(m_Device->GetGraphicsQueue(), m_CurrentImageIndex, m_Semaphores.RenderComplete);

		if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				// Swap chain is no longer compatible with the surface and needs to be recreated
				OnResize(m_Width, m_Height);
				return;
			}
			else
			{
				VK_CHECK_RESULT(result);
			}
		}

		m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % config.FramesInFlight;

		// Make sure the frame we're requesting has finished rendering
		VK_CHECK_RESULT(vkWaitForFences(m_Device->GetNativeDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX));
	}

	VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32 *imageIndex)
	{
		return fpAcquireNextImageKHR(m_Device->GetNativeDevice(), m_SwapChain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, imageIndex);
	}
	
	VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32 imageIndex, VkSemaphore wait)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain;
		presentInfo.pImageIndices = &imageIndex;

		// Check if a wait semaphore has been specified to wait for before presenting the image
		if (wait != VK_NULL_HANDLE)
		{
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &wait;
		}

		return fpQueuePresentKHR(queue, &presentInfo);
	}
	
	void VulkanSwapChain::CreateFramebuffer()
	{
		VkImageView ivAttachments[2];
		VkFramebufferCreateInfo framebufferCreateInfo = {};
		uint32 i;

		ivAttachments[1] = m_DepthStencil.ImageView;

		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = NULL;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = ivAttachments;
		framebufferCreateInfo.width = m_Width;
		framebufferCreateInfo.height = m_Height;
		framebufferCreateInfo.layers = 1;

		m_Framebuffers.resize(m_ImageCount);
		for (i = 0; i < m_Framebuffers.size(); ++i)
		{
			ivAttachments[0] = m_Buffers[i].View;
			VK_CHECK_RESULT(vkCreateFramebuffer(m_Device->GetNativeDevice(), &framebufferCreateInfo, nullptr, &m_Framebuffers[i]));
		}
	}
	
	void VulkanSwapChain::CreateDepthStencil()
	{
		VkDevice device = m_Device->GetNativeDevice();
		VkFormat depthFormat = m_Device->GetPhysicalDevice()->GetDepthFormat();
		VkImageCreateInfo imgCI{};
		VkImageViewCreateInfo imgViewCI{};
		VulkanAllocator allocator("SwapChain");

		imgCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imgCI.imageType = VK_IMAGE_TYPE_2D;
		imgCI.format = depthFormat;
		imgCI.extent = { m_Width, m_Height, 1 };
		imgCI.mipLevels = 1;
		imgCI.arrayLayers = 1;
		imgCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imgCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imgCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		m_DepthStencil.MemoryAllocation = allocator.AllocateImage(imgCI, VMA_MEMORY_USAGE_GPU_ONLY, m_DepthStencil.Image);

		imgViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imgViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imgViewCI.image = m_DepthStencil.Image;
		imgViewCI.format = depthFormat;
		imgViewCI.subresourceRange.baseMipLevel = 0;
		imgViewCI.subresourceRange.levelCount = 1;
		imgViewCI.subresourceRange.baseArrayLayer = 0;
		imgViewCI.subresourceRange.layerCount = 1;
		imgViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT)
			imgViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VK_CHECK_RESULT(vkCreateImageView(device, &imgViewCI, nullptr, &m_DepthStencil.ImageView));
	}
	
	void VulkanSwapChain::FindImageFormatAndColorSpace()
	{
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetNativePhysicalDevice();
		std::vector<VkSurfaceFormatKHR> surfaceFormats;
		uint32 formatCount;

		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, NULL));
		HL_ASSERT(formatCount > 0);
		surfaceFormats.resize(formatCount);
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, surfaceFormats.data()));

		if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
		{
			m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
			m_ColorSpace = surfaceFormats[0].colorSpace;
		}
		else
		{
			bool foundB8G8R8A8Unorm = false;
			for (auto &&surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
				{
					m_ColorFormat = surfaceFormat.format;
					m_ColorSpace = surfaceFormat.colorSpace;
					foundB8G8R8A8Unorm = true;
					break;
				}
			}

			if (!foundB8G8R8A8Unorm)
			{
				m_ColorFormat = surfaceFormats[0].format;
				m_ColorSpace = surfaceFormats[0].colorSpace;
			}
		}
	}
}

#endif // HIGHLO_API_VULKAN

