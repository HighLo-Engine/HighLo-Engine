// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Core/FileSystem.h"
#include "Engine/Renderer/Renderer.h"

#define VULKAN_SHADER_LOG_PREFIX "Shader>       "

namespace highlo
{
    VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
        : m_AssetPath(filePath)
    {
        m_Name = m_AssetPath.Filename();
        m_Language = ShaderLanguage::None; // TODO
        HLString source = FileSystem::Get()->ReadTextFile(filePath);
        Load(source, forceCompile);
    }

    VulkanShader::VulkanShader(const HLString &source, const HLString &name, ShaderLanguage language)
    {
        m_Name = name;
        m_Language = language;
        Load(source, true);
    }
    
    VulkanShader::~VulkanShader()
    {
        Release();
    }
    
    void VulkanShader::Reload(bool forceCompile)
    {
        HL_CORE_INFO(VULKAN_SHADER_LOG_PREFIX "[+] Reloading shader {0}... [+]", **m_AssetPath);
        m_Loaded = false; // Reflect current stage: Shader is being reloaded

        HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
        Load(source, forceCompile);

        Renderer::OnShaderReloaded(GetHash());

        for (ShaderReloadedCallback callback : m_ReloadedCallbacks)
            callback();
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
        m_ReloadedCallbacks.push_back(callback);
    }
    
    const std::unordered_map<HLString, ShaderBuffer> &VulkanShader::GetShaderBuffers() const
    {
        return m_Buffers;
    }
    
    const std::unordered_map<HLString, ShaderResourceDeclaration> &VulkanShader::GetResources() const
    {
        return m_Resources;
    }
    
    void VulkanShader::SetMacro(const HLString &name, const HLString &value)
    {
        m_Macros[name] = value;
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

#endif // HIGHLO_API_VULKAN

