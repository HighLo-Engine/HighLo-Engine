// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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