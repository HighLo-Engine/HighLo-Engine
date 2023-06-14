// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UniformBufferSet.h"

namespace highlo
{
	UniformBufferSet::UniformBufferSet(uint32 frames)
		: m_Frames(frames)
	{
	}

	UniformBufferSet::~UniformBufferSet()
	{
	}

	void UniformBufferSet::CreateUniform(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
	{
		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			Ref<UniformBuffer> uniformBuffer = UniformBuffer::Create(size, binding, layout);
			SetUniform(uniformBuffer, 0, frame);
		}
	}
	
	Ref<UniformBuffer> UniformBufferSet::GetUniform(uint32 binding, uint32 set, uint32 frame)
	{
		HL_ASSERT(m_UniformBuffers.find(frame) != m_UniformBuffers.end());
		HL_ASSERT(m_UniformBuffers.at(frame).find(set) != m_UniformBuffers.at(frame).end());
		HL_ASSERT(m_UniformBuffers.at(frame).at(set).find(binding) != m_UniformBuffers.at(frame).at(set).end());

		return m_UniformBuffers.at(frame).at(set).at(binding);
	}
	
	void UniformBufferSet::SetUniform(const Ref<UniformBuffer> &uniform, uint32 set, uint32 frame)
	{
		m_UniformBuffers[frame][set][uniform->GetBinding()] = uniform;
	}

	void UniformBufferSet::ForEach(const UniformBufferCallback &callback) const
	{
		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			for (auto &[set, buffer] : m_UniformBuffers.at(frame))
			{
				for (auto &[binding, ub] : buffer)
				{
					HL_ASSERT(ub);
					callback(ub);
				}
			}
		}
	}
	
	Ref<UniformBufferSet> UniformBufferSet::Create(uint32 frames)
	{
		return Ref<UniformBufferSet>::Create(frames);
	}
}

