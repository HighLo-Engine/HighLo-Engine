#pragma once

#include "Engine/Graphics/RenderPass.h"

#ifdef HIGHLO_API_VULKAN

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

