// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/ComputePipeline.h"
#include "Engine/Graphics/CommandBuffer.h"
#include "Vulkan.h"

namespace highlo
{
	class VulkanComputePipeline : public ComputePipeline
	{
	public:

		VulkanComputePipeline(const Ref<Shader> &computeShader);
		virtual ~VulkanComputePipeline();

		virtual void Begin(const Ref<CommandBuffer> &renderCommandBuffer = nullptr) override;
		virtual void End() override;

		virtual void Invalidate() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

		// Vulkan-specific
		VkCommandBuffer GetActiveCommandBuffer() { return m_ActiveComputeCommandBuffer; }
		void Dispatch(VkDescriptorSet descriptorSet, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) const;
		void Execute(VkDescriptorSet *descriptorSets, uint32 descriptorSetCount, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ);

		void SetPushConstants(const void *data, uint32 size) const;
		
		void CreatePipeline();
		void RT_CreatePipeline();

	private:

		Ref<Shader> m_Shader = nullptr;

		VkPipelineLayout m_ComputePipelineLayout = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
		VkPipeline m_ComputePipeline = nullptr;

		VkCommandBuffer m_ActiveComputeCommandBuffer = nullptr;

		bool m_UsingGraphicsQueue = false;
	};
}

#endif // HIGHLO_API_VULKAN

