// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderPassBuilder.h"

namespace highlo
{
	RenderPassBuilder::RenderPassBuilder()
	{
	}

	RenderPassBuilder::~RenderPassBuilder()
	{
	}

	RenderPassBuilder &RenderPassBuilder::DebugName(const HLString &debugName)
	{
		m_TargetSpec.DebugName = debugName;
		return *this;
	}

	RenderPassBuilder &RenderPassBuilder::DebugMarkerColor(const glm::vec4 &color)
	{
		m_TargetSpec.DebugMarkerColor = color;
		return *this;
	}

	RenderPassBuilder &RenderPassBuilder::SetFramebuffer(const Ref<Framebuffer> &framebuffer)
	{
		m_TargetSpec.Framebuffer = framebuffer;
		return *this;
	}

	Ref<RenderPass> RenderPassBuilder::Build(const RenderPassSpecification *overrideSpec)
	{
		if (overrideSpec)
			return RenderPass::Create(*overrideSpec);

		return RenderPass::Create(m_TargetSpec);
	}
}

