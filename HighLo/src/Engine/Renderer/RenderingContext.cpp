// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderingContext.h"

#include "Engine/Platform/OpenGL/OpenGLContext.h"
#include "Engine/Platform/Vulkan/VulkanContext.h"
#include "Engine/Platform/DX11/DX11Context.h"

namespace highlo
{
	Ref<RenderingContext> RenderingContext::Create(void *handle)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLContext>::Create(handle);
	#elif HIGHLO_API_DX11
		return Ref<DX11Context>::Create(handle);
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanContext>::Create(handle);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

