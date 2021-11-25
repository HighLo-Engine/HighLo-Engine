// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderLibrary.h"

namespace highlo
{
	ShaderLibrary::ShaderLibrary() {}
	ShaderLibrary::~ShaderLibrary() {}
	
	void ShaderLibrary::Add(const Ref<Shader> &shader)
	{
		auto &name = shader->GetName();
		HL_ASSERT(m_Shaders.find(name) == m_Shaders.end());
		m_Shaders[name] = shader;
	}
	
	void ShaderLibrary::Add(const HLString &name, const Ref<Shader> &shader)
	{
		HL_ASSERT(m_Shaders.find(name) == m_Shaders.end());
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Load(const FileSystemPath &filePath, bool forceCompile)
	{		
		Ref<Shader> shader = Shader::Create(filePath, forceCompile);
		if (!shader)
		{
			HL_CORE_ERROR("Failed to load shader into shaderlibrary!");
			return;
		}

		Add(shader);
	}
	
	void ShaderLibrary::Load(const HLString &name, const FileSystemPath &filePath, bool forceCompile)
	{
		Ref<Shader> shader = Shader::Create(filePath, forceCompile);
		if (!shader)
		{
			HL_CORE_ERROR("Failed to load shader into shaderlibrary!");
			return;
		}

		Add(name, shader);
	}
	
	const Ref<Shader> &ShaderLibrary::Get(const HLString &name)
	{
		HL_ASSERT(m_Shaders.find(*name) != m_Shaders.end());
		return m_Shaders[*name];
	}
}

