#pragma once

#include "Engine/Graphics/VertexBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:

		VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		VulkanVertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual VertexBufferUsage GetUsage() override { return m_Usage; }

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
		VertexBufferUsage m_Usage;
	};
}

#endif // HIGHLO_API_VULKAN

