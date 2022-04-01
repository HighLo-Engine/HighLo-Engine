#include "HighLoPch.h"
#include "VulkanMaterial.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanMaterial::VulkanMaterial(const Ref<Shader> &shader, const HLString &name)
        : m_Shader(shader), m_Name(name)
    {
        Invalidate();
    }

    VulkanMaterial::VulkanMaterial(const Ref<Material> &other, const HLString &name)
        : m_Shader(other->GetShader()), m_Name(name)
    {
        Invalidate();
    }

    VulkanMaterial::~VulkanMaterial()
    {
    }
    
    void VulkanMaterial::Invalidate()
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, float value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, int32 value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, uint32 value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, bool value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::vec2 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::vec3 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::vec4 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::ivec2 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::ivec3 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::ivec4 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::mat2 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::mat3 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const glm::mat4 &value)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture)
    {
    }

    void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot)
    {
    }
    
    void VulkanMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
    {
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
        return Ref<Texture2D>();
    }
    
    Ref<Texture3D> VulkanMaterial::GetTexture3D(const HLString &name)
    {
        return Ref<Texture3D>();
    }
    
    Ref<Texture2D> VulkanMaterial::TryGetTexture2D(const HLString &name)
    {
        return Ref<Texture2D>();
    }
    
    Ref<Texture3D> VulkanMaterial::TryGetTexture3D(const HLString &name)
    {
        return Ref<Texture3D>();
    }
    
    bool VulkanMaterial::GetFlag(MaterialFlag flag) const
    {
        return false;
    }
    
    void VulkanMaterial::SetFlag(MaterialFlag flag, bool value)
    {
    }
    
    void VulkanMaterial::UpdateForRendering()
    {
    }

    void VulkanMaterial::AllocateStorage()
    {
    }

    void VulkanMaterial::OnShaderReloaded()
    {
    }

    const ShaderUniform *VulkanMaterial::FindUniformDeclaration(const HLString &name)
    {
        return nullptr;
    }

    const ShaderResourceDeclaration *VulkanMaterial::FindResourceDeclaration(const HLString &name)
    {
        return nullptr;
    }
}

#endif // HIGHLO_API_VULKAN

