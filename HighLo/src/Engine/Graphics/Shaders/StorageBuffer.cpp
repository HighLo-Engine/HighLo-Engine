// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "StorageBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLStorageBuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanStorageBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11StorageBuffer.h"
#elif HIGHLO_API_DX12
// TODO
#include "Engine/Platform/DX11/DX11StorageBuffer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<StorageBuffer> StorageBuffer::Create(uint32 size, uint32 binding)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLStorageBuffer>::Create(size, binding);
	#elif HIGHLO_API_DX11
		return Ref<DX11StorageBuffer>::Create(size, binding);
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanStorageBuffer>::Create(size, binding);
	#endif // HIGHLO_API_OPENGL
	}
}

