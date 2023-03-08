// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Graphics/Shaders/ShaderPreProcessor.h"

#include "Vulkan.h"
#include <vk_mem_alloc.h>

#include "VulkanShaderResource.h"

#include <spirv_cross/spirv_glsl.hpp>

namespace highlo
{
	struct VkShaderMaterialDescriptorSet
	{
		VkDescriptorPool Pool = nullptr;
		std::vector<VkDescriptorSet> DescriptorSets;
	};

	class VulkanShader : public Shader
	{
	public:

		struct ReflectionData
		{
			std::vector<VkShaderResource::ShaderDescriptorSet> ShaderDescriptorSets;
			std::unordered_map<HLString, ShaderResourceDeclaration> Resources;
			std::unordered_map<HLString, ShaderBuffer> ConstantBuffers;
			std::vector<VkShaderResource::PushConstantRange> PushConstantRanges;
		};

	public:

		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source, const HLString &name = "undefined", ShaderLanguage language = ShaderLanguage::GLSL);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override { return m_FilePath.Hash(); }

		virtual void Bind() const override {}
		virtual void Unbind() override {}

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_ReflectionData.ConstantBuffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_ReflectionData.Resources; }
		virtual const ShaderResourceDeclaration *GetResource(const HLString &name) const override;

		virtual void SetMacro(const HLString &name, const HLString &value) override;

		/*
		bool TryReadReflectionData(StreamReader *serializer);
		void SerializeReflectionData(StreamWriter *serializer);
		void SetReflectionData(const ReflectionData &reflectionData);
		*/

		// Vulkan-specific
		const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageCreateInfos() const { return m_PipelineShaderStageCreateInfos; }
		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32 set) { return m_DescriptorSetLayouts.at(set); }
		const std::vector<VkShaderResource::ShaderDescriptorSet> &GetShaderDescriptorSets() const { return m_ReflectionData.ShaderDescriptorSets; }
		bool HasDescriptorSet(uint32 set) const { return m_TypeCounts.find(set) != m_TypeCounts.end(); }

		const std::vector<VkShaderResource::PushConstantRange> &GetPushConstantRanges() const { return m_ReflectionData.PushConstantRanges; }
		VkShaderResource::UniformBuffer &GetUniformBuffer(const uint32 binding = 0, const uint32 set = 0) { HL_ASSERT(m_ReflectionData.ShaderDescriptorSets.at(set).UniformBuffers.size() > binding); return m_ReflectionData.ShaderDescriptorSets.at(set).UniformBuffers.at(binding); }
		uint32 GetUniformBufferCount(const uint32 set = 0)
		{
			if (m_ReflectionData.ShaderDescriptorSets.size() < set)
				return 0;

			return (uint32)m_ReflectionData.ShaderDescriptorSets[set].UniformBuffers.size();
		}

		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();

		VkShaderMaterialDescriptorSet AllocateDescriptorSet(uint32 set = 0);
		VkShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set = 0);
		VkShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set, uint32 numberOfSets);
		const VkWriteDescriptorSet *GetDescriptorSet(const HLString &name, uint32 set = 0) const;

	private:

		// Shader-compilation
		void Load(const HLString &source, bool forceCompile = false);
		HLString Compile(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, const VkShaderStageFlagBits stage) const;
		void CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, bool forceCompile);
		void TryGetVulkanCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, VkShaderStageFlagBits stage) const;
		void LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData);
		void CreateDescriptors();

		// Pre-Processing
		std::unordered_map<VkShaderStageFlagBits, HLString> PreProcess(const HLString &source);
		std::unordered_map<VkShaderStageFlagBits, HLString> PreProcessGLSL(const HLString &source);
		std::unordered_map<VkShaderStageFlagBits, HLString> PreProcessHLSL(const HLString &source);

		// Shader-Reflection
		void ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData);
		void Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData);

	private:

		HLRendererID m_RendererID = 0;
		HLString m_Name;
		FileSystemPath m_FilePath;
		ShaderLanguage m_Language = ShaderLanguage::None;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		bool m_DisableOptimization = false;
		uint32 m_ConstantBufferOffset = 0;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;
		std::unordered_map<VkShaderStageFlagBits, HLString> m_ShaderSources;
		std::map<VkShaderStageFlagBits, StageData> m_StagesMetaData;

		ReflectionData m_ReflectionData;
		std::unordered_map<uint32, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
		std::vector<VkPipelineShaderStageCreateInfo> m_PipelineShaderStageCreateInfos;

		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		VkDescriptorSet m_DescriptorSet;


	};
}

#endif // HIGHLO_API_VULKAN

