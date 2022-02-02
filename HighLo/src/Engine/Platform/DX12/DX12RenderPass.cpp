// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12RenderPass.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12RenderPass::DX12RenderPass(const RenderPassSpecification &specification)
		: m_Specification(specification)
	{
	}

	DX12RenderPass::~DX12RenderPass()
	{
	}
}

#endif // HIGHLO_API_DX12

