// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/RenderPass.h"

#include "Vulkan.h"

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

