// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SharedReference.h"

namespace highlo
{
	class StorageBuffer : public IsSharedReference
	{
	public:

		HLAPI virtual ~StorageBuffer() = default;

		HLAPI virtual void SetData(const void *data, uint32 size, uint32 offset = 0) = 0;
		HLAPI virtual void Resize(uint32 size) = 0;

		HLAPI virtual uint32 GetBinding() const = 0;

		HLAPI static Ref<StorageBuffer> Create(uint32 size, uint32 binding);
	};
}

