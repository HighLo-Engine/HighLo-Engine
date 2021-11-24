// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace highlo
{
	class VertexArray : public IsSharedReference
	{
	public:
		
		HLAPI virtual ~VertexArray() {}

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() const = 0;

		HLAPI virtual void Invalidate() = 0;

		HLAPI virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;
		HLAPI virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) = 0;

		HLAPI virtual std::vector<Ref<VertexBuffer>> &GetVertexBuffers() = 0;
		HLAPI virtual Ref<IndexBuffer> &GetIndexBuffer() = 0;

		HLAPI static Ref<VertexArray> Create();
	};
}

