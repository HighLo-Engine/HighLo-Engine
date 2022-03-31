#pragma once

#include "Engine/Graphics/IndexBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:

		VulkanIndexBuffer(std::vector<int32> &indices);
		VulkanIndexBuffer(void *data, uint32 size);
		VulkanIndexBuffer(uint32 size);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32 GetCount() override;
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }

	private:

		HLRendererID m_RendererID = 0;
	};
}

#endif // HIGHLO_API_VULKAN

