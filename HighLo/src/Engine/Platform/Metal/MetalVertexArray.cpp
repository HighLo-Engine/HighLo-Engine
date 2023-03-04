// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalVertexArray.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalVertexArray::MetalVertexArray(const VertexArraySpecification &spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	MetalVertexArray::~MetalVertexArray()
	{
		m_VertexBuffers.clear();
		m_VertexBuffers.shrink_to_fit();
		m_IndexBuffer = nullptr;
	}

	void MetalVertexArray::Bind() const
	{
	}
	
	void MetalVertexArray::Unbind() const
	{
	}
	
	void MetalVertexArray::Invalidate()
	{
	}
	
	void MetalVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void MetalVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
}

#endif // HIGHLO_API_METAL

