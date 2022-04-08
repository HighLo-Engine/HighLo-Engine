#pragma once

#include "Engine/Graphics/VertexArray.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "VulkanShaderDefs.h"

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
		VkPipeline GetVulkanPipeline() { return m_Pipeline; }
		VkPipelineLayout GetVulkanPipelineLayout() { return m_PipelineLayout; }
		VkDescriptorSet GetDescriptorSet(uint32 set = 0)
		{
			HL_ASSERT(m_Descriptors.Descriptors.size() > set);
			return m_Descriptors.Descriptors[set];
		}

		const std::vector<VkDescriptorSet> &GetDescriptorSets() const { return m_Descriptors.Descriptors; }

	private:

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		VertexArraySpecification m_Specification;

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;
		VulkanShaderMaterialDescriptorSet m_Descriptors;
	};
}

#endif // HIGHLO_API_VULKAN

