// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Material.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12Material::DX12Material(const Ref<Shader> &shader, const HLString &name)
		: m_Shader(shader), m_Name(name)
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&DX12Material::OnShaderReloaded, this));
		AllocateStorage();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;
	}

	DX12Material::DX12Material(const Ref<Material> &other, const HLString &name)
		: m_Name(name), m_Shader(other->GetShader())
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&DX12Material::OnShaderReloaded, this));
		if (name.IsEmpty())
			m_Name = other->GetShader()->GetName();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;
	}

	DX12Material::~DX12Material()
	{
	}

	void DX12Material::Invalidate()
	{
	}
	
	bool DX12Material::Has(const HLString &name)
	{
		const ShaderUniform *decl = FindUniformDeclaration(name);
		const ShaderUniform *resource = FindUniformDeclaration(name);

		if (!decl && !resource)
			return false;

		return true;
	}

	void DX12Material::Set(const HLString &name, float value)
	{
		Set<float>(name, value);
	}

	void DX12Material::Set(const HLString &name, int32 value)
	{
		Set<int32>(name, value);
	}

	void DX12Material::Set(const HLString &name, uint32 value)
	{
		Set<uint32>(name, value);
	}

	void DX12Material::Set(const HLString &name, bool value)
	{
		Set<bool>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::vec2 &value)
	{
		Set<glm::vec2>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::vec3 &value)
	{
		Set<glm::vec3>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::vec4 &value)
	{
		Set<glm::vec4>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::ivec2 &value)
	{
		Set<glm::ivec2>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::ivec3 &value)
	{
		Set<glm::ivec3>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::ivec4 &value)
	{
		Set<glm::ivec4>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::mat2 &value)
	{
		Set<glm::mat2>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::mat3 &value)
	{
		Set<glm::mat3>(name, value);
	}

	void DX12Material::Set(const HLString &name, const glm::mat4 &value)
	{
		Set<glm::mat4>(name, value);
	}

	void DX12Material::Set(const HLString &name, const Ref<Texture2D> &texture)
	{
		auto decl = FindResourceDeclaration(name);
		if (!decl)
		{
			HL_CORE_WARN("Cannot find material property: {0}", *name);
			return;
		}

		uint32 slot = decl->GetRegister();
		m_Texture2Ds[slot] = texture;
	}

	void DX12Material::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot)
	{
		auto decl = FindResourceDeclaration(name);
		if (!decl)
		{
			HL_CORE_WARN("Cannot find material property: {0}", *name);
			return;
		}

		m_Texture2Ds[slot] = texture;
	}

	void DX12Material::Set(const HLString &name, const Ref<Texture3D> &texture)
	{
		auto decl = FindResourceDeclaration(name);
		if (!decl)
		{
			HL_CORE_WARN("Cannot find material property: {0}", *name);
			return;
		}

		uint32 slot = decl->GetRegister();
		if (m_Textures.size() <= slot)
			m_Textures.resize((size_t)slot + 1);

		m_Textures[slot] = texture;
	}

	float &DX12Material::GetFloat(const HLString &name)
	{
		return Get<float>(name);
	}

	int32 &DX12Material::GetInt(const HLString &name)
	{
		return Get<int32>(name);
	}

	uint32 &DX12Material::GetUInt(const HLString &name)
	{
		return Get<uint32>(name);
	}

	bool &DX12Material::GetBool(const HLString &name)
	{
		return Get<bool>(name);
	}

	glm::vec2 &DX12Material::GetVector2(const HLString &name)
	{
		return Get<glm::vec2>(name);
	}

	glm::vec3 &DX12Material::GetVector3(const HLString &name)
	{
		return Get<glm::vec3>(name);
	}

	glm::vec4 &DX12Material::GetVector4(const HLString &name)
	{
		return Get<glm::vec4>(name);
	}

	glm::ivec2 &DX12Material::GetIVector2(const HLString &name)
	{
		return Get<glm::ivec2>(name);
	}

	glm::ivec3 &DX12Material::GetIVector3(const HLString &name)
	{
		return Get<glm::ivec3>(name);
	}

	glm::ivec4 &DX12Material::GetIVector4(const HLString &name)
	{
		return Get<glm::ivec4>(name);
	}

	glm::mat2 &DX12Material::GetMatrix2(const HLString &name)
	{
		return Get<glm::mat2>(name);
	}

	glm::mat3 &DX12Material::GetMatrix3(const HLString &name)
	{
		return Get<glm::mat3>(name);
	}

	glm::mat4 &DX12Material::GetMatrix4(const HLString &name)
	{
		return Get<glm::mat4>(name);
	}

	Ref<Texture2D> DX12Material::GetTexture2D(const HLString &name)
	{
		return GetResource<Texture2D>(name);
	}

	Ref<Texture3D> DX12Material::GetTexture3D(const HLString &name)
	{
		return GetResource<Texture3D>(name);
	}

	Ref<Texture2D> DX12Material::TryGetTexture2D(const HLString &name)
	{
		return TryGetResource<Texture2D>(name);
	}

	Ref<Texture3D> DX12Material::TryGetTexture3D(const HLString &name)
	{
		return TryGetResource<Texture3D>(name);
	}

	bool DX12Material::GetFlag(MaterialFlag flag) const
	{
		return (uint32)flag & m_Flags;
	}

	void DX12Material::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
			m_Flags |= (uint32)flag;
		else
			m_Flags &= ~(uint32)flag;
	}

	void DX12Material::UpdateForRendering(const Ref<UniformBufferSet> &uniformBufferSet)
	{
		m_Shader->Bind();

		if (uniformBufferSet)
		{
			uniformBufferSet->ForEach([](const Ref<UniformBuffer> &uniformBuffer)
			{
				uniformBuffer->Bind();
			});
		}
		else
		{
			HL_CORE_WARN("Missing uniform buffer set! Please check your UpdateForRendering() call!");
		}


	}

	void DX12Material::AllocateStorage()
	{
		const auto &shaderBuffers = m_Shader->GetShaderBuffers();
		if (shaderBuffers.size() > 0)
		{
			uint32 size = 0;
			for (auto &[name, buffer] : shaderBuffers)
				size += buffer.Size;

			m_LocalData.Allocate(size);
			m_LocalData.ZeroInitialize();
		}
	}

	void DX12Material::OnShaderReloaded()
	{
	}

	const ShaderUniform *DX12Material::FindUniformDeclaration(const HLString &name)
	{
		const auto &shaderBuffers = m_Shader->GetShaderBuffers();

		if (shaderBuffers.size() > 0)
		{
			for (const auto &[n, buffer] : shaderBuffers)
			{
				if (buffer.Uniforms.find(name) != buffer.Uniforms.end())
				{
					return &buffer.Uniforms.at(name);
				}
			}
		}

		return nullptr;
	}

	const ShaderResourceDeclaration *DX12Material::FindResourceDeclaration(const HLString &name)
	{
		auto &resources = m_Shader->GetResources();
		for (const auto &[n, resource] : resources)
		{
			if (resource.GetName() == name)
				return &resource;
		}


		return nullptr;
	}
}

#endif // HIGHLO_API_DX12

