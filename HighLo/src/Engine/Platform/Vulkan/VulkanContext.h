// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/RenderingContext.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanContext : public RenderingContext
	{
	public:

		VulkanContext(void *handle);
		virtual ~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override;

	private:

		inline static VkInstance s_VulkanInstance;
		VkDebugReportCallbackEXT m_DebugReportCallback = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = nullptr;

		// Devices
		// Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		// Ref<VulkanDevice> m_Device;
		// VulkanSwapChain m_SwapChain;
	};
}

#endif // HIGHLO_API_VULKAN

