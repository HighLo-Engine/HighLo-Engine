#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "VulkanShaderDefs.h"

namespace highlo
{
	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual uint64 GetHash() const override { return m_AssetPath.Hash(); }

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_Resources; }

		static void ClearUniformBuffers();

		// Vulkan-specific
		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32 set) { return m_DescriptorSetLayouts.at(set); }

		const std::vector<VulkanShaderDescriptorSet> &GetShaderDescriptorSets() const { return m_ShaderDescriptorSets; }
		bool HasDescriptorSet(uint32 set) const { return m_TypeCounts.find(set) != m_TypeCounts.end(); }

		const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageCreateInfos() const { return m_PipelineShaderStageCreateInfos; }

		const std::vector<VulkanShaderPushConstantRange> &GetPushConstantRanges() const { return m_PushConstantRanges; }

		VulkanShaderDescriptorSet AllocateDescriptorSet(uint32 set = 0);
		VulkanShaderDescriptorSet CreateDescriptorSets(uint32 set = 0);
		VulkanShaderDescriptorSet CreateDescriptorSets(uint32 set, uint32 numberOfSets);
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

		HLRendererID m_RendererID = 0;
		HLString m_Name;
		FileSystemPath m_AssetPath;

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
		
		//	std::map<VkShaderStageFlagBits, StageData> m_StagesMetaData;
	};
}

#endif // HIGHLO_API_VULKAN

