// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SwapChain.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLSwapChain.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11SwapChain.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12SwapChain.h"
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
		return Ref<DX11SwapChain>::Create();
	#elif HIGHLO_API_DX12
		return Ref<DX12SwapChain>::Create();
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanSwapChain>::Create();
	#endif // HIGHLO_API_OPENGL
	}
}

