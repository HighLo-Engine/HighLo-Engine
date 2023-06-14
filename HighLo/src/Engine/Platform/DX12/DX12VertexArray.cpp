// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12VertexArray.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12VertexArray::DX12VertexArray(const VertexArraySpecification &spec)
		: m_Specification(spec)
	{
		HL_ASSERT(spec.Shader);
		HL_ASSERT(spec.RenderPass);
		Invalidate();
	}

	DX12VertexArray::~DX12VertexArray()
	{
	}
	
	void DX12VertexArray::Bind() const
	{
	}
	
	void DX12VertexArray::Unbind() const
	{
	}
	
	void DX12VertexArray::Invalidate()
	{
	}
	
	void DX12VertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}
	
	void DX12VertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
}

#endif // HIGHLO_API_DX12

