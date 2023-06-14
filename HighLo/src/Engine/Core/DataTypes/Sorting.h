// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-22) initial release
//

#pragma once

namespace highlo
{
	class SortingStack
	{
	public:

		int32 m_Count = 0;
		int32 m_Capacity = 0;
		int32 *m_Data = nullptr;

		HLAPI SortingStack();
		HLAPI ~SortingStack();

		HLAPI static SortingStack &Get();

		HLAPI void SetCapacity(int32 capacity);
		HLAPI void EnsureCapacity(int32 minCapacity);

		HLAPI void Push(const int32 item)
		{
			EnsureCapacity(m_Count + 1);
			m_Data[++m_Count] = item;
		}

		HLAPI int32 Pop()
		{
			HL_ASSERT(m_Count > 0);
			const int32 item = m_Data[m_Count - 1];
			--m_Count;
			return item;
		}
	};

	class Sorting
	{
	public:

		template<typename T>
		static void Swap(T &a, T &b) noexcept
		{
			T tmp = a;
			a = b;
			b = tmp;
		}

		template<typename T>
		HLAPI static void QuickSort(T *data, int32 count)
		{
			if (count < 2)
				return;

			// Idea: we use a stack to maintain the left and right border of the array

			auto &stack = SortingStack::Get();
			stack.Push(0); // left border
			stack.Push(count - 1); // right border

			while (stack.m_Count)
			{
				// Get the current left and right borders
				int32 right = stack.Pop();
				const int32 left = stack.Pop();

				// Partition the data
				T *x = &data[right];
				int32 i = left - 1;
				for (int32 j = left; j <= right - 1; ++j)
				{
					if (data[j] < *x)
					{
						++i;
						Swap(data[i], data[j]);
					}
				}

				Swap(data[i + 1], data[right]);
				const int32 pivot = i + 1;

				// If we have elements on the left side of the pivot, push them as the new borders
				if (pivot - 1 > left)
				{
					stack.Push(left);
					stack.Push(pivot - 1);
				}

				// If we have elements of the right side of the pivot, push them as the new borders
				if (pivot + 1 < right)
				{
					stack.Push(pivot + 1);
					stack.Push(right);
				}
			}
		}

		template<typename T>
		HLAPI static void QuickSort(T *data, int32 count, bool (*compare)(const T &a, const T &b))
		{
			if (count < 2)
				return;

			// Idea: we use a stack to maintain the left and right border of the array

			auto &stack = SortingStack::Get();
			stack.Push(0); // left border
			stack.Push(count - 1); // right border

			while (stack.m_Count)
			{
				// Get the current left and right borders
				int32 right = stack.Pop();
				const int32 left = stack.Pop();

				// Partition the data
				T *x = &data[right];
				int32 i = left - 1;
				for (int32 j = left; j <= right - 1; ++j)
				{
					if (compare(data[j], *x))
					{
						++i;
						Swap(data[i], data[j]);
					}
				}

				Swap(data[i + 1], data[right]);
				const int32 pivot = i + 1;

				// If we have elements on the left side of the pivot, push them as the new borders
				if (pivot - 1 > left)
				{
					stack.Push(left);
					stack.Push(pivot - 1);
				}

				// If we have elements of the right side of the pivot, push them as the new borders
				if (pivot + 1 < right)
				{
					stack.Push(pivot + 1);
					stack.Push(right);
				}
			}
		}


	};
}

