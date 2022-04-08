// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderPreProcessor.h"

#ifdef HIGHLO_API_OPENGL
#elif HIGHLO_API_DX11
#elif HIGHLO_API_DX12
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanShaderPreProcessor.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<ShaderPreProcessor> ShaderPreProcessor::Create()
	{
	#ifdef HIGHLO_API_OPENGL
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanShaderPreProcessor>::Create();
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

