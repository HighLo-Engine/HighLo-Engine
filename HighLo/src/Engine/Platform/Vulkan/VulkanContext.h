// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/RenderingContext.h"
#include "Vulkan.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

namespace highlo
{
	class VulkanContext : public RenderingContext
	{
	public:

		VulkanContext(void *handle, WindowData &data);
		virtual ~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override {}
		virtual void MakeCurrent() override {}
		virtual void SetSwapInterval(bool bEnabled) override {}

		virtual void *GetCurrentContext() override { return nullptr; }

		// Vulkan-specific
		Ref<VulkanDevice> GetDevice() { return m_Device; }

		static VkInstance GetInstance() { return s_VulkanInstance; }

		/// <summary>
		/// Creates a new VulkanContext based on the existing global RenderingContext.
		/// You can also call `Renderer::GetContext()` to retrieve the global rendering context directly and then call `.As<VulkanContext>()` on the retrieved value.
		/// </summary>
		/// <returns>Returns the current global rendering context, implicitly casted to a Vulkan context.</returns>
		static Ref<VulkanContext> Get() { return Ref<VulkanContext>(Renderer::GetContext()); }
		
		/// <summary>
		/// Retrieves the current vulkan device, attached to the global rendering context.
		/// </summary>
		/// <returns>Returns the current vulkan device</returns>
		static Ref<VulkanDevice> GetCurrentDevice() { return Get()->GetDevice(); }

	private:

		// Devices
		Ref<PhysicalDevice> m_PhysicalDevice = nullptr;
		Ref<VulkanDevice> m_Device = nullptr;

		// Vulkan instance
		inline static VkInstance s_VulkanInstance;

		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = nullptr;

		VulkanSwapChain m_SwapChain;
	};
}

#endif // HIGHLO_API_VULKAN

