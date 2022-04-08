// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Shader.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	static std::unordered_map<uint32, ShaderUniformBuffer> s_UniformBuffers;
	static std::unordered_map<uint32, ShaderStorageBuffer> s_StorageBuffers;

	DX12Shader::DX12Shader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
	}

	DX12Shader::DX12Shader(const HLString &source)
	{
	}
	
	DX12Shader::~DX12Shader()
	{
		Release();
	}

	void DX12Shader::Reload(bool forceCompile)
	{
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
}

#endif // HIGHLO_API_DX12

