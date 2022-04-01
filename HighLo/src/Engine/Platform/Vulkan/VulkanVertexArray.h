#pragma once

#include "Engine/Graphics/VertexArray.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanVertexArray : public VertexArray
	{
	public:

		VulkanVertexArray(const VertexArraySpecification &spec);
		virtual ~VulkanVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Invalidate() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> &GetVertexBuffers() override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> &GetIndexBuffer() override { return m_IndexBuffer; }

		virtual VertexArraySpecification &GetSpecification() override { return m_Specification; }
		virtual const VertexArraySpecification &GetSpecification() const override { return m_Specification; }

	private:

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		VertexArraySpecification m_Specification;
	};
}

#endif // HIGHLO_API_VULKAN

