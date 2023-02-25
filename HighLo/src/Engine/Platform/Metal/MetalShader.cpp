// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalShader.h"

#ifdef HIGHLO_API_METAL

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/ShaderUtils.h"

namespace highlo
{
	MetalShader::MetalShader(const FileSystemPath &filePath, bool forceCompile)
	{
		m_Name = filePath.Filename();
		m_Language = utils::ShaderLanguageFromExtension(filePath.Extension());
		HLString source = FileSystem::Get()->ReadTextFile(filePath);
		Load(source, forceCompile);
	}

	MetalShader::MetalShader(const HLString &source, const HLString &name, ShaderLanguage language)
	{
		m_Name = name;
		m_Language = language;
		Load(source, true);
	}
	
	MetalShader::~MetalShader()
	{
		Release();
	}
	
	void MetalShader::Reload(bool forceCompile)
	{
	}
	
	void MetalShader::Release()
	{
	}
	
	void MetalShader::Bind() const
	{
	}
	
	void MetalShader::Unbind()
	{
	}
	
	void MetalShader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
	{
		m_ReloadedCallbacks.push_back(callback);
	}

	const ShaderResourceDeclaration *MetalShader::GetResource(const HLString &name) const
	{
		if (m_Resources.find(name) == m_Resources.end())
		{
			return nullptr;
		}

		return &m_Resources.at(name);
	}
	
	void MetalShader::SetMacro(const HLString &name, const HLString &value)
	{
		m_Macros[name] = value;
	}
	
	void MetalShader::Load(const HLString &source, bool forceCompile)
	{
	}
}

#endif // HIGHLO_API_METAL

