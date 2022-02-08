// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLComputePipeline.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	OpenGLComputePipeline::OpenGLComputePipeline(const Ref<Shader> &computeShader)
		: m_Shader(computeShader)
	{
		Invalidate();
	}

	OpenGLComputePipeline::~OpenGLComputePipeline()
	{
	}

	void OpenGLComputePipeline::Begin(Ref<CommandBuffer> renderCommandBuffer)
	{
	}

	void OpenGLComputePipeline::End()
	{
	}

	void OpenGLComputePipeline::Invalidate()
	{
	}
}

#endif // HIGHLO_API_OPENGL

