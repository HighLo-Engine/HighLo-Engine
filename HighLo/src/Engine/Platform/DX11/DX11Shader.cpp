// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11Shader.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/ShaderUtils.h"

/// @note @FlareFlax:
/// for DirectX we probably need HLSL, but our shaders are only written in glsl. (no problem with SPIR-V ;))
/// SPIR-V can cross-compile also to hlsl, so we need to use it here also
/// @see: https://vulkan.lunarg.com/doc/view/1.2.154.1/windows/spirv_toolchain.html

#ifdef HIGHLO_API_DX11

namespace highlo
{
	static std::unordered_map<uint32, ShaderUniformBuffer> s_UniformBuffers;
	static std::unordered_map<uint32, ShaderStorageBuffer> s_StorageBuffers;

	DX11Shader::DX11Shader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
		m_Language = utils::ShaderLanguageFromExtension(filePath.Extension());
		HLString source = FileSystem::Get()->ReadTextFile(filePath);
	//	Load(source, forceCompile);
	}

	DX11Shader::DX11Shader(const HLString &source, const HLString &name, ShaderLanguage language)
	{
		m_Name = name;
		m_Language = language;
	//	Load(source, true);
	}

	DX11Shader::~DX11Shader()
	{
		Release();
	}

	void DX11Shader::Reload(bool forceCompile)
	{
	}

	void DX11Shader::Release()
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
	
	void DX11Shader::SetMacro(const HLString &name, const HLString &value)
	{
		m_Macros[name] = value;
	}
	
	const ShaderResourceDeclaration *DX11Shader::GetResource(const HLString &name) const
	{
		if (m_Resources.find(name) == m_Resources.end())
		{
			return nullptr;
		}

		return &m_Resources.at(name);
	}
}

#endif
