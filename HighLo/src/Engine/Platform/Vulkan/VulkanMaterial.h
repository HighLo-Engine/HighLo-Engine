// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/Material.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "VulkanShaderDefs.h"

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

		// Setters
		virtual void Set(const HLString &name, float value) override;
		virtual void Set(const HLString &name, int32 value) override;
		virtual void Set(const HLString &name, uint32 value) override;
		virtual void Set(const HLString &name, bool value) override;
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
		virtual void Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot) override;
		virtual void Set(const HLString &name, const Ref<Texture3D> &texture) override;

		// Getters
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

		virtual void UpdateForRendering(const Ref<UniformBufferSet> &unformBufferSet = nullptr) override;

		virtual Ref<Shader> GetShader() const override { return m_Shader; }
		virtual const HLString &GetName() const override { return m_Name; }

		// Vulkan-specific
		void InvalidateDescriptorSets();
		Allocator GetUniformStorageBuffer() { return m_LocalData; }

		void SetUniformBufferWriteDescriptors(const std::vector<std::vector<VkWriteDescriptorSet>> &uniformBufferWriteDescriptors)
		{
			m_UniformBufferWriteDescriptorSets = uniformBufferWriteDescriptors;
		}

	private:

		void AllocateStorage();
		void OnShaderReloaded();

		void SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture);
		void SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex);
		void SetVulkanDescriptor(const HLString &name, const Ref<Texture3D> &texture);

		const ShaderUniform *FindUniformDeclaration(const HLString &name);
		const ShaderResourceDeclaration *FindResourceDeclaration(const HLString &name);

	private:

		enum class PendingDescriptorType
		{
			None = 0,
			Texture2D,
			Texture3D
		};

		struct PendingDescriptor : public IsSharedReference
		{
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WriteDescriptor;
			VkDescriptorImageInfo ImageInfo;
			Ref<Texture> Texture;
			VkDescriptorImageInfo SubmittedImageInfo{};
		};

		struct PendingDescriptorArray : public IsSharedReference
		{
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WriteDescriptor;
			std::vector<VkDescriptorImageInfo> ImageInfos;
			std::vector<Ref<Texture>> Textures;
			VkDescriptorImageInfo SubmittedImageInfo{};
		};

		std::unordered_map<uint32, Ref<PendingDescriptor>> m_ResidentDescriptors;
		std::unordered_map<uint32, Ref<PendingDescriptorArray>> m_ResidentDescriptorArrays;
		std::vector<WeakRef<PendingDescriptor>> m_PendingDescriptors;

	private:

		Ref<Shader> m_Shader;
		HLString m_Name;
		uint32 m_Flags = 0;

		Allocator m_LocalData;

		std::vector<Ref<Texture>> m_Textures;
		std::vector<std::vector<Ref<Texture>>> m_TextureArrays;
		std::vector<std::vector<VkWriteDescriptorSet>> m_WriteDescriptorSets;
		std::vector<std::vector<VkWriteDescriptorSet>> m_UniformBufferWriteDescriptorSets;

		VulkanShaderMaterialDescriptorSet m_DescriptorSets[3];
		std::vector<bool> m_DirtyDescriptorSets;
	};
}

#endif // HIGHLO_API_VULKAN

