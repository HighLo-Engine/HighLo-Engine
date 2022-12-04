// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/RenderingContext.h"
#include "Engine/Renderer/Renderer.h"

#include "Vulkan.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"

namespace highlo
{
	class VulkanContext : public RenderingContext
	{
	public:

		VulkanContext(void *handle, WindowData &data);
		virtual ~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override;

		// Vulkan-specific
		Ref<VulkanDevice> GetVulkanDevice() { return m_Device; }
		Ref<VulkanPhysicalDevice> GetVulkanPhysicalDevice() { return m_PhysicalDevice; }

		static VkInstance GetVulkanCurrentInstance() { return s_VulkanInstance; }
		static Ref<VulkanContext> GetVulkanCurrentContext() { return Ref<VulkanContext>(Renderer::GetContext()); }
		static Ref<VulkanDevice> GetVulkanCurrentDevice() { return GetVulkanCurrentContext()->GetVulkanDevice(); }

	private:

		void *m_NativeWindowHandle = nullptr;

		VulkanSwapChain m_SwapChain;

		Ref<VulkanDevice> m_Device = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;

		inline static VkInstance s_VulkanInstance;
		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = nullptr;
	};
}

#endif // HIGHLO_API_VULKAN

