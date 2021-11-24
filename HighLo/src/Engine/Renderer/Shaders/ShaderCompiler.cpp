// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderCompiler.h"

#include "Engine/Platform/OpenGL/OpenGLShaderCompiler.h"
#include "Engine/Platform/Vulkan/VulkanShaderCompiler.h"

namespace highlo
{
	Ref<ShaderCompiler> ShaderCompiler::Create(const FileSystemPath &filePath, const BufferLayout &layout)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLShaderCompiler>::Create(filePath, layout);
	#elif HIGHLO_API_DX11
		return nullptr;// TODO
	#elif HIGHLO_API_DX12
		return nullptr;// TODO
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanShaderCompiler>::Create(filePath, layout);
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

