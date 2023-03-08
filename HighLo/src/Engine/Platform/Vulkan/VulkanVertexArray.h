// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/VertexArray.h"
#include "Vulkan.h"
#include "VulkanShader.h"

namespace highlo
{
	class VulkanVertexArray : public VertexArray
	{
	public:

		VulkanVertexArray(const VertexArraySpecification &spec);
		virtual ~VulkanVertexArray();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void Invalidate() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> &GetVertexBuffers() override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> &GetIndexBuffer() override { return m_IndexBuffer; }

		virtual VertexArraySpecification &GetSpecification() override { return m_Specification; }
		virtual const VertexArraySpecification &GetSpecification() const override { return m_Specification; }

		// Vulkan-specific
		void SetUniformBuffer(const Ref<UniformBuffer> &uniformBuffer, uint32 binding, uint32 set = 0);
		void RT_SetUniformBuffer(const Ref<UniformBuffer> &uniformBuffer, uint32 binding, uint32 set = 0);

		VkPipeline GetVulkanPipeline() { return m_VulkanPipeline; }
		VkPipelineLayout GetVulkanPipelineLayout() { return m_PipelineLayout; }
		VkDescriptorSet GetDescriptorSet(uint32 set = 0)
		{
			HL_ASSERT(m_DescriptorSets.DescriptorSets.size() > set);
			return m_DescriptorSets.DescriptorSets[set];
		}

		const std::vector<VkDescriptorSet> &GetDescriptorSets() const { return m_DescriptorSets.DescriptorSets; }

	private:

		VertexArraySpecification m_Specification;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;

		VkPipelineLayout m_PipelineLayout = nullptr;
		VkPipeline m_VulkanPipeline = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
	
		VkShaderMaterialDescriptorSet m_DescriptorSets;
	};
}

#endif // HIGHLO_API_VULKAN

