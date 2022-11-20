// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

namespace highlo
{
    VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
        : m_AssetPath(filePath)
    {
    }

    VulkanShader::VulkanShader(const HLString &source, const HLString &name, ShaderLanguage language)
    {
    }
    
    VulkanShader::~VulkanShader()
    {
    }
    
    void VulkanShader::Reload(bool forceCompile)
    {
    }
    
    void VulkanShader::Release()
    {
    }
    
    uint64 VulkanShader::GetHash() const
    {
        return m_AssetPath.Hash();
    }
    
    void VulkanShader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
    {
    }
    
    const std::unordered_map<HLString, ShaderBuffer> &VulkanShader::GetShaderBuffers() const
    {
        return {};
    }
    
    const std::unordered_map<HLString, ShaderResourceDeclaration> &VulkanShader::GetResources() const
    {
        return {};
    }
    
    void VulkanShader::SetMacro(const HLString &name, const HLString &value)
    {
    }

    // VULKAN-SPECIFIC FUNCS

    std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
    {
        return std::vector<VkDescriptorSetLayout>();
    }

    internal::VulkanShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32 set)
    {
        return internal::VulkanShaderMaterialDescriptorSet();
    }

    internal::VulkanShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set)
    {
        return internal::VulkanShaderMaterialDescriptorSet();
    }

    internal::VulkanShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set, uint32 numberOfSets)
    {
        return internal::VulkanShaderMaterialDescriptorSet();
    }

    const VkWriteDescriptorSet *VulkanShader::GetDescriptorSet(const HLString &name, uint32 set) const
    {
        return nullptr;
    }

    // PRIVATE FUNCS
    
    void VulkanShader::Load(const HLString &source, bool forceCompile)
    {
    }
    
    std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcess(const HLString &source)
    {
        return std::unordered_map<VkShaderStageFlagBits, HLString>();
    }
    
    std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcessGLSL(const HLString &source)
    {
        return std::unordered_map<VkShaderStageFlagBits, HLString>();
    }
    
    std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcessHLSL(const HLString &source)
    {
        return std::unordered_map<VkShaderStageFlagBits, HLString>();
    }
    
    void VulkanShader::ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
    {
    }
    
    void VulkanShader::Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData)
    {
    }
    
    HLString VulkanShader::Compile(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, const VkShaderStageFlagBits stage) const
    {
        return HLString();
    }
    
    void VulkanShader::CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, bool forceCompile)
    {
    }
    
    void VulkanShader::TryGetVulkanCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, VkShaderStageFlagBits stage) const
    {
    }
    
    void VulkanShader::LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
    {
    }
    
    void VulkanShader::CreateDescriptors()
    {
    }
}

