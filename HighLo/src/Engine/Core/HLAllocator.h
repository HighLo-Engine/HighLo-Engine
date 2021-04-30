#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLLog.h"

namespace highlo
{
	struct Allocator
	{
		Byte *m_Data;
		uint32 m_Size;

		HLAPI Allocator();
		HLAPI Allocator(Byte *data, uint32 size);

		HLAPI static Allocator Copy(const void *data, uint32 size);
		HLAPI static void *Allocate(uint64 size, uint64 alignment = 16);
		HLAPI static void Free(void *memory);

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
			return *(T*)(m_Data + offset);
		}

		template<typename T>
		HLAPI FORCEINLINE T *As()
		{
			return (T*)m_Data;
		}
	};
}