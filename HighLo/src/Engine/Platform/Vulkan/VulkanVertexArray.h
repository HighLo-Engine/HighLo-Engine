// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "Engine/Renderer/VertexArray.h"

namespace highlo
{
	/// <summary>
	/// Even though Vulkan does not have the concept of a vertexArray (only OpenGL does have it), we use this class as a wrapper around the VulkanPipeline,
	/// to support the same API throughout the entire system
	/// </summary>
	class VulkanVertexArray : public VertexArray
	{
	public:

		VulkanVertexArray();
		virtual ~VulkanVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Invalidate() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> &GetVertexBuffers() override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> &GetIndexBuffer() override { return m_IndexBuffer; }

	private:

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}

#endif // HIGHLO_API_VULKAN

