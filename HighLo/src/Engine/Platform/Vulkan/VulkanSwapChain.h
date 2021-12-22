// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/SwapChain.h"

#include "VulkanDevice.h"
#include "VulkanAllocator.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	struct SwapChainBuffer
	{
		VkImage Image;
		VkImageView View;
	};

	struct DepthStencilBuffers
	{
		VkImage Image;
		VmaAllocation MemoryAllocation;
		VkImageView ImageView;
	};

	struct SemaphoreBuffers
	{
		VkSemaphore PresentComplete;	// Swap Chain
		VkSemaphore RenderComplete;		// Command Buffer
	};

	class VulkanSwapChain : public SwapChain
	{
	public:

		VulkanSwapChain();
		virtual ~VulkanSwapChain();

		// Inherited via SwapChain
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
		
		// Vulkan specific
		VkRenderPass GetNativeRenderPass() { return m_RenderPass; }
		VkFormat GetNativeColorFormat() { return m_ColorFormat; }
		VkSemaphore GetRenderCompleteSemaphore() { return m_Semaphores.RenderComplete; }

		VkFramebuffer GetCurrentFramebuffer() { return GetNativeFramebuffer(m_CurrentImageIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetNativeCommandBuffer(m_CurrentBufferIndex); }

		VkFramebuffer GetNativeFramebuffer(uint32 index)
		{
			HL_ASSERT(index < m_Framebuffers.size());
			return m_Framebuffers[index];
		}

		VkCommandBuffer GetNativeCommandBuffer(uint32 index)
		{
			HL_ASSERT(index < m_CommandBuffers.size());
			return m_CommandBuffers[index];
		}

	private:

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32 *imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32 imageIndex, VkSemaphore wait = VK_NULL_HANDLE);

		void CreateFramebuffer();
		void CreateDepthStencil();
		void FindImageFormatAndColorSpace();

		bool m_VSync = false;
		uint32 m_Width = 0, m_Height = 0;
		uint32 m_CurrentBufferIndex = 0;
		uint32 m_CurrentImageIndex = 0;
		uint32 m_QueueNodeIndex = UINT32_MAX;
		uint32 m_ImageCount = 0;
		std::vector<VkImage> m_Images;

		VkInstance m_Instance;
		Ref<VulkanDevice> m_Device;
		VkSwapchainKHR m_SwapChain = nullptr;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;
	
		std::vector<SwapChainBuffer> m_Buffers;
		DepthStencilBuffers m_DepthStencil;
		SemaphoreBuffers m_Semaphores;
		
		VkSubmitInfo m_SubmitInfo;
		VkRenderPass m_RenderPass;

		std::vector<VkFramebuffer> m_Framebuffers;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		VkCommandPool m_CommandPool = nullptr;
		VkSurfaceKHR m_Surface;
		
		std::vector<VkFence> m_WaitFences;

		friend class VulkanContext;
	};
}

#endif // HIGHLO_API_VULKAN

