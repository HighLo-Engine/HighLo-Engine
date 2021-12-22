// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "CommandBuffer.h"

#include "Engine/Platform/Vulkan/VulkanCommandBuffer.h"

namespace highlo
{
	Ref<CommandBuffer> CommandBuffer::CreateFromSwapChain(const HLString &debugName)
	{
	#ifdef HIGHLO_API_OPENGL
		HL_ASSERT(false, "Not supported in OpenGL!");
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanCommandBuffer>::Create(debugName, true);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<CommandBuffer> CommandBuffer::Create(const HLString &debugName, uint32 count)
	{
	#ifdef HIGHLO_API_OPENGL
		HL_ASSERT(false, "Not supported in OpenGL!");
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanCommandBuffer>::Create(count, debugName);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

