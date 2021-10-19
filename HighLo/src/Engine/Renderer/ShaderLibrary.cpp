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
		HL_ASSERT(m_Shaders.find(*name) == m_Shaders.end());
		m_Shaders[*name] = shader;
	}
	
	void ShaderLibrary::Load(const HLString &path, BufferLayout &layout, bool isCompute)
	{
		auto shaderSource = Shader::LoadShaderSource(path);
		Ref<Shader> shader = nullptr;

		if (isCompute)
			shader = Shader::CreateComputeShader(shaderSource);
		else
			shader = Shader::Create(shaderSource, layout);

		Add(shader);
	}
	
	void ShaderLibrary::Load(const HLString &name, const HLString &path, BufferLayout &layout, bool isCompute)
	{
		auto shaderSource = Shader::LoadShaderSource(path);
		Ref<Shader> shader = nullptr;

		if (isCompute)
			shader = Shader::CreateComputeShader(shaderSource);
		else
			shader = Shader::Create(shaderSource, layout);

		HL_ASSERT(m_Shaders.find(*name) == m_Shaders.end());
		m_Shaders[*name] = shader;
	}
	
	const Ref<Shader> &ShaderLibrary::Get(const HLString &name)
	{
		HL_ASSERT(m_Shaders.find(*name) != m_Shaders.end());
		return m_Shaders[*name];
	}
}
