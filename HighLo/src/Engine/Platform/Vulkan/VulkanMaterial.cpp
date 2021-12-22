#include "HighLoPch.h"
#include "VulkanMaterial.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanTexture2D.h"
#include "VulkanTexture3D.h"

namespace highlo
{
	VulkanMaterial::VulkanMaterial(const Ref<Shader> &shader, const HLString &name)
		: m_Shader(shader), m_Name(name), m_WriteDescriptors(Renderer::GetConfig().FramesInFlight), m_DirtyDescriptorSets(Renderer::GetConfig().FramesInFlight, true)
	{
		Init();

		// TODO
		//Renderer::RegisterShaderDependency(shader, this);
	}
	
	VulkanMaterial::VulkanMaterial(const Ref<Material> &other, const HLString &name)
		: m_Shader(other->GetShader()), m_Name(name), m_WriteDescriptors(Renderer::GetConfig().FramesInFlight), m_DirtyDescriptorSets(Renderer::GetConfig().FramesInFlight, true)
	{
		if (name.IsEmpty())
			m_Name = other->GetName();

		// TODO
		//Renderer::RegisterShaderDependency(shader, this);

		auto vulkanMaterial = other.As<VulkanMaterial>();
		m_UniformStorageBuffer = Allocator::Copy(vulkanMaterial->m_UniformStorageBuffer.m_Data, vulkanMaterial->m_UniformStorageBuffer.m_Size);

		m_ResidentDescriptors = vulkanMaterial->m_ResidentDescriptors;
		m_ResidentDescriptorArrays = vulkanMaterial->m_ResidentDescriptorArrays;
		m_PendingDescriptors = vulkanMaterial->m_PendingDescriptors;
		m_Textures = vulkanMaterial->m_Textures;
		m_TextureArrays = vulkanMaterial->m_TextureArrays;
		m_TextureHashes = vulkanMaterial->m_TextureHashes;
	}

	VulkanMaterial::~VulkanMaterial()
	{
	}
	
	void VulkanMaterial::Invalidate()
	{
		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		auto shader = m_Shader.As<VulkanShader>();
		if (shader->HasDescriptorSet(0))
		{
			const auto &shaderDescriptorSets = shader->GetShaderDescriptorSets();
			if (!shaderDescriptorSets.empty())
			{
				for (auto &&[binding, descriptor] : m_ResidentDescriptors)
					m_PendingDescriptors.push_back(descriptor);
			}
		}
	}
	
	void VulkanMaterial::Set(const HLString &name, float value)
	{
		Set<float>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, int32 value)
	{
		Set<int32>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, uint32 value)
	{
		Set<uint32>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, bool value)
	{
		// 4-byte ints = bool
		Set<int32>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::vec2 &value)
	{
		Set<glm::vec2>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::vec3 &value)
	{
		Set<glm::vec3>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::vec4 &value)
	{
		Set<glm::vec4>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::ivec2 &value)
	{
		Set<glm::ivec2>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::ivec3 &value)
	{
		Set<glm::ivec3>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::ivec4 &value)
	{
		Set<glm::ivec4>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::mat2 &value)
	{
		Set<glm::mat2>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::mat3 &value)
	{
		Set<glm::mat3>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const glm::mat4 &value)
	{
		Set<glm::mat4>(name, value);
	}
	
	void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture)
	{
		SetVulkanDescriptor(name, texture);
	}
	
	void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex)
	{
		SetVulkanDescriptor(name, texture, arrayIndex);
	}

	void VulkanMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
	{
		SetVulkanDescriptor(name, texture);
	}

	float &VulkanMaterial::GetFloat(const HLString &name)
	{
		return Get<float>(name);
	}
	
	int32 &VulkanMaterial::GetInt(const HLString &name)
	{
		return Get<int32>(name);
	}
	
	uint32 &VulkanMaterial::GetUInt(const HLString &name)
	{
		return Get<uint32>(name);
	}

	bool &VulkanMaterial::GetBool(const HLString &name)
	{
		return Get<bool>(name);
	}
	
	glm::vec2 &VulkanMaterial::GetVector2(const HLString &name)
	{
		return Get<glm::vec2>(name);
	}
	
	glm::vec3 &VulkanMaterial::GetVector3(const HLString &name)
	{
		return Get<glm::vec3>(name);
	}

	glm::vec4 &VulkanMaterial::GetVector4(const HLString &name)
	{
		return Get<glm::vec4>(name);
	}

	glm::ivec2 &VulkanMaterial::GetIVector2(const HLString &name)
	{
		return Get<glm::ivec2>(name);
	}

	glm::ivec3 &VulkanMaterial::GetIVector3(const HLString &name)
	{
		return Get<glm::ivec3>(name);
	}

	glm::ivec4 &VulkanMaterial::GetIVector4(const HLString &name)
	{
		return Get<glm::ivec4>(name);
	}

	glm::mat2 &VulkanMaterial::GetMatrix2(const HLString &name)
	{
		return Get<glm::mat2>(name);
	}

	glm::mat3 &VulkanMaterial::GetMatrix3(const HLString &name)
	{
		return Get<glm::mat3>(name);
	}

	glm::mat4 &VulkanMaterial::GetMatrix4(const HLString &name)
	{
		return Get<glm::mat4>(name);
	}

	Ref<Texture2D> VulkanMaterial::GetTexture2D(const HLString &name)
	{
		return GetResource<Texture2D>(name);
	}

	Ref<Texture3D> VulkanMaterial::GetTexture3D(const HLString &name)
	{
		return GetResource<Texture3D>(name);
	}

	Ref<Texture2D> VulkanMaterial::TryGetTexture2D(const HLString &name)
	{
		return TryGetResource<Texture2D>(name);
	}

	Ref<Texture3D> VulkanMaterial::TryGetTexture3D(const HLString &name)
	{
		return TryGetResource<Texture3D>(name);
	}

	bool VulkanMaterial::GetFlag(MaterialFlag flag) const
	{
		return (uint32)flag & m_Flags;
	}

	void VulkanMaterial::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_Flags |= (uint32)flag;
		}
		else
		{
			m_Flags &= ~(uint32)flag;
		}
	}

	void VulkanMaterial::InvalidateDescriptorSets()
	{
		const uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		for (uint32 i = 0; i < framesInFlight; i++)
			m_DirtyDescriptorSets[i] = true;
	}

	void VulkanMaterial::UpdateForRendering(const std::vector<std::vector<VkWriteDescriptorSet>> &uniformBufferWriteDescriptors)
	{
		auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		for (auto &&[binding, descriptor] : m_ResidentDescriptors)
		{
			if (descriptor->Type == PendingDescriptorType::Texture2D)
			{
				Ref<VulkanTexture2D> image = descriptor->TheTexture.As<VulkanTexture2D>();
				HL_ASSERT(image->GetTextureInfo().ImageView, "ImageView is null");

				if (descriptor->WriteDescriptorSet.pImageInfo && image->GetTextureInfo().ImageView != descriptor->WriteDescriptorSet.pImageInfo->imageView)
				{
					m_PendingDescriptors.emplace_back(descriptor);
					InvalidateDescriptorSets();
				}
			}
		}

		std::vector<VkDescriptorImageInfo> arrayImageInfos;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		if (m_DirtyDescriptorSets[frameIndex] || true)
		{
			m_DirtyDescriptorSets[frameIndex] = false;
			m_WriteDescriptors[frameIndex].clear();

			if (!uniformBufferWriteDescriptors.empty())
			{
				for (auto &wd : uniformBufferWriteDescriptors[frameIndex])
					m_WriteDescriptors[frameIndex].push_back(wd);
			}

			for (auto &&[binding, pd] : m_ResidentDescriptors)
			{
				if (pd->Type == PendingDescriptorType::Texture2D)
				{
					Ref<VulkanTexture2D> texture = pd->TheTexture.As<VulkanTexture2D>();
					pd->TextureInfo = texture->GetVulkanDescriptorInfo();
					pd->WriteDescriptorSet.pImageInfo = &pd->TextureInfo;
				}
				else if (pd->Type == PendingDescriptorType::Texture3D)
				{
					Ref<VulkanTexture3D> texture = pd->TheTexture.As<VulkanTexture3D>();
					pd->TextureInfo = texture->GetVulkanDescriptorInfo();
					pd->WriteDescriptorSet.pImageInfo = &pd->TextureInfo;
				}
				else if (pd->Type == PendingDescriptorType::Texture2D)
				{
					Ref<VulkanTexture2D> image = pd->TheTexture.As<VulkanTexture2D>();
					pd->TextureInfo = image->GetVulkanDescriptorInfo();
					pd->WriteDescriptorSet.pImageInfo = &pd->TextureInfo;
				}

				m_WriteDescriptors[frameIndex].push_back(pd->WriteDescriptorSet);
			}

			for (auto &&[binding, pd] : m_ResidentDescriptorArrays)
			{
				if (pd->Type == PendingDescriptorType::Texture2D)
				{
					for (auto tex : pd->Textures)
					{
						Ref<VulkanTexture2D> texture = tex.As<VulkanTexture2D>();
						arrayImageInfos.emplace_back(texture->GetVulkanDescriptorInfo());
					}
				}
				pd->WriteDescriptorSet.pImageInfo = arrayImageInfos.data();
				pd->WriteDescriptorSet.descriptorCount = (uint32)arrayImageInfos.size();
				m_WriteDescriptors[frameIndex].push_back(pd->WriteDescriptorSet);
			}

		}

		auto vulkanShader = m_Shader.As<VulkanShader>();
		auto descriptorSet = vulkanShader->AllocateDescriptorSet();
		m_DescriptorSets[frameIndex] = descriptorSet;
		for (auto &writeDescriptor : m_WriteDescriptors[frameIndex])
			writeDescriptor.dstSet = descriptorSet.DescriptorSets[0];

		vkUpdateDescriptorSets(device, (uint32)m_WriteDescriptors[frameIndex].size(), m_WriteDescriptors[frameIndex].data(), 0, nullptr);
		m_PendingDescriptors.clear();

	}

	VkDescriptorSet VulkanMaterial::GetDescriptorSet(uint32 index) const
	{
		return !m_DescriptorSets[index].DescriptorSets.empty() ? m_DescriptorSets[index].DescriptorSets[0] : nullptr;
	}
	
	void VulkanMaterial::Init()
	{
		AllocateStorage();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;

		Invalidate();
	}
	
	void VulkanMaterial::AllocateStorage()
	{
		const auto &shaderBuffers = m_Shader->GetShaderBuffers();

		if (shaderBuffers.size() > 0)
		{
			uint32 size = 0;
			for (auto [name, shaderBuffer] : shaderBuffers)
				size += shaderBuffer.Size;

			m_UniformStorageBuffer.Allocate(size);
			m_UniformStorageBuffer.ZeroInitialize();
		}
	}

	void VulkanMaterial::OnShaderReloaded()
	{
		// TODO
	}
	
	const ShaderUniform *VulkanMaterial::FindUniformDeclaration(const HLString &name)
	{
		const auto &shaderBuffers = m_Shader->GetShaderBuffers();
		if (shaderBuffers.size() > 0)
		{
			const ShaderBuffer &buffer = (*shaderBuffers.begin()).second;
			if (buffer.Uniforms.find(name) == buffer.Uniforms.end())
				return nullptr;

			return &buffer.Uniforms.at(name);
		}

		return nullptr;
	}
	
	const ShaderResourceDeclaration *VulkanMaterial::FindResourceDeclaration(const HLString &name)
	{
		auto &resources = m_Shader->GetResources();
		for (const auto &[n, resource] : resources)
		{
			if (resource.GetName() == name)
				return &resource;
		}

		return nullptr;
	}
	
	void VulkanMaterial::SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture)
	{
		const ShaderResourceDeclaration *resource = FindResourceDeclaration(name);
		HL_ASSERT(resource);

		uint32 binding = resource->GetRegister();
		// Texture is already set
		if (binding < m_Textures.size() && m_Textures[binding] && texture->GetHash() == m_Textures[binding]->GetHash())
			return;

		if (binding >= m_Textures.size())
			m_Textures.resize(binding + 1);
		m_Textures[binding] = texture;

		const VkWriteDescriptorSet *wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
		HL_ASSERT(wds);

		m_ResidentDescriptors[binding] = PendingDescriptor::Create(PendingDescriptorType::Texture2D, *wds, {}, {}, texture.As<Texture>());
		m_PendingDescriptors.push_back(m_ResidentDescriptors.at(binding));

		InvalidateDescriptorSets();
	}

	void VulkanMaterial::SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex)
	{
		const ShaderResourceDeclaration *resource = FindResourceDeclaration(name);
		HL_ASSERT(resource);

		uint32 binding = resource->GetRegister();
		// Texture is already set
		if (binding < m_TextureArrays.size() && m_TextureArrays[binding].size() < arrayIndex && texture->GetHash() == m_TextureArrays[binding][arrayIndex]->GetHash())
			return;

		if (binding >= m_TextureArrays.size())
			m_TextureArrays.resize(binding + 1);

		if (arrayIndex >= m_TextureArrays[binding].size())
			m_TextureArrays[binding].resize(arrayIndex + 1);

		m_TextureArrays[binding][arrayIndex] = texture;

		const VkWriteDescriptorSet *wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
		HL_ASSERT(wds);
		
		if (m_ResidentDescriptorArrays.find(binding) == m_ResidentDescriptorArrays.end())
		{
			m_ResidentDescriptorArrays[binding] = PendingDescriptorArray::Create(PendingDescriptorType::Texture3D, *wds, {}, {}, {});
		}

		auto &residentDesriptorArray = m_ResidentDescriptorArrays.at(binding);
		if (arrayIndex >= residentDesriptorArray->Textures.size())
			residentDesriptorArray->Textures.resize(arrayIndex + 1);

		residentDesriptorArray->Textures[arrayIndex] = texture;

	//	m_PendingDescriptors.push_back(m_ResidentDescriptors.at(binding));
		InvalidateDescriptorSets();
	}

	void VulkanMaterial::SetVulkanDescriptor(const HLString &name, const Ref<Texture3D> &texture)
	{
		const ShaderResourceDeclaration *resource = FindResourceDeclaration(name);
		HL_ASSERT(resource);

		uint32 binding = resource->GetRegister();
		// Texture is already set
		if (binding < m_Textures.size() && m_Textures[binding] && texture->GetHash() == m_Textures[binding]->GetHash())
			return;

		if (binding >= m_Textures.size())
			m_Textures.resize(binding + 1);
		m_Textures[binding] = texture;

		const VkWriteDescriptorSet *wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
		HL_ASSERT(wds);
		
		m_ResidentDescriptors[binding] = PendingDescriptor::Create(PendingDescriptorType::Texture3D, *wds, {}, {}, texture.As<Texture>());
		m_PendingDescriptors.push_back(m_ResidentDescriptors.at(binding));

		InvalidateDescriptorSets();
	}
}

#endif // HIGHLO_API_VULKAN

