#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "VulkanShaderDefs.h"
#include "Engine/Graphics/Shaders/ShaderPreProcessor.h"

namespace highlo
{
	struct StageData
	{
		std::unordered_set<IncludeData> Headers;
		uint64 HashValue = 0;

		bool operator==(const StageData &other) const
		{
			return Headers == other.Headers && HashValue == other.HashValue;
		}

		bool operator!=(const StageData &other) const
		{
			return !(*this == other);
		}
	};

	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override { return m_AssetPath.Hash(); }

		virtual void Bind() const override {}
		virtual void Unbind() override {}

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_Resources; }

		virtual void SetMacro(const HLString &name, const HLString &value) override;

		static void ClearUniformBuffers();

		// Vulkan-specific
		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32 set) { return m_DescriptorSetLayouts.at(set); }

		const std::vector<VulkanShaderDescriptorSet> &GetShaderDescriptorSets() const { return m_ShaderDescriptorSets; }
		bool HasDescriptorSet(uint32 set) const { return m_TypeCounts.find(set) != m_TypeCounts.end(); }

		const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageCreateInfos() const { return m_PipelineShaderStageCreateInfos; }

		const std::vector<VulkanShaderPushConstantRange> &GetPushConstantRanges() const { return m_PushConstantRanges; }

		VulkanShaderMaterialDescriptorSet AllocateDescriptorSet(uint32 set = 0);
		VulkanShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set = 0);
		VulkanShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set, uint32 numberOfSets);
		const VkWriteDescriptorSet *GetDescriptorSet(const HLString &name, uint32 set = 0) const;

		VulkanShaderUniformBuffer &GetUniformBuffer(const uint32 binding = 0, const uint32 set = 0)
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
		FileSystemPath m_AssetPath;
		ShaderLanguage m_Language = ShaderLanguage::None;
		Ref<ShaderPreProcessor> m_PreProcessor = nullptr;

		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;

		// Vulkan-specific
		std::vector<VkPipelineShaderStageCreateInfo> m_PipelineShaderStageCreateInfos;
		std::unordered_map<VkShaderStageFlagBits, HLString> m_ShaderSources;
		std::vector<VulkanShaderDescriptorSet> m_ShaderDescriptorSets;
		std::vector<VulkanShaderPushConstantRange> m_PushConstantRanges;
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::unordered_map<uint32, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
		VkDescriptorSet m_DescriptorSet;
		
		std::map<VkShaderStageFlagBits, StageData> m_StagesMetaData;
	};
}

#endif // HIGHLO_API_VULKAN

