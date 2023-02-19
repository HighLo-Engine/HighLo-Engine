// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalMaterial.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalMaterial::MetalMaterial(const Ref<Shader> &shader, const HLString &name)
		: m_Shader(shader), m_Name(name)
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&MetalMaterial::OnShaderReloaded, this));
		AllocateStorage();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;
	}

	MetalMaterial::MetalMaterial(const Ref<Material> &other, const HLString &name)
		: m_Shader(other->GetShader()), m_Name(name)
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&MetalMaterial::OnShaderReloaded, this));
		if (name.IsEmpty())
			m_Name = other->GetShader()->GetName();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;
	}

	MetalMaterial::~MetalMaterial()
	{
		m_LocalData.Release();
	}
	
	void MetalMaterial::Invalidate()
	{
	}
	
	void MetalMaterial::Set(const HLString &name, float value)
	{
		Set<float>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, int32 value)
	{
		Set<int32>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, uint32 value)
	{
		Set<uint32>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, bool value)
	{
		Set<bool>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::vec2 &value)
	{
		Set<glm::vec2>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::vec3 &value)
	{
		Set<glm::vec3>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::vec4 &value)
	{
		Set<glm::vec4>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::ivec2 &value)
	{
		Set<glm::ivec2>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::ivec3 &value)
	{
		Set<glm::ivec3>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::ivec4 &value)
	{
		Set<glm::ivec4>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::mat2 &value)
	{
		Set<glm::mat2>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::mat3 &value)
	{
		Set<glm::mat3>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const glm::mat4 &value)
	{
		Set<glm::mat4>(name, value);
	}
	
	void MetalMaterial::Set(const HLString &name, const Ref<Texture2D> &texture)
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

	void MetalMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot)
	{
		auto decl = FindResourceDeclaration(name);
		if (!decl)
		{
			HL_CORE_WARN("Cannot find material property: {0}", *name);
			return;
		}

		m_Texture2Ds[slot] = texture;
	}

	void MetalMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
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

	float &MetalMaterial::GetFloat(const HLString &name)
	{
		return Get<float>(name);
	}
	
	int32 &MetalMaterial::GetInt(const HLString &name)
	{
		return Get<int32>(name);
	}
	
	uint32 &MetalMaterial::GetUInt(const HLString &name)
	{
		return Get<uint32>(name);
	}
	
	bool &MetalMaterial::GetBool(const HLString &name)
	{
		return Get<bool>(name);
	}
	
	glm::vec2 &MetalMaterial::GetVector2(const HLString &name)
	{
		return Get<glm::vec2>(name);
	}
	
	glm::vec3 &MetalMaterial::GetVector3(const HLString &name)
	{
		return Get<glm::vec3>(name);
	}
	
	glm::vec4 &MetalMaterial::GetVector4(const HLString &name)
	{
		return Get<glm::vec4>(name);
	}
	
	glm::ivec2 &MetalMaterial::GetIVector2(const HLString &name)
	{
		return Get<glm::ivec2>(name);
	}
	
	glm::ivec3 &MetalMaterial::GetIVector3(const HLString &name)
	{
		return Get<glm::ivec3>(name);
	}
	
	glm::ivec4 &MetalMaterial::GetIVector4(const HLString &name)
	{
		return Get<glm::ivec4>(name);
	}
	
	glm::mat2 &MetalMaterial::GetMatrix2(const HLString &name)
	{
		return Get<glm::mat2>(name);
	}
	
	glm::mat3 &MetalMaterial::GetMatrix3(const HLString &name)
	{
		return Get<glm::mat3>(name);
	}
	
	glm::mat4 &MetalMaterial::GetMatrix4(const HLString &name)
	{
		return Get<glm::mat4>(name);
	}
	
	Ref<Texture2D> MetalMaterial::GetTexture2D(const HLString &name)
	{
		return GetResource<Texture2D>(name);
	}
	
	Ref<Texture3D> MetalMaterial::GetTexture3D(const HLString &name)
	{
		return GetResource<Texture3D>(name);
	}
	
	Ref<Texture2D> MetalMaterial::TryGetTexture2D(const HLString &name)
	{
		return TryGetResource<Texture2D>(name);
	}
	
	Ref<Texture3D> MetalMaterial::TryGetTexture3D(const HLString &name)
	{
		return TryGetResource<Texture3D>(name);
	}
	
	bool MetalMaterial::GetFlag(MaterialFlag flag) const
	{
		return (uint32)flag & m_Flags;
	}
	
	void MetalMaterial::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
			m_Flags |= (uint32)flag;
		else
			m_Flags &= ~(uint32)flag;
	}
	
	void MetalMaterial::UpdateForRendering(const Ref<UniformBufferSet> &uniformBufferSet)
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
	
	void MetalMaterial::AllocateStorage()
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
	
	void MetalMaterial::OnShaderReloaded()
	{
	}
	
	const ShaderUniform *MetalMaterial::FindUniformDeclaration(const HLString &name)
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
	
	const ShaderResourceDeclaration *MetalMaterial::FindResourceDeclaration(const HLString &name)
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

#endif // HIGHLO_API_METAL

