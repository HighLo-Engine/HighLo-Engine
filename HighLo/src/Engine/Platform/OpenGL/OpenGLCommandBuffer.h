#pragma once

#include "Engine/Renderer/CommandBuffer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLCommandBuffer : public CommandBuffer
	{
	public:

		OpenGLCommandBuffer(uint32 count = 0, const HLString &debugName = "");
		OpenGLCommandBuffer(const HLString &debugName = "", bool swapChain = false);
		virtual ~OpenGLCommandBuffer();

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

#endif // HIGHLO_API_OPENGL

