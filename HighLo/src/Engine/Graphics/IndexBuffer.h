// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	class IndexBuffer : public IsSharedReference
	{
	public:
		
		HLAPI virtual ~IndexBuffer() = default;

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() const = 0;

		HLAPI virtual uint32 GetCount() = 0;
		HLAPI virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) = 0;
		HLAPI virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) = 0;

		HLAPI virtual HLRendererID GetRendererID() = 0;

		HLAPI static Ref<IndexBuffer> Create(std::vector<int32> &indices);
		HLAPI static Ref<IndexBuffer> Create(void *data, uint32 size);
		HLAPI static Ref<IndexBuffer> Create(uint32 size);
	};
}

