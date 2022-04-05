// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ComputePipeline.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLComputePipeline.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11ComputePipeline.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12ComputePipeline.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanComputePipeline.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<ComputePipeline> ComputePipeline::Create(const Ref<Shader> &computeShader)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLComputePipeline>::Create(computeShader);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanComputePipeline>::Create(computeShader);
	#elif HIGHLO_API_DX11
		return Ref<DX11ComputePipeline>::Create(computeShader);
	#elif HIGHLO_API_DX12
		return Ref<DX12ComputePipeline>::Create(computeShader);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

