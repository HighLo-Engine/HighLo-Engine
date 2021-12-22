#pragma once

#include "Engine/Renderer/Material.h"
#include "Engine/Core/Allocator.h"

#ifdef HIGHLO_API_VULKAN

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
		virtual void Set(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex) override;
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

		virtual Ref<Texture2D> &GetTexture2D(const HLString &name) override;
		virtual Ref<Texture3D> GetTexture3D(const HLString &name) override;

		virtual Ref<Texture2D> TryGetTexture2D(const HLString &name) override;
		virtual Ref<Texture3D> TryGetTexture3D(const HLString &name) override;

		virtual uint32 GetFlags() const override { return m_Flags; }
		virtual bool GetFlag(MaterialFlag flag) const override;
		virtual void SetFlag(MaterialFlag flag, bool value = true) override;

		virtual Ref<Shader> GetShader() const override { return m_Shader; }
		virtual const HLString &GetName() const override { return m_Name; }

	private:

		enum class PendingDescriptorType
		{
			None = 0,
			Texture2D,
			Texture3D,
		};

		struct PendingDescriptor : public IsSharedReference
		{
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WriteDescriptorSet;
			VkDescriptorImageInfo TextureInfo;
			VkDescriptorImageInfo SubmittedTextureInfo{};
			Ref<Texture> Texture;
		};

		struct PendingDescriptorArray : public IsSharedReference
		{
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WriteDescriptorSet;
			VkDescriptorImageInfo TextureInfo;
			VkDescriptorImageInfo SubmittedTextureInfo{};
			std::vector<Ref<Texture>> Textures;
		};

	private:

		std::unordered_map<uint32, Ref<PendingDescriptor>> m_ResidentDescriptors;
		std::unordered_map<uint32, Ref<PendingDescriptorArray>> m_ResidentDescriptorArrays;
		std::vector<std::weak_ptr<PendingDescriptor>> m_PendingDescriptors;

		Ref<Shader> m_Shader;
		HLString m_Name;
		uint32 m_Flags = 0;

		Allocator m_UniformStorageBuffer;
		std::vector<Ref<Texture>> m_Textures;
		std::vector<std::vector<Ref<Texture>>> m_TextureArrays;

		VulkanShader::ShaderMaterialDescriptorSet m_DescriptorSets[3];

		std::unordered_map<uint32, uint64> m_ImageHashes;

		std::vector<std::vector<VkWriteDescriptorSet>> m_WriteDescriptors;
		std::vector<bool> m_DirtyDescriptorSets;

		std::unordered_map<HLString, VkDescriptorImageInfo> m_ImageInfos;
	};
}

#endif // HIGHLO_API_VULKAN

