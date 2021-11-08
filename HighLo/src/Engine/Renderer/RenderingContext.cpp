// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderingContext.h"

#include "Engine/Platform/OpenGL/OpenGLContext.h"

namespace highlo
{
	UniqueReference<RenderingContext> RenderingContext::Create(void *handle)
	{
	#ifdef HIGHLO_API_OPENGL
		return UniqueReference<OpenGLContext>::Create(handle);
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

