#include "HighLoPch.h"
#include "Allocator.h"

namespace highlo
{
	Allocator::Allocator()
		: m_Data(nullptr), m_Size(0) {}

	Allocator::Allocator(Byte *data, uint32 size)
		: m_Data(data), m_Size(size) {}

	Allocator Allocator::Copy(const void *data, uint32 size)
	{
		Allocator buffer;
		buffer.Allocate(size);
		memcpy(buffer.m_Data, data, size);
		return buffer;
	}

	void Allocator::Allocate(uint32 size)
	{
		delete[] m_Data;
		m_Data = nullptr;

		if (size == 0)
			return;

		m_Data = new Byte[size];
		m_Size = size;
	}

	void *Allocator::Allocate(uint64 size, uint64 alignment)
	{
		return _aligned_malloc((size_t)size, (size_t)alignment);
	}

	void Allocator::Release()
	{
		delete[] m_Data;
		m_Data = nullptr;
		m_Size = 0;
	}

	void Allocator::Free(void *memory)
	{
		_aligned_free(memory);
	}

	void Allocator::ZeroInitialize()
	{
		if (m_Data)
			memset(m_Data, 0, m_Size);
	}

	void Allocator::Write(void *data, uint32 size, uint32 offset)
	{
		HL_ASSERT(offset + size <= m_Size, "Buffer Overflow!");
		memcpy(m_Data + offset, data, size);
	}

	uint32 Allocator::GetSize() const
	{
		return m_Size;
	}

	Allocator::operator bool() const
	{
		return m_Data;
	}

	Byte &Allocator::operator[](uint32 i)
	{
		return m_Data[i];
	}

	const Byte &Allocator::operator[](uint32 i) const
	{
		return m_Data[i];
	}
}