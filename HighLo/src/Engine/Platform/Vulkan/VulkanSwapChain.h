// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/SwapChain.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "VulkanDevice.h"

namespace highlo
{
	struct VulkanSwapChainBuffer
	{
		VkImage Image;
		VkImageView View;
	};

	struct VulkanDepthStencil
	{
		VkImage Image;
		VmaAllocation Allocation;
		VkImageView View;
	};

	struct VulkanSemaphore
	{
		VkSemaphore PresentComplete;
		VkSemaphore RenderComplete;
	};

	class VulkanSwapChain : public SwapChain
	{
	public:

		VulkanSwapChain() {}
		virtual ~VulkanSwapChain() {}

		virtual void Init(const Ref<RenderingContext> &context) override;
		virtual void InitSurface(void *windowHandle) override;
		virtual void Create(uint32 *width, uint32 *height, bool vsync) override;
		virtual void Cleanup() override;

		virtual void OnResize(uint32 width, uint32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Present() override;

		virtual uint32 GetImageCount() const override { return m_ImageCount; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32 GetCurrentBufferIndex() const override { return m_CurrentBufferIndex; }

		// Vulkan-specific
		VkFramebuffer GetFramebuffer(uint32 index);
		VkCommandBuffer GetDrawCommandbuffer(uint32 index);

		VkRenderPass GetRenderPass() { return m_RenderPass; }
		VkFramebuffer GetCurrentFramebuffer() { return GetFramebuffer(m_CurrentImageIndex); }
		VkCommandBuffer GetCurrentCommandbuffer() { return GetDrawCommandbuffer(m_CurrentBufferIndex); }
		VkFormat GetColorFormat() { return m_ColorFormat; }
		VkSemaphore GetRenderCompleteSemaphore() { return m_Semaphores.RenderComplete; }

	private:

		VkResult AcquireNextImage(VkSemaphore presenComplete, uint32 *imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32 imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void CreateFramebuffer();
		void CreateDepthStencil();
		void FindImageFormatAndColorSpace();

	private:

		Ref<VulkanDevice> m_Device = nullptr;

		uint32 m_Width = 0;
		uint32 m_Height = 0;
		uint32 m_CurrentBufferIndex = 0;
		uint32 m_CurrentImageIndex = 0;
		uint32 m_QueueNodeIndex = UINT32_MAX;
		uint32 m_ImageCount = 0;
		bool m_VSync = false;

		std::vector<VulkanSwapChainBuffer> m_Buffers;
		VulkanDepthStencil m_DepthStencil;
		VulkanSemaphore m_Semaphores;

		std::vector<VkFramebuffer> m_Framebuffers;
		std::vector<VkCommandBuffer> m_Commandbuffers;
		VkCommandPool m_CommandPool = nullptr;

		VkSwapchainKHR m_SwapChain = nullptr;
		std::vector<VkImage> m_Images;
		std::vector<VkFence> m_WaitFences;
		VkRenderPass m_RenderPass;
		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

		VkSurfaceKHR m_Surface = nullptr;
		VkSubmitInfo m_SubmitInfo;
		VkInstance m_Instance;

		friend class VulkanContext;
	};
}

#endif // HIGHLO_API_VULKAN

