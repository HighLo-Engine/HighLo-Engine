// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "StorageBuffer.h"

namespace highlo
{
	class StorageBufferSet : public IsSharedReference
	{
	public:

		using StorageBufferCallback = std::function<void>(const Ref<StorageBuffer&>);

		HLAPI StorageBufferSet(uint32 frames);
		HLAPI virtual ~StorageBufferSet();

		HLAPI void CreateStorage(uint32 size, uint32 binding);

		HLAPI Ref<StorageBuffer> GetStorage(uint32 binding, uint32 set = 0, uint32 frame = 0);
		HLAPI void SetStorage(const Ref<StorageBuffer> &storageBuffer, uint32 set = 0, uint32 frame = 0);
		HLAPI void Resize(uint32 binding, uint32 set, uint32 size);

		HLAPI void ForEach(const StorageBufferCallback &callback);

		HLAPI static Ref<StorageBufferSet> Create(uint32 frames);

	private:

		uint32 m_Frames;
		std::map<uint32, std::map<uint32, std::map<uint32, Ref<StorageBuffer>>>> m_StorageBuffers; // frame -> set -> binding
	};
}

