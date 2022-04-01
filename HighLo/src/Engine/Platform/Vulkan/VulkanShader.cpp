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
}

#endif // HIGHLO_API_VULKAN

