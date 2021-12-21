// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SwapChain.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLSwapChain.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanSwapChain.h"
#endif// HIGHLO_API_OPENGL

namespace highlo
{
	Ref<SwapChain> SwapChain::Create()
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLSwapChain>::Create();
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanSwapChain>::Create();
	#endif // HIGHLO_API_OPENGL
	}
}

