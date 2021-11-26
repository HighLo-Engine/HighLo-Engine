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
	
	void StorageBufferSet::CreateStorage(uint32 size, uint32 binding)
	{
		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			const Ref<StorageBuffer> storageBuffer = StorageBuffer::Create(size, binding);
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
		for (uint32 frame = 0; frame < m_Frames; ++frame)
		{
			m_StorageBuffers.at(frame).at(set).at(binding)->Resize(size);
		}
	}

	Ref<StorageBufferSet> StorageBufferSet::Create(uint32 frames)
	{
		return Ref<StorageBufferSet>::Create(frames);
	}
}

