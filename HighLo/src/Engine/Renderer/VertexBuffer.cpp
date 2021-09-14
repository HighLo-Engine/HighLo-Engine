// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VertexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/Opengl/OpenGLVertexBuffer.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11VertexBuffer.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices, VertexBufferUsage usage)
	{
		return Ref<OpenGLVertexBuffer>::Create(vertices, usage);
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size, uint32 stride, VertexBufferUsage usage)
	{
		return Ref<OpenGLVertexBuffer>::Create(data, size, stride, usage);
	}

	Ref<VertexBuffer> VertexBuffer::Create(size_t size, VertexBufferUsage usage)
	{
		return Ref<OpenGLVertexBuffer>::Create(size, usage);
	}
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices, VertexBufferUsage usage)
	{
		return Ref<DX11VertexBuffer>::Create(vertices);
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size, uint32 stride, VertexBufferUsage usage)
	{
		return Ref<DX11VertexBuffer>::Create(data, size, stride);
	}
#endif // HIGHLO_API_DX11
}