#include "HighLoPch.h"
#include "OpenGLRenderPass.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	OpenGLRenderPass::OpenGLRenderPass(const RenderPassSpecification &spec)
		: m_Specification(spec) {}
	
	OpenGLRenderPass::~OpenGLRenderPass() {}
}

#endif // HIGHLO_API_OPENGL