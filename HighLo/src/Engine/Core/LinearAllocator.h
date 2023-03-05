// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-05) initial release
//

#pragma once

namespace highlo
{
	class LinearAllocator
	{
	public:

		/// <summary>
		/// Allocates maxSize memory from the OS, and holds on to the memory until Shutdown() is called. 
		/// If the user provides their own memory, no allocations are done by the linear allocator.
		/// </summary>
		/// <param name="maxSize">The total size to allocate and hold onto during the lifetime of the LinearAllocator.</param>
		/// <param name="memory">If provided, the memory will be used for holding the data and no allocations will be done.</param>
		HLAPI void Init(uint64 maxSize, void *memory = nullptr);
		
		/// <summary>
		/// Frees the internal memory, if the data is owned by the Linear allocator, 
		/// otherwise no free operation is done by the allocator itself and the data has to be freed externally.
		/// </summary>
		HLAPI void Shutdown();

		/// <summary>
		/// Determines the current position in the internal memory block and provides a memory block with the requested size.
		/// </summary>
		/// <param name="size">The size the user needs for their operation.</param>
		/// <returns>Returns a valid memory block on success, nullptr otherwise.</returns>
		HLAPI void *Allocate(uint64 size);

		/// <summary>
		/// Resets the LinearAllocator and zeros out the internal memory.
		/// </summary>
		HLAPI void FreeAll();

	private:

		void *m_Memory = nullptr;
		uint64 m_TotalSize = 0;
		uint64 m_Allocated = 0;
		bool m_OwnsMemory = false;
	};
}