#include "HighLoPch.h"
#include "ComputePipeline.h"

#include "Engine/Platform/Vulkan/VulkanComputePipeline.h"

namespace highlo
{
	Ref<ComputePipeline> ComputePipeline::Create(Ref<Shader> computeShader)
	{
	#ifdef HIGHLO_API_OPENGL
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanComputePipeline>::Create(computeShader);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

