// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/RenderPass.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanRenderPass : public RenderPass
	{
	public:

		VulkanRenderPass(const RenderPassSpecification &spec);
		virtual ~VulkanRenderPass();

		virtual RenderPassSpecification &GetSpcification() override { return m_Specification; }
		virtual const RenderPassSpecification &GetSpcification() const override { return m_Specification; }

	private:

		RenderPassSpecification m_Specification;
	};
}

#endif // HIGHLO_API_VULKAN

