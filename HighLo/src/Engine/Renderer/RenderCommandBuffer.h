#pragma once

namespace highlo
{
	class RenderCommandBuffer : public IsSharedReference
	{
	public:

		virtual ~RenderCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		static Ref<RenderCommandBuffer> Create(uint32 count = 0);
	};
}