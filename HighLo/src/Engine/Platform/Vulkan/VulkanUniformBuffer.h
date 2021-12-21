// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/Shaders/UniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint32 size, uint32 binding);
		virtual ~VulkanUniformBuffer();

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual uint32 GetBinding() const override { return m_Binding; }

	private:

		uint32 m_Size = 0, m_Binding = 0;
	};
}

#endif // HIGHLO_API_VULKAN

