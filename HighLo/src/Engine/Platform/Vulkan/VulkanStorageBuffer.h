// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/StorageBuffer.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanStorageBuffer : public StorageBuffer
	{
	public:

		VulkanStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~VulkanStorageBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void UploadToShader() override;
		virtual void Resize(uint32 size) override;

	private:


	};
}

