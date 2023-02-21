// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "IndexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLIndexBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11IndexBuffer.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12IndexBuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanIndexBuffer.h"
#elif HIGHLO_API_METAL
#include "Engine/Platform/Metal/MetalIndexBuffer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<IndexBuffer> IndexBuffer::Create(std::vector<int32> &indices)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLIndexBuffer>::Create(indices);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		return Ref<DX12IndexBuffer>::Create(indices);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanIndexBuffer>::Create(indices);
	#elif HIGHLO_API_METAL
		return Ref<MetalIndexBuffer>::Create(indices);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<IndexBuffer> IndexBuffer::Create(void *data, uint32 size)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLIndexBuffer>::Create(data, size);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		return Ref<DX12IndexBuffer>::Create(data, size);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanIndexBuffer>::Create(data, size);
	#elif HIGHLO_API_METAL
		return Ref<MetalIndexBuffer>::Create(data, size);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32 size)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLIndexBuffer>::Create(size);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		return Ref<DX12IndexBuffer>::Create(size);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanIndexBuffer>::Create(size);
	#elif HIGHLO_API_METAL
		return Ref<MetalIndexBuffer>::Create(size);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif
	}
}

