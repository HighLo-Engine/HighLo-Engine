// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

namespace highlo
{
    VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
        : m_FilePath(filePath)
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
        return m_FilePath.Hash();
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
}

