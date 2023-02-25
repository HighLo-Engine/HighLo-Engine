// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-17) Added new CommandBuffer functions
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "PipelineStatistics.h"

namespace highlo
{
	class CommandBuffer : public IsSharedReference
	{
	public:

		HLAPI virtual ~CommandBuffer() = default;

		HLAPI virtual void Begin() = 0;
		HLAPI virtual void End() = 0;
		HLAPI virtual void Submit() = 0;

		HLAPI virtual float GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex = 0) const = 0;

		HLAPI virtual uint32 BeginTimestampQuery() = 0;
		HLAPI virtual void EndTimestampQuery(uint32 queryID) = 0;

		HLAPI virtual const PipelineStatistics &GetPipelineStatistics(uint32 frameIndex) const = 0;

		HLAPI static Ref<CommandBuffer> Create(const HLString &debugName = "", uint32 count = 0);
		HLAPI static Ref<CommandBuffer> CreateFromSwapChain(const HLString &debugName = "");
	};
}

