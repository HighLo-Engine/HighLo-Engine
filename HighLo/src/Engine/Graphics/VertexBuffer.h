// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "BufferLayout.h"

namespace highlo
{
	enum class VertexBufferUsage
	{
		None = 0,
		Static = 1,
		Dynamic = 2
	};

	class VertexBuffer : public IsSharedReference
	{
	public:
		
		HLAPI virtual ~VertexBuffer() = default;

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() const = 0;

		HLAPI virtual void SetData(void *data, uint32 size, uint32 offset = 0) = 0;

		HLAPI virtual HLRendererID GetRendererID() = 0;
		HLAPI virtual VertexBufferUsage GetUsage() = 0;

		HLAPI static Ref<VertexBuffer> Create(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		HLAPI static Ref<VertexBuffer> Create(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
	};
}

