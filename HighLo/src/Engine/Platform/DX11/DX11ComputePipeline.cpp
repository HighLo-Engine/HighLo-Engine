// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11ComputePipeline.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11ComputePipeline::DX11ComputePipeline(Ref<Shader> computeShader)
		: m_Shader(computeShader)
	{
		Invalidate();
	}

	DX11ComputePipeline::~DX11ComputePipeline()
	{
	}
	
	void DX11ComputePipeline::Begin(Ref<CommandBuffer> renderCommandBuffer)
	{
	}
	
	void DX11ComputePipeline::End()
	{
	}
	
	void DX11ComputePipeline::Invalidate()
	{
	}
}

#endif // HIGHLO_API_DX11

