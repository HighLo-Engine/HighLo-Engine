#include "HighLoPch.h"
#include "RenderPass.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLRenderPass.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification &spec)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLRenderPass>::Create(spec);
	#endif // HIGHLO_API_OPENGL
	}
}
