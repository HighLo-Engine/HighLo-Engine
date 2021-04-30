#include "HighLoPch.h"
#include "VertexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/Opengl/Rendering/OpenGLVertexBuffer.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/Rendering/DX11VertexBuffer.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices)
	{
		return Ref<VertexBuffer>(new OpenGLVertexBuffer(vertices));
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size, uint32 stride)
	{
		return Ref<VertexBuffer>(new OpenGLVertexBuffer(data, size, stride));
	}
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices)
	{
		return Ref<VertexBuffer>(new DX11VertexBuffer(vertices));
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size, uint32 stride)
	{
		return Ref<VertexBuffer>(new DX11VertexBuffer(data, size, stride));
	}
#endif // HIGHLO_API_DX11
}