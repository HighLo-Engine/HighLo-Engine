// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/ImGui/ImGuiRenderer.h"

#ifdef HIGHLO_API_VULKAN

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

