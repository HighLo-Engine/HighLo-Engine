// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FramebufferBuilder.h"

namespace highlo
{
	FramebufferBuilder::FramebufferBuilder()
	{
		m_TargetSpec.ClearOnLoad = false;
		m_TargetSpec.ClearColorOnLoad = false;
		m_TargetSpec.ClearDepthOnLoad = false;
		m_TargetSpec.ShouldSample = false;
		m_TargetSpec.NoResize = false;
		m_TargetSpec.SwapChainTarget = false;
		m_TargetSpec.Blend = false;
	}

	FramebufferBuilder::~FramebufferBuilder()
	{
	}

	FramebufferBuilder &FramebufferBuilder::DebugName(const HLString &name)
	{
		m_TargetSpec.DebugName = HLString(name);
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::ClearColor(const glm::vec4 &clearColor)
	{
		m_TargetSpec.ClearColor = clearColor;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::Samples(uint32 samples)
	{
		m_TargetSpec.Samples = samples;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::Width(uint32 width)
	{
		m_TargetSpec.Width = width;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::Height(uint32 height)
	{
		m_TargetSpec.Height = height;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::Scale(float scale)
	{
		m_TargetSpec.Scale = scale;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::DepthClearValue(float depthClearValue)
	{
		m_TargetSpec.DepthClearValue = depthClearValue;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithClearOnLoad()
	{
		m_TargetSpec.ClearOnLoad = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithClearColorOnLoad()
	{
		m_TargetSpec.ClearColorOnLoad = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithClearDepthOnLoad()
	{
		m_TargetSpec.ClearDepthOnLoad = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithSampling()
	{
		m_TargetSpec.ShouldSample = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithNoResize()
	{
		m_TargetSpec.NoResize = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithSwapchainTarget()
	{
		m_TargetSpec.SwapChainTarget = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::WithBlending()
	{
		m_TargetSpec.Blend = true;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::BlendMode(FramebufferBlendMode blendMode)
	{
		m_TargetSpec.BlendMode = blendMode;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::SetAttachments(const std::vector<FramebufferTextureSpecification> &attachments)
	{
		for (auto &attachment : attachments)
			m_TargetSpec.Attachments.Attachments.push_back(attachment);

		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::SetExistingImage(const Ref<Texture2D> &texture)
	{
		m_TargetSpec.ExistingImage = texture;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::AddExistingImage(const Ref<Texture2D> &texture)
	{
		m_TargetSpec.ExistingImages[m_CurrentImageInsertIndex] = texture;
		++m_CurrentImageInsertIndex;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::ExistingImageLayers(const std::vector<uint32> &layers)
	{
		m_TargetSpec.ExistingImageLayers = layers;
		return *this;
	}

	FramebufferBuilder &FramebufferBuilder::ExistingFramebuffer(const Ref<Framebuffer> &framebuffer)
	{
		m_TargetSpec.ExistingFramebuffer = framebuffer;
		return *this;
	}

	Ref<Framebuffer> FramebufferBuilder::Build(const FramebufferSpecification *overrideSpec)
	{
		if (overrideSpec)
			return Framebuffer::Create(*overrideSpec);

		return Framebuffer::Create(m_TargetSpec);
	}
}

