// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-08) initial release
//

#pragma once

#include "CommandBuffer.h"
#include "Engine/Graphics/Shaders/Shader.h"

namespace highlo
{
	class ComputePipeline : public IsSharedReference
	{
	public:

		HLAPI virtual ~ComputePipeline() {}

		HLAPI virtual void Begin(const Ref<CommandBuffer> &renderCommandBuffer = nullptr) = 0;
		HLAPI virtual void End() = 0;

		HLAPI virtual void Invalidate() = 0;

		HLAPI virtual Ref<Shader> GetShader() = 0;

		HLAPI static Ref<ComputePipeline> Create(const Ref<Shader> &computeShader);
	};
}

