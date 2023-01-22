// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12ComputePipeline.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12ComputePipeline::DX12ComputePipeline(const Ref<Shader> &computeShader)
		: m_Shader(computeShader)
	{
	}

	DX12ComputePipeline::~DX12ComputePipeline()
	{
	}
	
	void DX12ComputePipeline::Begin(const Ref<CommandBuffer> &renderCommandBuffer)
	{
	}
	
	void DX12ComputePipeline::End()
	{
	}
	
	void DX12ComputePipeline::Invalidate()
	{
	}
}

#endif // HIGHLO_API_DX12

