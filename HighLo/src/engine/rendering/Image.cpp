#include "HighLoPch.h"
#include "Image.h"

#ifdef HIGHLO_API_OPENGL
#include "engine/api/opengl/rendering/OpenGLImage.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "engine/api/dx11/rendering/DX11Image.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
	Ref<Image2D> Image2D::Create(ImageFormat format, uint32 width, uint32 height, Allocator buffer)
	{
	#ifdef HIGHLO_API_OPENGL
		return MakeRef<OpenGLImage2D>(format, width, height, buffer);
	#elif HIGHLO_API_DX11
		return MakeRef<DX11Image2D>(format, width, height, buffer);
	#else
		return nullptr;
	#endif
	}

	Ref<Image2D> Image2D::Create(ImageFormat format, uint32 width, uint32 height, const void *data)
	{
	#ifdef HIGHLO_API_OPENGL
		return MakeRef<OpenGLImage2D>(format, width, height, data);
	#elif HIGHLO_API_DX11
		return MakeRef<DX11Image2D>(format, width, height, data);
	#else
		return nullptr;
	#endif
	}
}
