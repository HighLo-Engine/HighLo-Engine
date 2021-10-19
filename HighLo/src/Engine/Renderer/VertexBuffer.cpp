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
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex> &vertices, VertexBufferUsage usage)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLVertexBuffer>::Create(vertices, usage);
	#elif HIGHLO_API_DX11
		return Ref<DX11VertexBuffer>::Create(vertices, usage);
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<VertexBuffer> VertexBuffer::Create(void *data, uint32 size, VertexBufferUsage usage)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLVertexBuffer>::Create(data, size, usage);
	#elif HIGHLO_API_DX11
		return Ref<DX11VertexBuffer>::Create(data, size, usage);
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32 size, VertexBufferUsage usage)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLVertexBuffer>::Create(size, usage);
	#elif HIGHLO_API_DX11
		return Ref<DX11VertexBuffer>::Create(size, usage);
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}