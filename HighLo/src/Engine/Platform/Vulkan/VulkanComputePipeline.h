#pragma once

#include "Engine/Graphics/ComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanComputePipeline : public ComputePipeline
	{
	public:

		VulkanComputePipeline(Ref<Shader> computeShader);
		virtual ~VulkanComputePipeline();

		virtual void Begin(Ref<CommandBuffer> renderCommandBuffer = nullptr) override;
		virtual void End() override;

		virtual void Invalidate() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

	private:

		Ref<Shader> m_Shader;
	};
}

#endif // HIGHLO_API_VULKAN

