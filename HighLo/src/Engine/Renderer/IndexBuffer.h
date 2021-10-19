// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32 GetCount() = 0;
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) = 0;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) = 0;

		virtual HLRendererID GetRendererID() = 0;

		static Ref<IndexBuffer> Create(std::vector<int32>& indices);
		static Ref<IndexBuffer> Create(void *data, uint32 size);
		static Ref<IndexBuffer> Create(uint32 size);
	};
}
