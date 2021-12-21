// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11Shader.h"

#include "Engine/Core/FileSystem.h"

/// @note @FlareFlax:
/// for DirectX we probably need HLSL, but our shaders are only written in glsl. (no problem with SPIR-V ;))
/// SPIR-V can cross-compile also to hlsl, so we need to use it here also
/// @see: https://vulkan.lunarg.com/doc/view/1.2.154.1/windows/spirv_toolchain.html

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11Shader::DX11Shader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
	}

	DX11Shader::DX11Shader(const HLString &source)
	{
		m_Name = "unknown"; // TODO: maybe we should add this as a parameter as well for all apis, so that the user can still access this shader through the shader library
	}

	DX11Shader::~DX11Shader()
	{
	}

	void DX11Shader::Reload(bool forceCompile)
	{
	}

	void DX11Shader::Bind() const
	{
	}

	void DX11Shader::Unbind()
	{
	}

	void DX11Shader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
	{
		m_ReloadedCallbacks.push_back(callback);
	}
}

#endif
