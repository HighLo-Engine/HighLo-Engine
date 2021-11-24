// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/UniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(const HLString &name, const std::vector<UniformVariable> &layout, UniformBufferParentShader parent, uint32 slot = 0);
		virtual ~VulkanUniformBuffer();

		virtual void UploadToShader(uint32 offset = 0) override;
	};
}

#endif // HIGHLO_API_VULKAN

