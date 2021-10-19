// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-17) Added new CommandBuffer functions
//     - 1.0 (2021-09-14) initial release
//

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

		virtual float GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex = 0) const = 0;

		virtual uint64 BeginTimestampQuery() = 0;
		virtual void EndTimestampQuery(uint64 queryID) = 0;

		static Ref<RenderCommandBuffer> Create(const HLString &debugName = "", uint32 count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapChain(const HLString &debugName = "");
	};
}