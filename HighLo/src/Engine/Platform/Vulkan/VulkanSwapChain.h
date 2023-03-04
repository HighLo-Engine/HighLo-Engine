// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/SwapChain.h"
#include "Vulkan.h"
#include "VulkanDevice.h"

#include <vk_mem_alloc.h>

namespace highlo
{
	class VulkanSwapChain : public SwapChain
	{
	public:

		VulkanSwapChain() = default;

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
		VkRenderPass GetRenderPass() { return m_RenderPass; }

		VkFramebuffer GetCurrentFramebuffer() { return GetFramebuffer(m_CurrentImageIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetDrawCommandBuffer(m_CurrentBufferIndex); }

		VkFormat GetColorFormat() { return m_ColorFormat; }

		VkFramebuffer GetFramebuffer(uint32 index)
		{
			HL_ASSERT(index < m_Framebuffers.size());
			return m_Framebuffers[index];
		}

		VkCommandBuffer GetDrawCommandBuffer(uint32 index)
		{
			HL_ASSERT(index < m_CommandBuffers.size());
			return m_CommandBuffers[index].CommandBuffer;
		}

		VkSemaphore GetRenderCompleteSemaphore() { return m_Semaphores.RenderComplete; }
		void SetVSync(const bool enabled) { m_VSync = enabled; }

	private:
		
		uint32 AcquireNextImage();
		void FindImageFormatAndColorSpace();

	private:

		VkInstance m_Instance = VK_NULL_HANDLE;
		Ref<RenderingContext> m_CurrentContext = nullptr;
		void *m_NativeWindowHandle = nullptr;
		Ref<VulkanDevice> m_Device = nullptr;
		bool m_VSync = false;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
		uint32 m_ImageCount = 0;
		std::vector<VkImage> m_VulkanImages;

		struct SwapchainImage
		{
			VkImage Image = nullptr;
			VkImageView ImageView = nullptr;
		};
		std::vector<SwapchainImage> m_Images;

		struct
		{
			VkImage Image = nullptr;
			VmaAllocation MemoryAlloc = nullptr;
			VkImageView ImageView = nullptr;
		} m_DepthStencil;

		std::vector<VkFramebuffer> m_Framebuffers;

		struct SwapchainCommandBuffer
		{
			VkCommandPool CommandPool = nullptr;
			VkCommandBuffer CommandBuffer = nullptr;
		};
		std::vector<SwapchainCommandBuffer> m_CommandBuffers;

		struct
		{
			// Swap chain
			VkSemaphore PresentComplete = nullptr;
			// Command buffer
			VkSemaphore RenderComplete = nullptr;
		} m_Semaphores;
		VkSubmitInfo m_SubmitInfo;

		std::vector<VkFence> m_WaitFences;

		VkRenderPass m_RenderPass = nullptr;
		uint32 m_CurrentBufferIndex = 0;
		uint32 m_CurrentImageIndex = 0;

		uint32 m_QueueNodeIndex = UINT32_MAX;
		uint32 m_Width = 0, m_Height = 0;

		VkSurfaceKHR m_Surface;

		friend class VulkanContext;
	};
}

#endif // HIGHLO_API_VULKAN

