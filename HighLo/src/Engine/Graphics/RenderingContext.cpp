// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderingContext.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLContext.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanContext.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Context.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12Context.h"
#elif HIGHLO_API_METAL
#include "Engine/Platform/Metal/MetalContext.h"
#endif // HIGHLO_API_OPENGL


namespace highlo
{
	Ref<RenderingContext> RenderingContext::Create(void *handle, WindowData &data)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLContext>::Create(handle, data);
	#elif HIGHLO_API_DX11
		return Ref<DX11Context>::Create(handle, data);
	#elif HIGHLO_API_DX12
		return Ref<DX12Context>::Create(handle, data);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanContext>::Create(handle, data);
	#elif HIGHLO_API_METAL
		return Ref<MetalContext>::Create(handle, data);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

