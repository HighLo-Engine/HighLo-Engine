// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGuiRenderer.h"

#include "Engine/Platform/OpenGL/OpenGLImGuiRenderer.h"
#include "Engine/Platform/Vulkan/VulkanImGuiRenderer.h"
#include "Engine/Platform/DX11/DX11ImGuiRenderer.h"

namespace highlo
{
	Ref<ImGuiRenderer> ImGuiRenderer::Create()
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLImGuiRenderer>::Create();
	#elif HIGHLO_API_DX11
		return Ref<DX11ImGuiRenderer>::Create();
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanImGuiRenderer>::Create();
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

