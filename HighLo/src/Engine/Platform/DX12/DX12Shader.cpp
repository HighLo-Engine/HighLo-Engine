// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Shader.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12Shader::DX12Shader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
	}

	DX12Shader::DX12Shader(const HLString &source)
	{
	}
	
	DX12Shader::~DX12Shader()
	{
	}

	void DX12Shader::Reload(bool forceCompile)
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
}

#endif // HIGHLO_API_DX12

