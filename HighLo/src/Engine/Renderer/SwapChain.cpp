#include "HighLoPch.h"
#include "SwapChain.h"

#ifdef HIGHLO_API_OPENGL
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanSwapChain.h"
#endif// HIGHLO_API_OPENGL

namespace highlo
{
	Ref<SwapChain> SwapChain::Create()
	{
	#ifdef HIGHLO_API_OPENGL
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanSwapChain>::Create();
	#endif // HIGHLO_API_OPENGL
	}
}

