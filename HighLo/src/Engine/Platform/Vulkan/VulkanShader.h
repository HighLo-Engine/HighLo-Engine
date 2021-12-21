// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"

#include "Engine/Renderer/Shaders/Shader.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanAllocator.h"

namespace highlo
{
	class VulkanShader : public Shader
	{
	public:

		struct UniformBuffer
		{
			VkDescriptorBufferInfo Descriptor;
			uint32 Size = 0;
			uint32 BindingPoint = 0;
			HLString Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct StorageBuffer
		{
			VmaAllocation MemoryAllocation = nullptr;
			VkDescriptorBufferInfo Descriptor;
			uint32 Size = 0;
			uint32 BindingPoint = 0;
			HLString Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct ImageSampler
		{
			uint32 BindingPoint = 0;
			uint32 DescriptorSet = 0;
			uint32 ArraySize = 0;
			HLString Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct PushConstantRange
		{
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
			uint32 Size = 0;
			uint32 Offset = 0;
		};

		struct ShaderDescriptorSet
		{
			std::unordered_map<uint32, UniformBuffer*> UniformBuffers;
			std::unordered_map<uint32, StorageBuffer*> StorageBuffers;
			std::unordered_map<uint32, ImageSampler> ImageSamplers;
			std::unordered_map<uint32, ImageSampler> StorageImages;
			std::unordered_map<HLString, VkWriteDescriptorSet> WriteDescriptorSets;

			operator bool() const
			{
				return !(UniformBuffers.empty())
					&& !(StorageBuffers.empty())
					&& !(ImageSamplers.empty())
					&& !(StorageImages.empty());
			}
		};

		struct ShaderMaterialDescriptorSet
		{
			VkDescriptorPool Pool = nullptr;
			std::vector<VkDescriptorSet> DescriptorSets;
		};

	public:

		// Inherited via Shader
		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual uint64 GetHash() const override;

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override;

		// Vulkan-specific
		const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageCreateInfos() const { return m_PipelineShaderStageCreateInfos; }

		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32 set) { return m_DescriptorSetLayouts.at(set); }
		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();

		UniformBuffer &GetUniformBuffer(uint32 binding = 0, uint32 set = 0);
		uint32 GetUniformBufferCount(uint32 set = 0);

		const std::vector<ShaderDescriptorSet> &GetShaderDescriptorSets() const { return m_ShaderDescriptorSets; }
		bool HasDescriptorSet(uint32 set) const { return m_TypeCounts.find(set) != m_TypeCounts.end(); }

		const std::vector<PushConstantRange> &GetPushConstantRanges() const { return m_PushConstantRanges; }

		ShaderMaterialDescriptorSet AllocateDescriptorSet(uint32 set = 0);
		ShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set = 0);
		ShaderMaterialDescriptorSet CreateDescriptorSets(uint32 set, uint32 numberOfSets);
		const VkWriteDescriptorSet *GetDescriptorSet(const HLString &name, uint32 set = 0) const;

		static void ClearUniformBuffers();

	private:

		std::unordered_map<VkShaderStageFlagBits, std::string> PreProcess(const HLString &source);
		void CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, bool forceCompile);
		void LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData);
		void Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData);
		void ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData);

		void CreateDescriptors();

	private:

		HLString m_Name;
		HLRendererID m_RendererID = 0;
		FileSystemPath m_AssetPath;

		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<PushConstantRange> m_PushConstantRanges;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;

		std::vector<ShaderDescriptorSet> m_ShaderDescriptorSets;
		std::vector<VkPipelineShaderStageCreateInfo> m_PipelineShaderStageCreateInfos;
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		VkDescriptorSet m_DescriptorSet;
		std::unordered_map<VkShaderStageFlagBits, HLString> m_ShaderSource;
		std::unordered_map<uint32, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
	};
}

#endif // HIGHLO_API_VULKAN

