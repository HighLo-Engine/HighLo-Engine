// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/ComputePipeline.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanComputePipeline : public ComputePipeline
	{
	public:

		VulkanComputePipeline(const Ref<Shader> &computeShader);
		virtual ~VulkanComputePipeline();

		virtual void Begin(const Ref<CommandBuffer> &renderCommandBuffer = nullptr) override;
		virtual void End() override;

		virtual void Invalidate() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

	private:

		Ref<Shader> m_Shader = nullptr;
	};
}

