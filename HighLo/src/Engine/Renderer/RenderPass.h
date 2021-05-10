#pragma once

#include "Framebuffer.h"

namespace highlo
{
	struct RenderPassSpecification
	{
		Ref<Framebuffer> Framebuffer;
		HLString DebugName;
	};

	class RenderPass : public IsSharedReference
	{
	public:

		HLAPI virtual ~RenderPass() = default;

		HLAPI virtual RenderPassSpecification &GetSpcification() = 0;
		HLAPI virtual const RenderPassSpecification &GetSpcification() const = 0;

		HLAPI static Ref<RenderPass> Create(const RenderPassSpecification &spec);
	};
}
