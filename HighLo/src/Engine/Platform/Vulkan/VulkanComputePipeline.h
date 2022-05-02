// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/ComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

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
		void Execute(VkDescriptorSet *descriptorSet, uint32 descriptorSetCount, uint32 x, uint32 y, uint32 z);
		void Dispatch(VkDescriptorSet descriptorSet, uint32 x, uint32 y, uint32 z) const;

		void SetPushConstants(const void *data, uint32 size, uint32 offset = 0) const;
		void CreatePipeline();

		VkCommandBuffer GetActiveCommandBuffer() { return m_ActiveComputeCommandbuffer; }

	private:

		Ref<Shader> m_Shader = nullptr;
		bool m_UsingGraphicsQueue = false;

		VkPipelineLayout m_ComputePipelineLayout = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
		VkPipeline m_ComputePipeline = nullptr;

		VkCommandBuffer m_ActiveComputeCommandbuffer = nullptr;
	};
}

#endif // HIGHLO_API_VULKAN

