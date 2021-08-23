#pragma once

#include "Engine/Core/Core.h"

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
			HLVectorIterator tmp = *this;
			++(*this);
			return tmp;
		}

		HLAPI HLVectorIterator &operator--()
		{
			--m_Data;
			return *this;
		}

		HLAPI HLVectorIterator &operator--(int)
		{
			HLVectorIterator tmp = *this;
			--(*this);
			return tmp;
		}

		HLAPI HLVectorIterator &operator+=(uint32 offset)
		{
			m_Data += offset;
			return *this;
		}

		HLAPI HLVectorIterator &operator+(uint32 offset) const
		{
			HLVectorIterator tmp = *this;
			return tmp += offset;
		}

		HLAPI HLVectorIterator &operator-=(uint32 offset)
		{
			m_Data += -offset;
			return *this;
		}

		HLAPI HLVectorIterator &operator-(uint32 offset) const
		{
			HLVectorIterator tmp = *this;
			return tmp -= offset;
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

		HLAPI HLVector(const HLVector &other)
		{
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			ReAllocate(m_Capacity);
			for (uint32 i = 0; i < m_Size; ++i)
			{
				m_Data[i] = std::move(other.m_Data[i]);
			}
		}

		HLAPI HLVector(HLVector &&other) noexcept
		{
			if (this != &other)
			{
				m_Size = other.m_Size;
				m_Capacity = other.m_Capacity;

				ReAllocate(m_Capacity);
				for (uint32 i = 0; i < m_Size; ++i)
				{
					m_Data[i] = std::move(other.m_Data[i]);
				}

				for (uint32 i = 0; i < other.m_Size; ++i)
				{
					other.m_Data[i]->~T();
				}

				other.m_Size = 0;
				other.m_Capacity = 0;
				other.m_Data = nullptr;
			}
		}

		HLAPI HLVector &operator=(const HLVector &other)
		{
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			ReAllocate(m_Capacity);
			for (uint32 i = 0; i < m_Size; ++i)
			{
				m_Data[i] = std::move(other.m_Data[i]);
			}

			return *this;
		}

		HLAPI HLVector &operator=(HLVector &&other) noexcept
		{
			if (this != &other)
			{
				m_Size = other.m_Size;
				m_Capacity = other.m_Capacity;

				ReAllocate(m_Capacity);
				for (uint32 i = 0; i < m_Size; ++i)
				{
					m_Data[i] = std::move(other.m_Data[i]);
				}

				for (uint32 i = 0; i < other.m_Size; ++i)
				{
					other.m_Data[i]->~T();
				}

				other.m_Size = 0;
				other.m_Capacity = 0;
				other.m_Data = nullptr;
			}

			return *this;
		}

		HLAPI ~HLVector()
		{
			Clear();
			// TODO: check why this is crashing
			//::operator delete(m_Data, m_Capacity * sizeof(T));
		}

		HLAPI void PushBack(const T &value)
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			m_Data[m_Size] = value;
			++m_Size;
		}

		HLAPI void PushBack(T &&value)
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
			return m_Data[m_Size++];
		}

		HLAPI void PopBack()
		{
			if (m_Size > 0)
			{
				--m_Size;
				m_Data[m_Size].~T();
			}
		}

		HLAPI void Remove(uint32 index)
		{
			HL_ASSERT(index >= 0 && index < m_Size);

			for (uint32 i = index; i < m_Size - 1; ++i)
			{
				m_Data[i] = m_Data[i + 1];
			}

			--m_Size;
		}

		HLAPI void Clear()
		{
			for (uint32 i = 0; i < m_Size; ++i)
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

		HLAPI bool IsEmpty() const { return m_Size == 0; }
		HLAPI uint32 Size() const { return m_Size; }
		HLAPI uint32 Capacity() const { return m_Capacity; }
		HLAPI T &At(uint32 index) { HL_ASSERT(index >= 0 && index < m_Size); return m_Data[index]; }
		HLAPI const T &At(uint32 index) const { HL_ASSERT(index >= 0 && index < m_Size); return m_Data[index]; }

		HLAPI void Reserve(uint32 capacity)
		{
			if (!m_Data)
			{
				m_Size = 0;
				m_Capacity = 0;
			}

			T *buf = new T[capacity];
			uint32 newSize = capacity < m_Size ? capacity : m_Size;
			for (uint32 i = 0; i < newSize; ++i)
				buf[i] = std::move(m_Data[i]);

			::operator delete(m_Data, m_Capacity * sizeof(T));
			m_Capacity = capacity;
			m_Data = buf;
		}

		HLAPI void Resize(uint32 size)
		{
			Reserve(size);
			m_Size = size;
		}

		HLAPI T &Front()
		{
			return m_Data[0];
		}

		HLAPI T &Back()
		{
			return m_Data[m_Size - 1];
		}

		HLAPI Iterator begin()
		{
			return Iterator(m_Data);
		}

		HLAPI Iterator end()
		{
			return Iterator(m_Data + m_Size);
		}

		HLAPI T &operator[](uint32 index)
		{
			HL_ASSERT(index < m_Size);
			return m_Data[index];
		}

		HLAPI const T &operator[](uint32 index) const
		{
			HL_ASSERT(index < m_Size);
			return m_Data[index];
		}

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const HLVector<T> &vector)
		{
			for (uint32 i = 0; i < vector.Size(); ++i)
				stream << vector[i] << std::endl;

			return stream;
		}
	};
}

