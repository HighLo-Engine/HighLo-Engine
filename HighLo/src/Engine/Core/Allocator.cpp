// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Allocator.h"

namespace highlo
{
	Allocator::Allocator()
		: Data(nullptr), Size(0) {}

	Allocator::Allocator(Byte *data, uint32 size)
		: Data(data), Size(size) {}

	Allocator Allocator::Copy(const void *data, uint32 size)
	{
		Allocator buffer;
		buffer.Allocate(size);
		memcpy(buffer.Data, data, size);
		return buffer;
	}

	void Allocator::Allocate(uint32 size)
	{
		if (size == 0)
			return;

		if (Data)
		{
			delete[] Data;
			Data = nullptr;
		}

		Data = new Byte[size];
		Size = size;
	}

	void *Allocator::AllocateAligned(uint64 size, uint64 alignment)
	{
		return _aligned_malloc((size_t)size, (size_t)alignment);
	}

	void Allocator::Release()
	{
		delete[] Data;
		Data = nullptr;
		Size = 0;
	}

	void Allocator::Free(void *memory)
	{
		if (memory)
			_aligned_free(memory);
	}

	void Allocator::ZeroInitialize()
	{
		if (Data)
			memset(Data, 0, Size);
	}

	void Allocator::Write(void *data, uint32 size, uint32 offset)
	{
		HL_ASSERT(offset + size <= Size, "Buffer Overflow!");
		memcpy(Data + offset, data, size);
	}

	uint32 Allocator::GetSize() const
	{
		return Size;
	}

	Allocator::operator bool() const
	{
		return Data;
	}

	Byte &Allocator::operator[](uint32 i)
	{
		return Data[i];
	}

	const Byte &Allocator::operator[](uint32 i) const
	{
		return Data[i];
	}
}

