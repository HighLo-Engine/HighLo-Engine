// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "VertexData.h"
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
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout &GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout &layout) = 0;

		virtual void UpdateContents(std::vector<Vertex> &vertices) = 0;
		virtual void UpdateContents(void *data, size_t size) = 0;

		virtual HLRendererID GetRendererID() = 0;
		virtual VertexBufferUsage GetUsage() = 0;

		static Ref<VertexBuffer> Create(std::vector<Vertex> &vertices, VertexBufferUsage usage = VertexBufferUsage::Static);
		static Ref<VertexBuffer> Create(void *data, size_t size, uint32 stride, VertexBufferUsage usage = VertexBufferUsage::Static);
		static Ref<VertexBuffer> Create(size_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
	};
}
