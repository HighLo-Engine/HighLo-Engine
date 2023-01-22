// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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

	void ShaderLibrary::LoadFromString(const HLString &source, ShaderLanguage language)
	{
		Ref<Shader> shader = Shader::CreateFromString(source, "undefined", language);
		if (!shader)
		{
			HL_CORE_ERROR("Failed to load shader into shaderlibrary!");
			return;
		}

		Add(shader);
	}

	void ShaderLibrary::LoadFromString(const HLString &name, const HLString &source, ShaderLanguage language)
	{
		Ref<Shader> shader = Shader::CreateFromString(source, name, language);
		if (!shader)
		{
			HL_CORE_ERROR("Failed to load shader into shaderlibrary!");
			return;
		}

		Add(name, shader);
	}

	void ShaderLibrary::ReloadShader(const HLString &name)
	{
		HL_ASSERT(m_Shaders.find(name) != m_Shaders.end());
		m_Shaders[name]->Reload();
	}

	void ShaderLibrary::ReloadAllShaders()
	{
		for (auto &[name, shader] : m_Shaders)
		{
			shader->Reload();
		}
	}

	void ShaderLibrary::Remove(const HLString &name)
	{
		HL_ASSERT(m_Shaders.find(name) != m_Shaders.end());
		m_Shaders[name]->Release();
		m_Shaders.erase(name);
	}
	
	const Ref<Shader> &ShaderLibrary::Get(const HLString &name)
	{
		HL_ASSERT(m_Shaders.find(name) != m_Shaders.end());
		return m_Shaders[name];
	}
}

