// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UniformBuffer.h"

#include "Engine/Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Engine/Platform/DX11/DX11UniformBuffer.h"

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
		HL_ASSERT(false);
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

