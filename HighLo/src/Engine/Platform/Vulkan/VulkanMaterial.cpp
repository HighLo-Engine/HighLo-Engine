// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanMaterial.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanMaterial::VulkanMaterial(const Ref<Shader> &shader, const HLString &name)
        : m_Shader(shader), m_Name(name)
    {
    }

    VulkanMaterial::VulkanMaterial(const Ref<Material> &other, const HLString &name)
    {
    }

    VulkanMaterial::~VulkanMaterial()
    {
    }
    
    void VulkanMaterial::Invalidate()
    {
    }
    
    bool VulkanMaterial::Has(const HLString &name)
    {
        return false;
    }

    void VulkanMaterial::Set(const HLString &name, float value)
    {
        Set<float>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, int32 value)
    {
        Set<int32>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, uint32 value)
    {
        Set<uint32>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, bool value)
    {
        Set<bool>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::vec2 &value)
    {
        Set<glm::vec2>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::vec3 &value)
    {
        Set<glm::vec3>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::vec4 &value)
    {
        Set<glm::vec4>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::ivec2 &value)
    {
        Set<glm::ivec2>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::ivec3 &value)
    {
        Set<glm::ivec3>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::ivec4 &value)
    {
        Set<glm::ivec4>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::mat2 &value)
    {
        Set<glm::mat2>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::mat3 &value)
    {
        Set<glm::mat3>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::mat4 &value)
    {
        Set<glm::mat4>(name, value);
    }
    
    void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture)
    {
        auto decl = FindResourceDeclaration(name);
        if (!decl)
        {
            HL_CORE_WARN("Cannot find material property: {0}", *name);
            return;
        }

        uint32 slot = decl->GetRegister();
        m_Texture2Ds[slot] = texture;
    }

    void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot)
    {
        auto decl = FindResourceDeclaration(name);
        if (!decl)
        {
            HL_CORE_WARN("Cannot find material property: {0}", *name);
            return;
        }

        m_Texture2Ds[slot] = texture;
    }

    void VulkanMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
    {
        auto decl = FindResourceDeclaration(name);
        if (!decl)
        {
            HL_CORE_WARN("Cannot find material property: {0}", *name);
            return;
        }

        uint32 slot = decl->GetRegister();
        if (m_Textures.size() <= slot)
            m_Textures.resize((size_t)slot + 1);

        m_Textures[slot] = texture;
    }

    float &VulkanMaterial::GetFloat(const HLString &name)
    {
        return Get<float>(name);
    }

    int32 &VulkanMaterial::GetInt(const HLString &name)
    {
        return Get<int32>(name);
    }

    uint32 &VulkanMaterial::GetUInt(const HLString &name)
    {
        return Get<uint32>(name);
    }

    bool &VulkanMaterial::GetBool(const HLString &name)
    {
        return Get<bool>(name);
    }

    glm::vec2 &VulkanMaterial::GetVector2(const HLString &name)
    {
        return Get<glm::vec2>(name);
    }

    glm::vec3 &VulkanMaterial::GetVector3(const HLString &name)
    {
        return Get<glm::vec3>(name);
    }

    glm::vec4 &VulkanMaterial::GetVector4(const HLString &name)
    {
        return Get<glm::vec4>(name);
    }

    glm::ivec2 &VulkanMaterial::GetIVector2(const HLString &name)
    {
        return Get<glm::ivec2>(name);
    }

    glm::ivec3 &VulkanMaterial::GetIVector3(const HLString &name)
    {
        return Get<glm::ivec3>(name);
    }

    glm::ivec4 &VulkanMaterial::GetIVector4(const HLString &name)
    {
        return Get<glm::ivec4>(name);
    }

    glm::mat2 &VulkanMaterial::GetMatrix2(const HLString &name)
    {
        return Get<glm::mat2>(name);
    }

    glm::mat3 &VulkanMaterial::GetMatrix3(const HLString &name)
    {
        return Get<glm::mat3>(name);
    }

    glm::mat4 &VulkanMaterial::GetMatrix4(const HLString &name)
    {
        return Get<glm::mat4>(name);
    }

    Ref<Texture2D> VulkanMaterial::GetTexture2D(const HLString &name)
    {
        return GetResource<Texture2D>(name);
    }

    Ref<Texture3D> VulkanMaterial::GetTexture3D(const HLString &name)
    {
        return GetResource<Texture3D>(name);
    }

    Ref<Texture2D> VulkanMaterial::TryGetTexture2D(const HLString &name)
    {
        return TryGetResource<Texture2D>(name);
    }

    Ref<Texture3D> VulkanMaterial::TryGetTexture3D(const HLString &name)
    {
        return TryGetResource<Texture3D>(name);
    }

    bool VulkanMaterial::GetFlag(MaterialFlag flag) const
    {
        return (uint32)flag & m_MaterialFlags;
    }

    void VulkanMaterial::SetFlag(MaterialFlag flag, bool value)
    {
        if (value)
            m_MaterialFlags |= (uint32)flag;
        else
            m_MaterialFlags &= ~(uint32)flag;
    }

    void VulkanMaterial::UpdateForRendering(const Ref<UniformBufferSet> &uniformBufferSet)
    {
        m_Shader->Bind();

        if (uniformBufferSet)
        {
            uniformBufferSet->ForEach([=](const Ref<UniformBuffer> &uniformBuffer)
            {
                uniformBuffer->Bind();
            });
        }

        for (uint32 i = 0; i < (uint32)m_Textures.size(); ++i)
        {
            auto &texture = m_Textures[i];
            if (texture)
            {
                HL_ASSERT(texture->GetType() == TextureType::Texture3D);
                // TODO
            }
        }

        for (auto &[textureSlot, texture] : m_Texture2Ds)
        {
            if (texture)
            {
                // TODO
            }
        }
    }

    void VulkanMaterial::AllocateStorage()
    {
        const auto &shaderBuffers = m_Shader->GetShaderBuffers();
        if (shaderBuffers.size() > 0)
        {
            uint32 size = 0;
            for (auto &[name, buffer] : shaderBuffers)
                size += buffer.Size;

            m_LocalData.Allocate(size);
            m_LocalData.ZeroInitialize();
        }
    }

    void VulkanMaterial::OnShaderReloaded()
    {
        // TODO
        HL_CORE_INFO("Reloading material {0}", *m_Name);
    }

    const ShaderUniform *VulkanMaterial::FindUniformDeclaration(const HLString &name)
    {
        const auto &shaderBuffers = m_Shader->GetShaderBuffers();

        if (shaderBuffers.size() > 0)
        {
            for (const auto &[n, buffer] : shaderBuffers)
            {
                if (buffer.Uniforms.find(name) != buffer.Uniforms.end())
                {
                    return &buffer.Uniforms.at(name);
                }
            }
        }

        return nullptr;
    }

    const ShaderResourceDeclaration *VulkanMaterial::FindResourceDeclaration(const HLString &name)
    {
        auto &resources = m_Shader->GetResources();
        for (const auto &[n, resource] : resources)
        {
            if (resource.GetName() == name)
                return &resource;
        }

        return nullptr;
    }
}

#endif // HIGHLO_API_VULKAN

