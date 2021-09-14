// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "IndexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLIndexBuffer.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11IndexBuffer.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<IndexBuffer> IndexBuffer::Create(std::vector<int32>& indices)
	{
		return Ref<OpenGLIndexBuffer>::Create(indices);
	}

	Ref<IndexBuffer> IndexBuffer::Create(void *data, uint32 size)
	{
		return Ref<OpenGLIndexBuffer>::Create(data, size);
	}
	
	Ref<IndexBuffer> IndexBuffer::Create(uint32 size)
	{
		return Ref<OpenGLIndexBuffer>::Create(size);
	}
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<IndexBuffer> IndexBuffer::Create(std::vector<int32>& indices)
	{
		return Ref<DX11IndexBuffer>::Create(indices);
	}
#endif // HIGHLO_API_DX11
}