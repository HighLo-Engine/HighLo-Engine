#include "HighLoPch.h"
#include "VulkanShader.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
        : m_AssetPath(filePath)
    {
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
    
    void VulkanShader::ClearUniformBuffers()
    {
    }

    std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
    {
        return std::vector<VkDescriptorSetLayout>();
    }
    
    VulkanShaderDescriptorSet VulkanShader::AllocateDescriptorSet(uint32 set)
    {
        return VulkanShaderDescriptorSet();
    }
    
    VulkanShaderDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set)
    {
        return VulkanShaderDescriptorSet();
    }
    
    VulkanShaderDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set, uint32 numberOfSets)
    {
        return VulkanShaderDescriptorSet();
    }
    
    const VkWriteDescriptorSet *VulkanShader::GetDescriptorSet(const HLString &name, uint32 set) const
    {
        return nullptr;
    }
}

#endif // HIGHLO_API_VULKAN

