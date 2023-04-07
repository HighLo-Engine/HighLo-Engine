#include "HighLoPch.h"
#include "OpenGLCommandBuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	OpenGLCommandBuffer::OpenGLCommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName)
	{
		// Timestamp queries
		const uint32 maxUserQueries = 16;
		m_TimestampQueryCount = 2 + 2 * maxUserQueries;
		m_TimestampQueryPool.resize(m_TimestampQueryCount);

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		m_ExecutionGPUTimes.resize(framesInFlight);
		for (auto &execution : m_ExecutionGPUTimes)
			execution.resize(m_TimestampQueryCount / 2);

		Ref<OpenGLCommandBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			for (uint32 i = 0; i < instance->m_TimestampQueryCount; ++i)
			{
				glGenQueries(1, &instance->m_TimestampQueryPool[i]);
			}
		});
	}

	OpenGLCommandBuffer::OpenGLCommandBuffer(const HLString &debugName, bool swapChain)
		: m_DebugName(debugName)
	{
		// Timestamp queries
		const uint32 maxUserQueries = 16;
		m_TimestampQueryCount = 2 + 2 * maxUserQueries;
		m_TimestampQueryPool.resize(m_TimestampQueryCount);

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		m_ExecutionGPUTimes.resize(framesInFlight);
		for (auto &execution : m_ExecutionGPUTimes)
			execution.resize(m_TimestampQueryCount / 2);

		Ref<OpenGLCommandBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			for (uint32 i = 0; i < instance->m_TimestampQueryCount; ++i)
			{
				glGenQueries(1, &instance->m_TimestampQueryPool[i]);
			}
		});
	}
	
	OpenGLCommandBuffer::~OpenGLCommandBuffer()
	{
		Renderer::SubmitWithoutResources([&]()
		{
			for (uint32 i = 0; i < m_TimestampQueryCount; ++i)
			{
				glDeleteQueries(1, &m_TimestampQueryPool[i]);
			}
		});
	}
	
	void OpenGLCommandBuffer::Begin()
	{
	}
	
	void OpenGLCommandBuffer::End()
	{
	}
	
	void OpenGLCommandBuffer::Submit()
	{
		Ref<OpenGLCommandBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			for (uint32 i = 0; i < instance->m_TimestampNextAvailQuery; i += 2)
			{
				uint64 startTime, stopTime;
				glGetQueryObjectui64v(instance->m_TimestampQueryPool[i], GL_QUERY_RESULT, &startTime);
				glGetQueryObjectui64v(instance->m_TimestampQueryPool[i + 1], GL_QUERY_RESULT, &stopTime);

				float elapsed_time = (float)((stopTime - startTime) / 1000000.0f);
				instance->m_ExecutionGPUTimes[frameIndex][i / 2] = elapsed_time;
			}
		});
	}
	
	float OpenGLCommandBuffer::GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex) const
	{
		if (queryIndex / 2 >= m_TimestampNextAvailQuery / 2)
			return 0.0f;

		return m_ExecutionGPUTimes[frameIndex][queryIndex / 2];
	}
	
	uint32 OpenGLCommandBuffer::BeginTimestampQuery()
	{
		HL_ASSERT(m_TimestampNextAvailQuery < m_TimestampQueryCount);
		uint32 queryIndex = m_TimestampNextAvailQuery;
		m_TimestampNextAvailQuery += 2;

		Ref<OpenGLCommandBuffer> instance = this;
		Renderer::Submit([instance, queryIndex]()
		{
			glQueryCounter(instance->m_TimestampQueryPool[queryIndex], GL_TIMESTAMP);
		});

		return queryIndex;
	}
	
	void OpenGLCommandBuffer::EndTimestampQuery(uint32 queryIndex)
	{
		Ref<OpenGLCommandBuffer> instance = this;
		Renderer::Submit([instance, queryIndex]()
		{
			glQueryCounter(instance->m_TimestampQueryPool[queryIndex + 1], GL_TIMESTAMP);
		});
	}

	const PipelineStatistics &OpenGLCommandBuffer::GetPipelineStatistics(uint32 frameIndex) const
	{
		HL_ASSERT(frameIndex < m_PipelineStatisticsQueryResults.size());
		return m_PipelineStatisticsQueryResults[frameIndex];
	}
}

#endif // HIGHLO_API_OPENGL

