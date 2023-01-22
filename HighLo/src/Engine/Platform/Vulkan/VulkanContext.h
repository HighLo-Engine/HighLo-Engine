// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/RenderingContext.h"
#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"

namespace highlo
{
	class VulkanContext : public RenderingContext
	{
	public:

		VulkanContext();
		VulkanContext(void *handle, WindowData &data);
		virtual ~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override;

		// Vulkan-specific
		static VkInstance GetInstance() { return s_VulkanInstance; }

		Ref<VulkanDevice> GetDevice() { return m_Device; }

		static Ref<VulkanContext> Get() { return Ref<VulkanContext>(Renderer::GetContext()); }
		static Ref<VulkanDevice> GetCurrentDevice() { return Get()->GetDevice(); }

	private:

		void *m_VulkanWindowHandle = nullptr;
		WindowData m_WindowData;

		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;
		Ref<VulkanDevice> m_Device = nullptr;
		VulkanSwapChain m_SwapChain;

		inline static VkInstance s_VulkanInstance;

		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = nullptr;
	};
}

#endif // HIGHLO_API_VULKAN

