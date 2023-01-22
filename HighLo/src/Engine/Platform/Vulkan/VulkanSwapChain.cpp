// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanSwapChain.h"

#include "Engine/Core/Profiler/ProfilerTimer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#endif // HIGHLO_API_GLFW

inline PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
inline PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
inline PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
inline PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;

inline PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
inline PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
inline PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
inline PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
inline PFN_vkQueuePresentKHR fpQueuePresentKHR;

namespace highlo
{
    void VulkanSwapChain::Init(const Ref<RenderingContext> &context)
    {
        m_Instance = VulkanContext::GetInstance();
        Ref<VulkanContext> vkContext = context.As<VulkanContext>();
        m_Device = vkContext->GetDevice();

        VkDevice device = m_Device->GetNativeDevice();

        GET_DEVICE_PROC_ADDR(device, CreateSwapchainKHR);
        GET_DEVICE_PROC_ADDR(device, DestroySwapchainKHR);
        GET_DEVICE_PROC_ADDR(device, GetSwapchainImagesKHR);
        GET_DEVICE_PROC_ADDR(device, AcquireNextImageKHR);
        GET_DEVICE_PROC_ADDR(device, QueuePresentKHR);

        GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceSupportKHR);
        GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
        GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfaceFormatsKHR);
        GET_INSTANCE_PROC_ADDR(m_Instance, GetPhysicalDeviceSurfacePresentModesKHR);
    }
    
    void VulkanSwapChain::InitSurface(void *windowHandle)
    {
        VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetNativeDevice();

    #ifdef HIGHLO_API_GLFW
        GLFWwindow *window = static_cast<GLFWwindow*>(windowHandle);
        glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface);
    #else
        // TODO: Add Win32 API version
    #endif // HIGHLO_API_GLFW

        uint32 queueCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
        HL_ASSERT(queueCount > 0);

        std::vector<VkQueueFamilyProperties> queueProperties(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProperties.data());

        // Iterate over each queue to learn whether it supports presenting:
        // Find a queue with present support
        // Will be used to present the swap chain images to the windowing system
        std::vector<VkBool32> supportsPresentMode(queueCount);
        for (uint32 i = 0; i < queueCount; ++i)
        {
            fpGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supportsPresentMode[i]);
        }

        // Search for a graphics and a present queue in the array of queue families, try to find one that supports both
        uint32 graphicsQueueNodeIndex = UINT32_MAX;
        uint32 presentQueueNodeIndex = UINT32_MAX;
        for (uint32 i = 0; i < queueCount; ++i)
        {
            if ((queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            {
                graphicsQueueNodeIndex = i;
            }

            if (supportsPresentMode[i] == VK_TRUE)
            {
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex = i;
                break;
            }
        }

        if (presentQueueNodeIndex == UINT32_MAX)
        {
            for (uint32 i = 0; i < queueCount; ++i)
            {
                if (supportsPresentMode[i] == VK_TRUE)
                {
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }

        HL_ASSERT(graphicsQueueNodeIndex != UINT32_MAX);
        HL_ASSERT(presentQueueNodeIndex != UINT32_MAX);

        m_QueueNodeIndex = graphicsQueueNodeIndex;

        FindImageFormatAndColorSpace();
    }
    
    void VulkanSwapChain::Create(uint32 *width, uint32 *height, bool vsync)
    {
        m_VSync = vsync;
        VkDevice device = m_Device->GetNativeDevice();
        VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetNativeDevice();

        VkSwapchainKHR oldSwapchain = m_SwapChain;

        // Get physical device surface properties and formats
        VkSurfaceCapabilitiesKHR surfaceCaps;
        VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &surfaceCaps));

        // Get available present modes
        uint32 presentModeCount;
        VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, nullptr));
        HL_ASSERT(presentModeCount > 0);

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, presentModes.data()));

        VkExtent2D swapchainExtent = {};
        if (surfaceCaps.currentExtent.width == (uint32)-1)
        {
            // If the surface size is undefined, the size is set to the size of the images requested.
            swapchainExtent.width = *width;
            swapchainExtent.height = *height;
        }
        else
        {
            // If the surface size is defined, the swap chain size must match
            swapchainExtent = surfaceCaps.currentExtent;
            *width = surfaceCaps.currentExtent.width;
            *height = surfaceCaps.currentExtent.height;
        }

        m_Width = *width;
        m_Height = *height;

        // If v-sync is not requested, try to find a mailbox mode. It's the lowest latency non-tearing present mode available
        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        if (!vsync)
        {
            for (uint32 i = 0; i < presentModeCount; ++i)
            {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                    break;
                }

                if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
                {
                    swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
                }
            }
        }

        // Determine the number of images
        uint32 desiredNumberOfSwapchainImages = surfaceCaps.minImageCount + 1;
        if ((surfaceCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfaceCaps.maxImageCount))
        {
            desiredNumberOfSwapchainImages = surfaceCaps.maxImageCount;
        }

        // Find the transformation of the surface
        VkSurfaceTransformFlagsKHR preTransform;
        if (surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else
        {
            preTransform = surfaceCaps.currentTransform;
        }

        // Find a supported composite alpha format (not all devices support alpha opaque)
        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        };

        for (auto &compositeAlphaFlag : compositeAlphaFlags)
        {
            if (surfaceCaps.supportedCompositeAlpha & compositeAlphaFlag)
            {
                compositeAlpha = compositeAlphaFlag;
                break;
            }
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.pNext = nullptr;
        swapchainCreateInfo.surface = m_Surface;
        swapchainCreateInfo.minImageCount = desiredNumberOfSwapchainImages;
        swapchainCreateInfo.imageFormat = m_ColorFormat;
        swapchainCreateInfo.imageColorSpace = m_ColorSpace;
        swapchainCreateInfo.imageExtent = { swapchainExtent.width, swapchainExtent.height };
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        swapchainCreateInfo.presentMode = swapchainPresentMode;
        swapchainCreateInfo.oldSwapchain = oldSwapchain;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.compositeAlpha = compositeAlpha;

        // Enable transfer source on swap chain images if supported
        if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        {
            swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }

        // Enable transfer destination on swap chain images if supported
        if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        {
            swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        // finally create the swapchain
        VK_CHECK_RESULT(fpCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &m_SwapChain));

        // If an existing swap chain is re-created, destroy the old swap chain. This also cleans up all the presentable images
        if (oldSwapchain != VK_NULL_HANDLE)
        {
            for (uint32 i = 0; i < m_ImageCount; ++i)
            {
                vkDestroyImageView(device, m_Buffers[i].View, nullptr);
            }
            fpDestroySwapchainKHR(device, oldSwapchain, nullptr);
        }

        // Get the swap chain images
        VK_CHECK_RESULT(fpGetSwapchainImagesKHR(device, m_SwapChain, &m_ImageCount, nullptr));
        m_Images.resize(m_ImageCount);
        VK_CHECK_RESULT(fpGetSwapchainImagesKHR(device, m_SwapChain, &m_ImageCount, m_Images.data()));

        // Get the swap chain buffers containing the image and imageview
        m_Buffers.resize(m_ImageCount);
        for (uint32 i = 0; i < m_ImageCount; ++i)
        {
            VkImageViewCreateInfo colorAttachmentView = {};
            colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            colorAttachmentView.pNext = nullptr;
            colorAttachmentView.format = m_ColorFormat;
            colorAttachmentView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
            colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            colorAttachmentView.subresourceRange.baseMipLevel = 0;
            colorAttachmentView.subresourceRange.levelCount = 1;
            colorAttachmentView.subresourceRange.baseArrayLayer = 0;
            colorAttachmentView.subresourceRange.layerCount = 1;
            colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            colorAttachmentView.flags = 0;

            m_Buffers[i].Image = m_Images[i];
            colorAttachmentView.image = m_Images[i];

            VK_CHECK_RESULT(vkCreateImageView(device, &colorAttachmentView, nullptr, &m_Buffers[i].View));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("Swapchain buffer: {}", i), m_Buffers[i].View);
        }

        // Create command buffers
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex = m_QueueNodeIndex;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VK_CHECK_RESULT(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &m_CommandPool));

        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.commandPool = m_CommandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = m_ImageCount;
        m_Commandbuffers.resize(m_ImageCount);
        VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, m_Commandbuffers.data()));

        // Create synchronization objects
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_Semaphores.PresentComplete));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SEMAPHORE, "Swapchain PresentComplete Semaphore", m_Semaphores.PresentComplete);

        VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_Semaphores.RenderComplete));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SEMAPHORE, "Swapchain RenderComplete Semaphore", m_Semaphores.RenderComplete);

        // Set up submit info structure
        // Semaphores will stay the same during application lifetime
        VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        m_SubmitInfo = {};
        m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        m_SubmitInfo.pWaitDstStageMask = &pipelineStageFlags;
        m_SubmitInfo.waitSemaphoreCount = 1;
        m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
        m_SubmitInfo.signalSemaphoreCount = 1;
        m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

        // Wait fences to sync command buffer access
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        m_WaitFences.resize(3);
        for (uint32 i = 0; i < m_WaitFences.size(); ++i)
        {
            VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &m_WaitFences[i]));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, fmt::format("Swapchain Fence: {}", i).c_str(), m_WaitFences[i]);
        }

        // Create Renderpasses
        std::array<VkAttachmentDescription, 2> attachments = {};
        
        // Color attachment
        attachments[0].format = m_ColorFormat;
        attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // Depth attachment
        attachments[1].format = m_Device->GetPhysicalDevice()->GetDepthFormat();
        attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorReference = {};
        colorReference.attachment = 0;
        colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthReference = {};
        depthReference.attachment = 1;
        depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDesc = {};
        subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDesc.colorAttachmentCount = 1;
        subpassDesc.pColorAttachments = &colorReference;
        subpassDesc.inputAttachmentCount = 0;
        subpassDesc.pInputAttachments = nullptr;
        subpassDesc.preserveAttachmentCount = 0;
        subpassDesc.pPreserveAttachments = nullptr;
        subpassDesc.pResolveAttachments = nullptr;

        VkSubpassDependency subpassDependency = {};
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0;
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = attachments.data();
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDesc;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpassDependency;

        VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &m_RenderPass));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_RENDER_PASS, "Swapchain RenderPass", m_RenderPass);

        CreateFramebuffer();
    }
    
    void VulkanSwapChain::Cleanup()
    {
        VkDevice device = m_Device->GetNativeDevice();

        if (m_SwapChain)
        {
            for (uint32 i = 0; i < m_ImageCount; ++i)
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
        Present();
    }
    
    void VulkanSwapChain::Present()
    {
        HL_PROFILE_FUNCTION();

        const uint64 DEFAULT_FENCE_TIMEOUT = 100000000000;
        VkDevice device = m_Device->GetNativeDevice();

        VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = &waitStageMask;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_Commandbuffers[m_CurrentBufferIndex];

        VK_CHECK_RESULT(vkResetFences(device, 1, &m_WaitFences[m_CurrentBufferIndex]));
        VK_CHECK_RESULT(vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]));

        VkResult result = QueuePresent(m_Device->GetGraphicsQueue(), m_CurrentImageIndex, m_Semaphores.RenderComplete);
        if (result != VK_SUCCESS)
        {
            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                OnResize(m_Width, m_Height);
                return;
            }
            else
            {
                VK_CHECK_RESULT(result);
            }
        }

        const auto &config = Renderer::GetConfig();
        m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % config.FramesInFlight;

        // Make sure the frame we're requesting has finished rendering
        VK_CHECK_RESULT(vkWaitForFences(device, 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX));
    }

    VkFramebuffer VulkanSwapChain::GetFramebuffer(uint32 index)
    {
        HL_ASSERT(index < m_Framebuffers.size());
        return m_Framebuffers[index];
    }

    VkCommandBuffer VulkanSwapChain::GetDrawCommandbuffer(uint32 index)
    {
        HL_ASSERT(index < m_Commandbuffers.size());
        return m_Commandbuffers[index];
    }

    VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presenComplete, uint32 *imageIndex)
    {
        return fpAcquireNextImageKHR(m_Device->GetNativeDevice(), m_SwapChain, UINT64_MAX, presenComplete, (VkFence)nullptr, imageIndex);
    }

    VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32 imageIndex, VkSemaphore waitSemaphore)
    {
        HL_PROFILE_FUNCTION();

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_SwapChain;
        presentInfo.pImageIndices = &imageIndex;

        if (waitSemaphore != VK_NULL_HANDLE)
        {
            presentInfo.pWaitSemaphores = &waitSemaphore;
            presentInfo.waitSemaphoreCount = 1;
        }

        return fpQueuePresentKHR(queue, &presentInfo);
    }

    void VulkanSwapChain::CreateFramebuffer()
    {
        VkDevice device = m_Device->GetNativeDevice();
        VkImageView ivAttachments[2];

        // Depth/Stencil attachment is the same for all frame buffers
        ivAttachments[1] = m_DepthStencil.View;

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = nullptr;
        framebufferCreateInfo.renderPass = m_RenderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = ivAttachments;
        framebufferCreateInfo.layers = 1;
        framebufferCreateInfo.width = m_Width;
        framebufferCreateInfo.height = m_Height;

        m_Framebuffers.resize(m_ImageCount);
        for (uint32 i = 0; i < m_ImageCount; ++i)
        {
            ivAttachments[0] = m_Buffers[i].View;
            VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &m_Framebuffers[i]));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FRAMEBUFFER, fmt::format("Swapchain framebuffer (Frame in flight: {})", i).c_str(), m_Framebuffers[i]);
        }
    }

    void VulkanSwapChain::CreateDepthStencil()
    {
        VkDevice device = m_Device->GetNativeDevice();
        VkFormat depthFormat = m_Device->GetPhysicalDevice()->GetDepthFormat();

        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = depthFormat;
        imageCreateInfo.extent = { m_Width, m_Height, 1 };
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

        m_DepthStencil.Allocation = utils::AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_DepthStencil.Image);
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, "Swapchain depth/stencil iamge", m_DepthStencil.Image);

        VkImageViewCreateInfo imageViewCreatInfo = {};
        imageViewCreatInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreatInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreatInfo.image = m_DepthStencil.Image;
        imageViewCreatInfo.format = depthFormat;
        imageViewCreatInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreatInfo.subresourceRange.levelCount = 1;
        imageViewCreatInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreatInfo.subresourceRange.layerCount = 1;
        imageViewCreatInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        
        // Add stencil flag for formats between VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
        if (depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT)
            imageViewCreatInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreatInfo, nullptr, &m_DepthStencil.View));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, "Swapchain depth/stencil image view", m_DepthStencil.View);
    }

    void VulkanSwapChain::FindImageFormatAndColorSpace()
    {
        VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetNativeDevice();

        uint32 formatCount;
        VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr));
        HL_ASSERT(formatCount > 0);

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, surfaceFormats.data()));

        if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
        {
            m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
            m_ColorSpace = surfaceFormats[0].colorSpace;
        }
        else
        {
            bool foundDesiredFormat = false;
            for (auto &&surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                {
                    m_ColorFormat = surfaceFormat.format;
                    m_ColorSpace = surfaceFormat.colorSpace;
                    foundDesiredFormat = true;
                    break;
                }
            }

            if (!foundDesiredFormat)
            {
                m_ColorFormat = surfaceFormats[0].format;
                m_ColorSpace = surfaceFormats[0].colorSpace;
            }
        }
    }
}

#endif // HIGHLO_API_VULKAN

