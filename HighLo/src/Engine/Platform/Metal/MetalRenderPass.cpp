// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalRenderPass.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalRenderPass::MetalRenderPass(const RenderPassSpecification &spec)
		: m_Specification(spec)
	{
	}

	MetalRenderPass::~MetalRenderPass()
	{
	}
}

#endif // HIGHLO_API_METAL

