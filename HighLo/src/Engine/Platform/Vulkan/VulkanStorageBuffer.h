#pragma once

#include "Engine/Graphics/Shaders/StorageBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanStorageBuffer : public StorageBuffer
	{
	public:

		VulkanStorageBuffer(uint32 size, uint32 binding);
		virtual ~VulkanStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual void Resize(uint32 size) override;

		virtual uint32 GetBinding() const override;

	private:


	};
}

#endif // HIGHLO_API_VULKAN

