// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UniformBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLUniformBuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanUniformBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11UniformBuffer.h"
#elif HIGHLO_API_DX12
// TODO
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32 size, uint32 binding)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLUniformBuffer>::Create(size, binding);
	#elif HIGHLO_API_DX11
		return Ref<DX11UniformBuffer>::Create(size, binding);
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanUniformBuffer>::Create(size, binding);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

