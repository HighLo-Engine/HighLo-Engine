#include "HighLoPch.h"
#include "Image.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/Rendering/OpenGLImage.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/Rendering/DX11Image.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
	Ref<Image2D> Image2D::Create(ImageFormat format, uint32 width, uint32 height, Allocator buffer)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLImage2D>::Create(format, width, height, buffer);
	#elif HIGHLO_API_DX11
		return Ref<DX11Image2D>::Create(format, width, height, buffer);
	#else
		return nullptr;
	#endif
	}

	Ref<Image2D> Image2D::Create(ImageFormat format, uint32 width, uint32 height, const void *data)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLImage2D>::Create(format, width, height, data);
	#elif HIGHLO_API_DX11
		return Ref<DX11Image2D>::Create(format, width, height, data);
	#else
		return nullptr;
	#endif
	}
}
