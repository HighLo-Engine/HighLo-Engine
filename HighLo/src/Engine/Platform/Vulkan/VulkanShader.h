// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Graphics/Shaders/ShaderPreProcessor.h"

#include "Vulkan.h"

namespace highlo
{
	namespace internal
	{
		struct VulkanShaderMaterialDescriptorSet
		{
			VkDescriptorPool Pool = nullptr;
			std::vector<VkDescriptorSet> Descriptors;
		};

		struct VulkanShaderUniformBuffer
		{
			VkDescriptorBufferInfo Descriptor;
			uint32 Size = 0;
			uint32 BindingPoint = 0;
			HLString Name;
			VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct VulkanShaderStorageBuffer
		{
			VkDescriptorBufferInfo Descriptor;
			uint32 Size = 0;
			uint32 BindingPoint = 0;
			HLString Name;
			VmaAllocation Allocation = nullptr;
			VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct VulkanShaderImageSampler
		{
			uint32 BindingPoint = 0;
			uint32 DescriptorSet = 0;
			uint32 ArraySize = 0;
			HLString Name;
			VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct VulkanShaderPushConstantRange
		{
			uint32 Offset = 0;
			uint32 Size = 0;
			VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct VulkanShaderDescriptorSet
		{
			std::unordered_map<uint32, VulkanShaderUniformBuffer *> UniformBuffers;
			std::unordered_map<uint32, VulkanShaderStorageBuffer *> StorageBuffers;
			std::unordered_map<uint32, VulkanShaderImageSampler> ImageSamplers;
			std::unordered_map<uint32, VulkanShaderImageSampler> StorageImages;
			std::unordered_map<uint32, VulkanShaderImageSampler> SeparateTextures;
			std::unordered_map<uint32, VulkanShaderImageSampler> SeparateSamplers;

			std::unordered_map<HLString, VkWriteDescriptorSet> WriteDescriptorSets;

			operator bool() const
			{
				return !(StorageBuffers.empty()
						 && UniformBuffers.empty()
						 && ImageSamplers.empty()
						 && StorageImages.empty());
			}
		};
	}

	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source, const HLString &name = "undefined", ShaderLanguage language = ShaderLanguage::GLSL);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override;

		virtual void Bind() const override {}
		virtual void Unbind() override {}

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override;
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override;

		virtual void SetMacro(const HLString &name, const HLString &value) override;

		// Vulkan-specific
		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32 set) { return m_DescriptorSetLayouts.at(set); }

		const std::vector<internal::VulkanShaderDescriptorSet> &GetShaderDescriptorSets() const { return m_ShaderDescriptorSets; }
		bool HasDescriptorSet(uint32 set) const { return m_TypeCounts.find(set) != m_TypeCounts.end(); }

		const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageCreateInfos() const { return m_PipelineShaderStageCreateInfos; }

		const std::vector<internal::VulkanShaderPushConstantRange> &GetPushConstantRanges() const { return m_PushConstantRanges; }

		internal::VulkanShaderMaterialDescriptorSet AllocateDescriptorSet(uint32 set = 0);
		internal::VulkanShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set = 0);
		internal::VulkanShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set, uint32 numberOfSets);
		const VkWriteDescriptorSet *GetDescriptorSet(const HLString &name, uint32 set = 0) const;

		internal::VulkanShaderUniformBuffer &GetUniformBuffer(const uint32 binding = 0, const uint32 set = 0)
		{
			HL_ASSERT(m_ShaderDescriptorSets.at(set).UniformBuffers.size() > binding);
			return *m_ShaderDescriptorSets.at(set).UniformBuffers.at(binding);
		}

		uint32 GetUniformBufferCount(const uint32 set = 0)
		{
			if (m_ShaderDescriptorSets.size() < set)
				return 0;

			return (uint32)m_ShaderDescriptorSets[set].UniformBuffers.size();
		}

	private:

		void Load(const HLString &source, bool forceCompile);

		// Pre-Processing
		std::unordered_map<VkShaderStageFlagBits, HLString> PreProcess(const HLString &source);
		std::unordered_map<VkShaderStageFlagBits, HLString> PreProcessGLSL(const HLString &source);
		std::unordered_map<VkShaderStageFlagBits, HLString> PreProcessHLSL(const HLString &source);

		// Shader-Reflection
		void ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData);
		void Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData);

		// Shader-compilation
		HLString Compile(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, const VkShaderStageFlagBits stage) const;
		void CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, bool forceCompile);
		void TryGetVulkanCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, VkShaderStageFlagBits stage) const;
		void LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData);
		void CreateDescriptors();

	private:

		HLRendererID m_RendererID = 0;
		HLString m_Name;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		FileSystemPath m_AssetPath;
		ShaderLanguage m_Language = ShaderLanguage::None;

		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;

		// Vulkan-specific
		std::vector<VkPipelineShaderStageCreateInfo> m_PipelineShaderStageCreateInfos;
		std::unordered_map<VkShaderStageFlagBits, HLString> m_ShaderSources;
		std::vector<internal::VulkanShaderDescriptorSet> m_ShaderDescriptorSets;
		std::vector<internal::VulkanShaderPushConstantRange> m_PushConstantRanges;
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::unordered_map<uint32, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
		VkDescriptorSet m_DescriptorSet;

		std::map<VkShaderStageFlagBits, StageData> m_StagesMetaData;
	};
}

#endif // HIGHLO_API_VULKAN

