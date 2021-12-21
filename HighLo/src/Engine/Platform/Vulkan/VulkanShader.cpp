// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
	}

	VulkanShader::VulkanShader(const HLString &source)
	{
	}

	VulkanShader::~VulkanShader()
	{
	}

	void VulkanShader::Reload(bool forceCompile)
	{
	}

	uint64 VulkanShader::GetHash() const
	{
		return m_AssetPath.Hash();
	}

	void VulkanShader::Bind() const
	{
	}

	void VulkanShader::Unbind()
	{
	}
	
	void VulkanShader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
	{
		m_ReloadedCallbacks.push_back(callback);
	}
	
	const std::unordered_map<HLString, ShaderResourceDeclaration> &VulkanShader::GetResources() const
	{
		return m_Resources;
	}

	std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
	{
		std::vector<VkDescriptorSetLayout> result;
		return result;
	}
	
	VulkanShader::UniformBuffer &VulkanShader::GetUniformBuffer(uint32 binding, uint32 set)
	{
		UniformBuffer *buffer = nullptr;
		return *buffer;
	}
	
	uint32 VulkanShader::GetUniformBufferCount(uint32 set)
	{
		return 0;
	}
	
	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32 set)
	{
		ShaderMaterialDescriptorSet result;
		return result;
	}
	
	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set)
	{
		ShaderMaterialDescriptorSet result;
		return result;
	}
	
	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set, uint32 numberOfSets)
	{
		ShaderMaterialDescriptorSet result;
		return result;
	}
	
	const VkWriteDescriptorSet *VulkanShader::GetDescriptorSet(const HLString &name, uint32 set) const
	{
		return nullptr;
	}
	
	void VulkanShader::ClearUniformBuffers()
	{
	}
	
	std::unordered_map<VkShaderStageFlagBits, std::string> VulkanShader::PreProcess(const HLString &source)
	{
		std::unordered_map<VkShaderStageFlagBits, std::string> result;
		return result;
	}
	
	void VulkanShader::CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, bool forceCompile)
	{
	}
	
	void VulkanShader::LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
	{
	}
	
	void VulkanShader::Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData)
	{
	}
	
	void VulkanShader::ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
	{
	}
	
	void VulkanShader::CreateDescriptors()
	{
	}
}

#endif // HIGHLO_API_VULKAN

