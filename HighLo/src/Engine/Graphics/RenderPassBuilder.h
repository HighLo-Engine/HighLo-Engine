// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-27) initial release
//

#pragma once

#include "RenderPass.h"

namespace highlo
{
	class RenderPassBuilder
	{
	public:

		HLAPI RenderPassBuilder();
		HLAPI virtual ~RenderPassBuilder();

		HLAPI RenderPassBuilder &DebugName(const HLString &debugName);
		HLAPI RenderPassBuilder &DebugMarkerColor(const glm::vec4 &color);
		HLAPI RenderPassBuilder &SetFramebuffer(const Ref<Framebuffer> &framebuffer);

		HLAPI Ref<RenderPass> Build(const RenderPassSpecification *overrideSpec = nullptr);

	private:

		RenderPassSpecification m_TargetSpec;
	};
}

