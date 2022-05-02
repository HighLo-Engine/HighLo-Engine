#include "HighLoPch.h"
#include "OpenGLCommandBuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLCommandBuffer::OpenGLCommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName)
	{
	}

	OpenGLCommandBuffer::OpenGLCommandBuffer(const HLString &debugName, bool swapChain)
		: m_DebugName(debugName)
	{
	}
	
	OpenGLCommandBuffer::~OpenGLCommandBuffer()
	{
	}
	
	void OpenGLCommandBuffer::Begin()
	{
	}
	
	void OpenGLCommandBuffer::End()
	{
	}
	
	void OpenGLCommandBuffer::Submit()
	{
	}
	
	float OpenGLCommandBuffer::GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}
	
	uint32 OpenGLCommandBuffer::BeginTimestampQuery()
	{
		return 0;
	}
	
	void OpenGLCommandBuffer::EndTimestampQuery(uint32 queryID)
	{
	}
	const PipelineStatistics &OpenGLCommandBuffer::GetPipelineStatistics(uint32 frameIndex) const
	{
		HL_ASSERT(frameIndex < m_PipelineStatisticsQueryResults.size());
		return m_PipelineStatisticsQueryResults[frameIndex];
	}
}

#endif // HIGHLO_API_OPENGL

