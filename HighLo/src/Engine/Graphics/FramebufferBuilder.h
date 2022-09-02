// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-02) initial release
//

#pragma once

#include "Framebuffer.h"

namespace highlo
{
	class FramebufferBuilder
	{
	public:

		HLAPI FramebufferBuilder();
		HLAPI virtual ~FramebufferBuilder();

		HLAPI FramebufferBuilder &DebugName(const HLString &name);
		HLAPI FramebufferBuilder &ClearColor(const glm::vec4 &clearColor);
		HLAPI FramebufferBuilder &Samples(uint32 samples);
		HLAPI FramebufferBuilder &Width(uint32 width);
		HLAPI FramebufferBuilder &Height(uint32 height);
		HLAPI FramebufferBuilder &Scale(float scale);
		HLAPI FramebufferBuilder &DepthClearValue(float depthClearValue);
		HLAPI FramebufferBuilder &WithClearOnLoad();
		HLAPI FramebufferBuilder &WithClearColorOnLoad();
		HLAPI FramebufferBuilder &WithClearDepthOnLoad();
		HLAPI FramebufferBuilder &WithSampling();
		HLAPI FramebufferBuilder &WithNoResize();
		HLAPI FramebufferBuilder &WithSwapchainTarget();
		HLAPI FramebufferBuilder &WithBlending();
		HLAPI FramebufferBuilder &BlendMode(FramebufferBlendMode blendMode);
		
		HLAPI FramebufferBuilder &SetAttachments(const std::initializer_list<FramebufferTextureSpecification> &attachments);
		HLAPI FramebufferBuilder &SetExistingImage(const Ref<Texture2D> &texture);
		HLAPI FramebufferBuilder &AddExistingImage(const Ref<Texture2D> &texture);
		HLAPI FramebufferBuilder &ExistingImageLayers(const std::vector<uint32> &layers);
		HLAPI FramebufferBuilder &ExistingFramebuffer(const Ref<Framebuffer> &framebuffer);

		HLAPI Ref<Framebuffer> Build(const FramebufferSpecification *overrideSpec = nullptr);

	private:

		FramebufferSpecification m_TargetSpec = {};
		uint32 m_CurrentImageInsertIndex = 0;
	};
}

