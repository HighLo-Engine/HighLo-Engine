// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/UniformBuffer.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~VulkanUniformBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void UploadToShader() override;

	private:


	};
}

