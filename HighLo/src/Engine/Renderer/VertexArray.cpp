#include "HighLoPch.h"
#include "VertexArray.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/Rendering/OpenGLVertexArray.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/Rendering/DX11VertexArray.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<VertexArray> VertexArray::Create()
	{
		return Ref<VertexArray>(new OpenGLVertexArray());
	}
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<VertexArray> VertexArray::Create()
	{
		return Ref<VertexArray>(new DX11VertexArray());
	}
#endif // HIGHLO_API_DX11
}