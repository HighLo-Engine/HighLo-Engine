// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/ImGui/ImGuiRenderer.h"

#include "Vulkan.h"

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

