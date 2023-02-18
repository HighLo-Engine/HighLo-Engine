// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Shader.h"

#ifdef HIGHLO_API_DX12

#define DX12_SHADER_LOG_PREFIX "Shader>       "

#include "Engine/Core/FileSystem.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Utils/ShaderUtils.h"

namespace highlo
{
	static std::unordered_map<uint32, ShaderUniformBuffer> s_UniformBuffers;
	static std::unordered_map<uint32, ShaderStorageBuffer> s_StorageBuffers;

	DX12Shader::DX12Shader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
		m_Language = utils::ShaderLanguageFromExtension(filePath.Extension());
		HLString source = FileSystem::Get()->ReadTextFile(filePath);
		Load(source, forceCompile);
	}

	DX12Shader::DX12Shader(const HLString &source, const HLString &name, ShaderLanguage language)
	{
		m_Name = name;
		m_Language = language;
		Load(source, true);
	}
	
	DX12Shader::~DX12Shader()
	{
		Release();
	}

	void DX12Shader::Reload(bool forceCompile)
	{
		HL_CORE_INFO(DX12_SHADER_LOG_PREFIX "[+] Reloading shader {0}... [+]", **m_AssetPath);
		m_Loaded = false; // Reflect current stage: Shader is being reloaded

		HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
		Load(source, forceCompile);

		Renderer::OnShaderReloaded(GetHash());

		for (ShaderReloadedCallback callback : m_ReloadedCallbacks)
			callback();
	}

	void DX12Shader::Release()
	{
	}

	uint64 DX12Shader::GetHash() const
	{
		return m_AssetPath.Hash();
	}

	void DX12Shader::Bind() const
	{
	}

	void DX12Shader::Unbind()
	{
	}

	void DX12Shader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
	{
		m_ReloadedCallbacks.push_back(callback);
	}
	
	void DX12Shader::SetMacro(const HLString &name, const HLString &value)
	{
		m_Macros[name] = value;
	}

	const ShaderResourceDeclaration *DX12Shader::GetResource(const HLString &name) const
	{
		if (m_Resources.find(name) == m_Resources.end())
		{
			return nullptr;
		}

		return &m_Resources.at(name);
	}
	
	void DX12Shader::Load(const HLString &source, bool forceCompile)
	{
	}
}

#endif // HIGHLO_API_DX12

