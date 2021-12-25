#pragma once

#include "Engine/Renderer/ComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "VulkanShader.h"

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

		// Vulkan specific

		void Dispatch(VkDescriptorSet descriptorSet, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ);
		void Execute(VkDescriptorSet *descriptorSets, uint32 descriptorSetCount, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ);

		VkCommandBuffer GetActiveCommandBuffer() { return m_ActiveComputeCommandBuffer; }

		void SetPushConstants(const void *data, uint32 size);

	private:

		Ref<VulkanShader> m_Shader;
		bool m_UsingGraphicsQueue = false;

		VkPipelineLayout m_ComputePipelineLayout = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
		VkPipeline m_ComputePipeline = nullptr;
		VkCommandBuffer m_ActiveComputeCommandBuffer = nullptr;
	};
}

#endif // HIGHLO_API_VULKAN

