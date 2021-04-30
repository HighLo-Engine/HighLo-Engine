#include "HighLoPch.h"
#include "IndexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/Rendering/OpenGLIndexBuffer.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/Rendering/DX11IndexBuffer.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<IndexBuffer> IndexBuffer::Create(std::vector<int>& indices)
	{
		return Ref<IndexBuffer>(new OpenGLIndexBuffer(indices));
	}
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<IndexBuffer> IndexBuffer::Create(std::vector<int>& indices)
	{
		return Ref<IndexBuffer>(new DX11IndexBuffer(indices));
	}
#endif // HIGHLO_API_DX11
}