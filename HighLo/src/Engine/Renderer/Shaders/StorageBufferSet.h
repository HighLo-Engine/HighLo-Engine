#pragma once

#include "StorageBuffer.h"

namespace highlo
{
	class StorageBufferSet : public IsSharedReference
	{
	public:

		HLAPI virtual ~StorageBufferSet() = default;

		HLAPI virtual void CreateStorage(uint32 size, uint32 binding) = 0;

		HLAPI virtual Ref<StorageBuffer> GetStorage(uint32 binding, uint32 set = 0, uint32 frame = 0) = 0;
		HLAPI virtual void SetStorage(const Ref<StorageBuffer> &storageBuffer, uint32 set = 0, uint32 frame = 0) = 0;
		HLAPI virtual void Resize(uint32 binding, uint32 set, uint32 size) = 0;

		HLAPI static Ref<StorageBufferSet> Create(uint32 frames);
	};
}

