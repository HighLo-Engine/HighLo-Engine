// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalComputePipeline.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalComputePipeline::MetalComputePipeline(const Ref<Shader> &computeShader)
		: m_Shader(computeShader)
	{
		Invalidate();
	}

	MetalComputePipeline::~MetalComputePipeline()
	{
	}

	void MetalComputePipeline::Begin(const Ref<CommandBuffer> &renderCommandBuffer)
	{
	}

	void MetalComputePipeline::End()
	{
	}

	void MetalComputePipeline::Invalidate()
	{
	}
}

#endif // HIGHLO_API_METAL

