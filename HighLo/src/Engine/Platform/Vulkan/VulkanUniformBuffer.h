#pragma once

#include "Engine/Graphics/Shaders/UniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint32 size, uint32 binding);
		virtual ~VulkanUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual uint32 GetBinding() const override { return m_Binding; }

	private:

		uint32 m_Binding = 0;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_VULKAN

