// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Sorting.h"

#include "Engine/Threading/LocalThread.h"
#include "Engine/Core/Allocator.h"

namespace highlo
{
	static LocalThread<SortingStack> s_SortingStacks;

	SortingStack::SortingStack()
	{
	}

	SortingStack::~SortingStack()
	{
		Allocator::Free(m_Data);
	}

	SortingStack &SortingStack::Get()
	{
		return s_SortingStacks.Get();
	}

	void SortingStack::SetCapacity(int32 capacity)
	{
		HL_ASSERT(capacity >= 0);
		if (m_Capacity == capacity)
			return;

		int32 *newData = nullptr;
		if (capacity > 0)
			newData = (int32*)malloc(capacity * sizeof(int32));

		const int32 newCount = m_Count < capacity ? m_Count : capacity;
		if (m_Data)
		{
			if (newData && newCount)
				memcpy(newData, m_Data, newCount * sizeof(int32));

			delete[] m_Data;
		}

		m_Data = newData;
		m_Capacity = capacity;
		m_Count = newCount;
	}

	void SortingStack::EnsureCapacity(int32 minCapacity)
	{
		if (m_Capacity >= minCapacity)
			return;

		int32 newCapacity = m_Capacity == 0 ? 64 : m_Capacity * 2;
		if (newCapacity < minCapacity)
			newCapacity = minCapacity;

		SetCapacity(newCapacity);
	}
}

