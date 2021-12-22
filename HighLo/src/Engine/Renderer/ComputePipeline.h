#pragma once

#include "CommandBuffer.h"
#include "Engine/Renderer/Shaders/Shader.h"

namespace highlo
{
	class ComputePipeline : public IsSharedReference
	{
	public:

		HLAPI virtual ~ComputePipeline() {}

		HLAPI virtual void Begin(Ref<CommandBuffer> renderCommandBuffer = nullptr) = 0;
		HLAPI virtual void End() = 0;

		HLAPI virtual Ref<Shader> GetShader() = 0;

		HLAPI static Ref<ComputePipeline> Create(Ref<Shader> computeShader);
	};
}

