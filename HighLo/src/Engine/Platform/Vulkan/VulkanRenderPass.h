// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/RenderPass.h"

namespace highlo
{
	class VulkanRenderPass : public RenderPass
	{
	public:

		VulkanRenderPass(const RenderPassSpecification &spec);
		virtual ~VulkanRenderPass();

		virtual RenderPassSpecification &GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification &GetSpecification() const override { return m_Specification; }

	private:

		RenderPassSpecification m_Specification;
	};
}

#endif // HIGHLO_API_VULKAN

