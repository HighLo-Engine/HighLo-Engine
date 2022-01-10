// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "CommandBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLCommandBuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanCommandBuffer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<CommandBuffer> CommandBuffer::CreateFromSwapChain(const HLString &debugName)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLCommandBuffer>::Create(debugName, true);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanCommandBuffer>::Create(debugName, true);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false, "Not supported in DX11");
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false, "Not supported in DX12");
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<CommandBuffer> CommandBuffer::Create(const HLString &debugName, uint32 count)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLCommandBuffer>::Create(count, debugName);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanCommandBuffer>::Create(count, debugName);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false, "Not supported in DX11");
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false, "Not supported in DX12");
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

