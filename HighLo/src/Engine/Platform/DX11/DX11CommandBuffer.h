// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Graphics/CommandBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11CommandBuffer : public CommandBuffer
	{
	public:

		DX11CommandBuffer(uint32 count = 0, const HLString &debugName = "");
		DX11CommandBuffer(const HLString &debugName = "", bool swapChain = false);
		virtual ~DX11CommandBuffer();

		virtual void Begin() override;
		virtual void End() override;
		virtual void Submit() override;

		virtual float GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex = 0) const override;

		virtual uint64 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint64 queryID) override;

	private:

		HLString m_DebugName;
	};
}

#endif // HIGHLO_API_DX11

