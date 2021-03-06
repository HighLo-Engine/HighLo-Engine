// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "StorageBufferSet.h"

namespace highlo
{
	StorageBufferSet::StorageBufferSet(uint32 frames)
		: m_Frames(frames)
	{
	}
	
	StorageBufferSet::~StorageBufferSet()
	{
	}
	
	void StorageBufferSet::CreateStorage(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
	{
		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			const Ref<StorageBuffer> storageBuffer = StorageBuffer::Create(size, binding, layout);
			SetStorage(storageBuffer, 0, frame);
		}
	}
	
	Ref<StorageBuffer> StorageBufferSet::GetStorage(uint32 binding, uint32 set, uint32 frame)
	{
		HL_ASSERT(m_StorageBuffers.find(frame) != m_StorageBuffers.end());
		HL_ASSERT(m_StorageBuffers.at(frame).find(set) != m_StorageBuffers.at(frame).end());
		HL_ASSERT(m_StorageBuffers.at(frame).at(set).find(binding) != m_StorageBuffers.at(frame).at(set).end());

		return m_StorageBuffers.at(frame).at(set).at(binding);
	}
	
	void StorageBufferSet::SetStorage(const Ref<StorageBuffer> &storageBuffer, uint32 set, uint32 frame)
	{
		m_StorageBuffers[frame][set][storageBuffer->GetBinding()] = storageBuffer;
	}
	
	void StorageBufferSet::Resize(uint32 binding, uint32 set, uint32 size)
	{
		// Nothing to do, if there are no storage buffers stored
		if (!m_StorageBuffers.size())
			return;

		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			m_StorageBuffers.at(frame).at(set).at(binding)->Resize(size);
		}
	}

	void StorageBufferSet::ForEach(const StorageBufferCallback &callback) const
	{
		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			for (uint32 set = 0; set < m_StorageBuffers.at(frame).size(); ++set)
			{
				for (uint32 binding = 0; binding < m_StorageBuffers.at(frame).at(set).size(); ++binding)
				{
					callback(m_StorageBuffers.at(frame).at(set).at(binding));
				}
			}
		}
	}

	Ref<StorageBufferSet> StorageBufferSet::Create(uint32 frames)
	{
		return Ref<StorageBufferSet>::Create(frames);
	}
}

