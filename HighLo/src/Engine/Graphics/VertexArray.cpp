// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VertexArray.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLVertexArray.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11VertexArray.h"
#elif HIGHLO_API_DX12
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanVertexArray.h"
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
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanVertexArray>::Create(spec);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

