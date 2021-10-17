// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderCommandBuffer.h"

namespace highlo
{
	Ref<RenderCommandBuffer> RenderCommandBuffer::Create(const HLString &debugName)
	{
	#ifdef HIGHLO_API_OPENGL
		return nullptr;
	#elif HIGHLO_API_VULKAN

	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<RenderCommandBuffer> RenderCommandBuffer::Create(const HLString &debugName, uint32 count)
	{
	#ifdef HIGHLO_API_OPENGL
		return nullptr;
	#elif HIGHLO_API_VULKAN

	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}