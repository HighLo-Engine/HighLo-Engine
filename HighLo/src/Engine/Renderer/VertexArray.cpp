#include "HighLoPch.h"
#include "VertexArray.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLVertexArray.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11VertexArray.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<VertexArray> VertexArray::Create()
	{
		return Ref<OpenGLVertexArray>::Create();
	}
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<VertexArray> VertexArray::Create()
	{
		return Ref<DX11VertexArray>::Create();
	}
#endif // HIGHLO_API_DX11
}