#pragma once

#include "Engine/Core/HLCore.h"

namespace highlo
{
	template<typename T>
	class HLVectorIterator
	{
	public:

		using ValueType = typename T::ValueType;

	private:

		ValueType *m_Data;

	public:

		HLAPI HLVectorIterator(ValueType *ptr)
			: m_Data(ptr) {}

		HLAPI HLVectorIterator &operator++()
		{
			++m_Data;
			return *this;
		}

		HLAPI HLVectorIterator &operator++(int)
		{
			HLVectorIterator it = *this;
			++(*this);
			return it;
		}

		HLAPI HLVectorIterator &operator--()
		{
			--m_Data;
			return *this;
		}

		HLAPI HLVectorIterator &operator--(int)
		{
			HLVectorIterator it = *this;
			--(*this);
			return it;
		}

		HLAPI ValueType &operator[](int32 index)
		{
			return *(m_Data + index);
		}

		HLAPI const ValueType &operator[](int32 index) const
		{
			return *(m_Data + index);
		}

		HLAPI ValueType *operator->()
		{
			return m_Data;
		}

		HLAPI const ValueType *operator->() const
		{
			return m_Data;
		}

		HLAPI ValueType &operator*()
		{
			return *m_Data;
		}

		HLAPI const ValueType &operator*() const
		{
			return *m_Data;
		}

		HLAPI bool operator==(const HLVectorIterator &other) const
		{
			return m_Data == other.m_Data;
		}

		HLAPI bool operator!=(const HLVectorIterator &other) const
		{
			return !(*this == other);
		}
	};

	template<typename T>
	class HLVector
	{
	private:

		T *m_Data = nullptr;
		uint32 m_Size = 0;
		uint32 m_Capacity = 0;

		void ReAllocate(uint32 newCapacity)
		{
			// Allocate new block of memory
			T *newBlock = (T*)::operator new(newCapacity * sizeof(T));

			// Move from old block to new block
			if (newCapacity < m_Size)
				m_Size = newCapacity;

			for (uint32 i = 0; i < m_Size; ++i)
				new (&newBlock[i]) T(std::move(m_Data[i]));

			// Delete old block
			::operator delete(m_Data, m_Capacity * sizeof(T));

			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

	public:

		using ValueType = T;
		using Iterator  = HLVectorIterator<HLVector<T>>;

		HLAPI HLVector()
		{
			ReAllocate(2);
		}

		HLAPI ~HLVector()
		{
			Clear();
			::operator delete();
		}

		HLAPI void PushBack(const T &value)
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			m_Data[m_Size] = value;
			++m_Size;
		}

		HLAPI void PushBack(T &&value) noexcept
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			m_Data[m_Size] = std::move(value);
			++m_Size;
		}

		template<typename... Args>
		HLAPI T &EmplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
			++m_Size;
			return m_Data[m_Size];
		}

		HLAPI void PopBack()
		{
			if (m_Size > 0)
			{
				--m_Size;
				m_Data[m_Size].~T();
			}
		}

		HLAPI void Clear()
		{
			for (uint23 i = 0; i < m_Size; ++i)
				m_Data[i].~T();

			m_Size = 0;
		}

		HLAPI void Print()
		{
			for (uint32 i = 0; i < m_Size; ++i)
				std::cout << m_Data[i] << std::endl;
		}

		HLAPI Iterator Find(const T &value)
		{
			for (uint32 i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == value)
					return Iterator(m_Data + i);
			}

			return Iterator(m_Data + m_Size);
		}

		HLAPI uint32 Size() const { return m_Size; }
		HLAPI uint32 Capacity() const { return m_Capacity; }

		HLAPI Iterator Begin()
		{
			return Iterator(m_Data);
		}

		HLAPI Iterator End()
		{
			return Iterator(m_Data + m_Size);
		}

		HLAPI T &operator[](uint32 index)
		{
			HL_ASSERT(index < m_Size);
			return m_Data[m_Size];
		}

		HLAPI const T &operator[](uint32 index) const
		{
			HL_ASSERT(index < m_Size);
			return m_Data[m_Size];
		}

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const HLVector<T> &vector)
		{
			for (uint32 i = 0; i < vector.Size(); ++i)
				stream << vector[i] << std::endl;

			return stream;
		}
	};
}