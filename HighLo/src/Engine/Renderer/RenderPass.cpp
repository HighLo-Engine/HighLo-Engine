// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderPass.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLRenderPass.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11RenderPass.h"
#elif HIGHLO_API_DX12
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanRenderPass.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification &spec)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLRenderPass>::Create(spec);
	#elif HIGHLO_API_DX11
		return Ref<DX11RenderPass>::Create(spec);
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanRenderPass>::Create(spec);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

