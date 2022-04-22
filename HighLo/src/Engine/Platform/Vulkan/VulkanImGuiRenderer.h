// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

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

