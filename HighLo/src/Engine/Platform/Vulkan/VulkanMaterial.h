// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Material.h"
#include "Vulkan.h"
#include "VulkanShader.h"

namespace highlo
{
	class VulkanMaterial : public Material
	{
	public:

		VulkanMaterial(const Ref<Shader> &shader, const HLString &name = "");
		VulkanMaterial(const Ref<Material> &other, const HLString &name = "");
		virtual ~VulkanMaterial();

		virtual void Invalidate() override;

		virtual bool Has(const HLString &name) override
		{
			const ShaderUniform *decl = FindUniformDeclaration(name);
			const ShaderUniform *resource = FindUniformDeclaration(name);

			if (!decl && !resource)
				return false;

			return true;
		}

		// Setters
		template<typename T>
		void Set(const HLString &name, const T &value)
		{
			auto decl = FindUniformDeclaration(name);
			HL_ASSERT(decl);
			if (!decl)
				return;

			auto &buffer = m_LocalData;
			buffer.Write((Byte*)&value, decl->GetSize(), decl->GetOffset());
		}

		void Set(const HLString &name, const Ref<Texture> &texture)
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

		virtual void Set(const HLString &name, float value) override;
		virtual void Set(const HLString &name, int32 value)  override;
		virtual void Set(const HLString &name, uint32 value) override;
		virtual void Set(const HLString &name, bool value)  override;
		virtual void Set(const HLString &name, const glm::vec2 &value) override;
		virtual void Set(const HLString &name, const glm::vec3 &value) override;
		virtual void Set(const HLString &name, const glm::vec4 &value) override;
		virtual void Set(const HLString &name, const glm::ivec2 &value) override;
		virtual void Set(const HLString &name, const glm::ivec3 &value) override;
		virtual void Set(const HLString &name, const glm::ivec4 &value) override;
		virtual void Set(const HLString &name, const glm::mat2 &value) override;
		virtual void Set(const HLString &name, const glm::mat3 &value) override;
		virtual void Set(const HLString &name, const glm::mat4 &value) override;

		virtual void Set(const HLString &name, const Ref<Texture2D> &texture) override;
		virtual void Set(const HLString &name, const Ref<Texture3D> &texture) override;
		virtual void Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot) override;

		// Getters
		template<typename T>
		T &Get(const HLString &name)
		{
			auto decl = FindUniformDeclaration(name);
			HL_ASSERT(decl);

			auto &buffer = m_LocalData;
			return buffer.Read<T>(decl->GetOffset());
		}

		template<typename T>
		Ref<T> GetResource(const HLString &name)
		{
			auto decl = FindResourceDeclaration(name);
			HL_ASSERT(decl);

			uint32 slot = decl->GetRegister();
			HL_ASSERT(slot < m_Textures.size());

			return Ref<T>(m_Textures[slot]);
		}

		template<typename T>
		Ref<T> TryGetResource(const HLString &name)
		{
			auto decl = FindResourceDeclaration(name);
			if (!decl)
				return nullptr;

			uint32 slot = decl->GetRegister();
			if (slot >= m_Textures.size())
				return nullptr;

			return Ref<T>(m_Textures[slot]);
		}

		virtual float &GetFloat(const HLString &name) override;
		virtual int32 &GetInt(const HLString &name) override;
		virtual uint32 &GetUInt(const HLString &name) override;
		virtual bool &GetBool(const HLString &name) override;
		virtual glm::vec2 &GetVector2(const HLString &name) override;
		virtual glm::vec3 &GetVector3(const HLString &name) override;
		virtual glm::vec4 &GetVector4(const HLString &name) override;
		virtual glm::ivec2 &GetIVector2(const HLString &name) override;
		virtual glm::ivec3 &GetIVector3(const HLString &name) override;
		virtual glm::ivec4 &GetIVector4(const HLString &name) override;
		virtual glm::mat2 &GetMatrix2(const HLString &name) override;
		virtual glm::mat3 &GetMatrix3(const HLString &name) override;
		virtual glm::mat4 &GetMatrix4(const HLString &name) override;

		virtual Ref<Texture2D> GetTexture2D(const HLString &name) override;
		virtual Ref<Texture3D> GetTexture3D(const HLString &name) override;

		virtual Ref<Texture2D> TryGetTexture2D(const HLString &name) override;
		virtual Ref<Texture3D> TryGetTexture3D(const HLString &name) override;

		virtual uint32 GetFlags() const override { return m_Flags; }
		virtual bool GetFlag(MaterialFlag flag) const override;
		virtual void SetFlag(MaterialFlag flag, bool value = true) override;

		virtual Ref<Shader> GetShader() const override { return m_Shader; }
		virtual const HLString &GetName() const override { return m_Name; }

		virtual void UpdateForRendering(const Ref<UniformBufferSet> &uniformBufferSet = nullptr) override;

		Allocator GetUniformStorageBuffer() { return m_LocalData; }

		// Vulkan-specific
		void RT_UpdateForRendering(const std::vector<std::vector<VkWriteDescriptorSet>> &uniformBufferWriteDescriptors = std::vector<std::vector<VkWriteDescriptorSet>>());
		void RT_UpdateForRendering(const std::vector<std::vector<VkWriteDescriptorSet>> &uniformBufferWriteDescriptors, const std::vector<std::vector<VkWriteDescriptorSet>> &storageBufferWriteDescriptors);
		void InvalidateDescriptorSets();

		VkDescriptorSet GetDescriptorSet(uint32 index) const { return !m_DescriptorSets[index].DescriptorSets.empty() ? m_DescriptorSets[index].DescriptorSets[0] : nullptr; }

	private:

		void Init();
		void AllocateStorage();
		void OnShaderReloaded();

		void SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture);
		void SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex);
		void SetVulkanDescriptor(const HLString &name, const Ref<Texture3D> &texture);

		const ShaderUniform *FindUniformDeclaration(const HLString &name);
		const ShaderResourceDeclaration *FindResourceDeclaration(const HLString &name);

	private:

		HLString m_Name;
		Ref<Shader> m_Shader;
		uint32 m_Flags = 0;

		Allocator m_LocalData;

		std::vector<Ref<Texture>> m_Textures;
		std::vector<std::vector<Ref<Texture>>> m_TextureArrays;
		std::vector<Ref<Texture2D>> m_Texture2Ds;

		VkShaderMaterialDescriptorSet m_DescriptorSets[3];
		std::vector<std::vector<VkWriteDescriptorSet>> m_WriteDescriptors;
		std::vector<bool> m_DirtyDescriptorSets;

		enum class PendingDescriptorType
		{
			None = 0, Texture2D, TextureCube, Image2D
		};

		struct PendingDescriptor
		{
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WDS;
			VkDescriptorImageInfo ImageInfo;
			Ref<Texture> Texture;
			Ref<Texture2D> Image;
			VkDescriptorImageInfo SubmittedImageInfo{};
		};

		struct PendingDescriptorArray
		{
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WDS;
			std::vector<VkDescriptorImageInfo> ImageInfos;
			std::vector<Ref<Texture>> Textures;
			std::vector<Ref<Texture2D>> Images;
			VkDescriptorImageInfo SubmittedImageInfo{};
		};

		std::unordered_map<uint32, std::shared_ptr<PendingDescriptor>> m_ResidentDescriptors;
		std::unordered_map<uint32, std::shared_ptr<PendingDescriptorArray>> m_ResidentDescriptorArrays;
		std::vector<std::shared_ptr<PendingDescriptor>> m_PendingDescriptors;
	};
}

#endif // HIGHLO_API_VULKAN

