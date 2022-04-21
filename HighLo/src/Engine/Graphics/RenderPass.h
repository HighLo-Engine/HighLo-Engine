// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Framebuffer.h"

namespace highlo
{
	struct RenderPassSpecification
	{
		Ref<Framebuffer> Framebuffer;
		HLString DebugName;
		glm::vec4 DebugMarkerColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class RenderPass : public IsSharedReference
	{
	public:

		HLAPI virtual ~RenderPass() = default;

		HLAPI virtual RenderPassSpecification &GetSpecification() = 0;
		HLAPI virtual const RenderPassSpecification &GetSpecification() const = 0;

		HLAPI static Ref<RenderPass> Create(const RenderPassSpecification &spec);
	};
}
