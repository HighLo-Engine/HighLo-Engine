// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGuiRenderer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLImGuiRenderer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanImGuiRenderer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11ImGuiRenderer.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12ImGuiRenderer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<ImGuiRenderer> ImGuiRenderer::Create()
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLImGuiRenderer>::Create();
	#elif HIGHLO_API_DX11
		return Ref<DX11ImGuiRenderer>::Create();
	#elif HIGHLO_API_DX12
		return Ref<DX12ImGuiRenderer>::Create();
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanImGuiRenderer>::Create();
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

