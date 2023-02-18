// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VertexArray.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLVertexArray.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11VertexArray.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12VertexArray.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanVertexArray.h"
#elif HIGHLO_API_METAL
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<VertexArray> VertexArray::Create(const VertexArraySpecification &spec)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLVertexArray>::Create(spec);
	#elif HIGHLO_API_DX11
		return Ref<DX11VertexArray>::Create(spec);
	#elif HIGHLO_API_DX12
		return Ref<DX12VertexArray>::Create(spec);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanVertexArray>::Create(spec);
	#elif HIGHLO_API_METAL
		HL_ASSERT(false);
		return nullptr;
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

