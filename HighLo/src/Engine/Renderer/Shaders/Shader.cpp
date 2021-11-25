// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Shader.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLShader.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Shader.h"
#elif HIGHLO_API_DX12
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanShader.h"
#endif // HIGHLO_API_OPENGL

#include "Engine/Math/Math.h"

namespace highlo
{
	Ref<Shader> Shader::Create(const FileSystemPath &filePath, bool forceCompile)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLShader>::Create(filePath, forceCompile);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanShader>::Create(filePath, forceCompile);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
	
	Ref<Shader> Shader::CreateFromString(const HLString &source)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLShader>::Create(source);
	#elif HIGHLO_API_DX11
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanShader>::Create(source);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

