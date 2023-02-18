// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VertexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/Opengl/OpenGLVertexBuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11VertexBuffer.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12VertexBuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanVertexBuffer.h"
#elif HIGHLO_API_METAL
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<VertexBuffer> VertexBuffer::Create(void *data, uint32 size, VertexBufferUsage usage)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLVertexBuffer>::Create(data, size, usage);
	#elif HIGHLO_API_DX11
		return Ref<DX11VertexBuffer>::Create(data, size, usage);
	#elif HIGHLO_API_DX12
		return Ref<DX12VertexBuffer>::Create(data, size, usage);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanVertexBuffer>::Create(data, size, usage);
	#elif HIGHLO_API_METAL
		HL_ASSERT(false);
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32 size, VertexBufferUsage usage)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLVertexBuffer>::Create(size, usage);
	#elif HIGHLO_API_DX11
		return Ref<DX11VertexBuffer>::Create(size, usage);
	#elif HIGHLO_API_DX12
		return Ref<DX12VertexBuffer>::Create(size, usage);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanVertexBuffer>::Create(size, usage);
	#elif HIGHLO_API_METAL
		HL_ASSERT(false);
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

