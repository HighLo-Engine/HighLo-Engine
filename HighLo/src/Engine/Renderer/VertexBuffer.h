#pragma once

#include "Engine/Core/HLCore.h"
#include "VertexData.h"
#include "BufferLayout.h"

namespace highlo
{
	class VertexBuffer : public IsSharedReference
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void UpdateContents(std::vector<Vertex>& vertices) = 0;
		virtual void UpdateContents(void* data, size_t size) = 0;

		static Ref<VertexBuffer> Create(std::vector<Vertex>& vertices);
		static Ref<VertexBuffer> Create(void* data, size_t size, uint32 stride);
	};
}
