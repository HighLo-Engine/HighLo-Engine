#pragma once

#include "Engine/ImGui/ImGuiRenderer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

namespace highlo
{
	class VulkanImGuiRenderer : public ImGuiRenderer
	{
	public:

		VulkanImGuiRenderer();
		virtual ~VulkanImGuiRenderer();

		virtual void Init(Window *window) override;
		virtual void Shutdown() override;

		virtual void NewFrame() override;

		virtual void Render() override;
		virtual void RenderDrawData() override;
	};
}

#endif // HIGHLO_API_VULKAN

