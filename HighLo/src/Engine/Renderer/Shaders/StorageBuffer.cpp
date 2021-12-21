// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "StorageBuffer.h"

#include "Engine/Platform/OpenGL/OpenGLStorageBuffer.h"

namespace highlo
{
	Ref<StorageBuffer> StorageBuffer::Create(uint32 size, uint32 binding)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLStorageBuffer>::Create(size, binding);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

