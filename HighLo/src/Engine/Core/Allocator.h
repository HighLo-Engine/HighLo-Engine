// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"

namespace highlo
{
	struct Allocator
	{
		Byte *Data;
		uint32 Size;

		HLAPI Allocator();
		HLAPI Allocator(Byte *data, uint32 size);

		HLAPI static Allocator Copy(const void *data, uint32 size);
		HLAPI static void *AllocateAligned(uint64 size, uint64 alignment = 16);
		HLAPI static void Free(void *memory);

		HLAPI void Resize(uint32 size);

		HLAPI void Allocate(uint32 size);
		HLAPI void Release();
		HLAPI void ZeroInitialize();
		HLAPI void Write(void *data, uint32 size, uint32 offset = 0);
		HLAPI uint32 GetSize() const;

		HLAPI operator bool() const;
		HLAPI Byte &operator[](uint32 i);
		HLAPI const Byte &operator[](uint32 i) const;

		template<typename T>
		HLAPI FORCEINLINE T &Read(uint32 offset = 0)
		{
			HL_ASSERT(offset < Size);
			return *(T*)(Data + offset);
		}

		template<typename T>
		HLAPI FORCEINLINE T *As()
		{
			return (T*)Data;
		}
	};
}
