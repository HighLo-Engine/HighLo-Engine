// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "CommandBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLCommandBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11CommandBuffer.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12CommandBuffer.h"
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
		return Ref<DX11CommandBuffer>::Create(debugName, true);
	#elif HIGHLO_API_DX12
		return Ref<DX12CommandBuffer>::Create(debugName, true);
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
		return Ref<DX11CommandBuffer>::Create(count, debugName);
	#elif HIGHLO_API_DX12
		return Ref<DX12CommandBuffer>::Create(count, debugName);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

