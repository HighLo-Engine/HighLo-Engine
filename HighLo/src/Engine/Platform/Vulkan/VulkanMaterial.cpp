#include "HighLoPch.h"
#include "VulkanMaterial.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanShader.h"
#include "VulkanTexture2D.h"
#include "VulkanTexture3D.h"

namespace highlo
{
    VulkanMaterial::VulkanMaterial(const Ref<Shader> &shader, const HLString &name)
        : m_Shader(shader), m_Name(name), m_WriteDescriptorSets(Renderer::GetConfig().FramesInFlight), m_DirtyDescriptorSets(Renderer::GetConfig().FramesInFlight, true)
    {
        AllocateStorage();

        m_Flags |= (uint32)MaterialFlag::DepthTest;
        m_Flags |= (uint32)MaterialFlag::Blend;

        Invalidate();
        Renderer::RegisterShaderDependency(shader, this);
    }

    VulkanMaterial::VulkanMaterial(const Ref<Material> &other, const HLString &name)
        : m_Shader(other->GetShader()), m_Name(name), m_WriteDescriptorSets(Renderer::GetConfig().FramesInFlight), m_DirtyDescriptorSets(Renderer::GetConfig().FramesInFlight, true)
    {
        if (name.IsEmpty())
            m_Name = other->GetName();

        Renderer::RegisterShaderDependency(m_Shader, this);

        auto vulkanMaterial = other.As<VulkanMaterial>();
        m_LocalData = Allocator::Copy(vulkanMaterial->m_LocalData.Data, vulkanMaterial->m_LocalData.Size);

        m_PendingDescriptors = vulkanMaterial->m_PendingDescriptors;
        m_ResidentDescriptors = vulkanMaterial->m_ResidentDescriptors;
        m_ResidentDescriptorArrays = vulkanMaterial->m_ResidentDescriptorArrays;
        m_Textures = vulkanMaterial->m_Textures;
        m_TextureArrays = vulkanMaterial->m_TextureArrays;
    }

    VulkanMaterial::~VulkanMaterial()
    {
        m_LocalData.Release();
    }
    
    void VulkanMaterial::Invalidate()
    {
        uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
        auto shader = m_Shader.As<VulkanShader>();
        if (shader->HasDescriptorSet(0))
        {
            const auto &shaderDescriptorSets = shader->GetShaderDescriptorSets();
            if (!shaderDescriptorSets.empty())
            {
                for (auto &&[binding, descriptor] : m_ResidentDescriptors)
                    m_PendingDescriptors.push_back(descriptor);
            }
        }
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
        SetVulkanDescriptor(name, texture);
    }

    void VulkanMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot)
    {
        SetVulkanDescriptor(name, texture, slot);
    }
    
    void VulkanMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
    {
        SetVulkanDescriptor(name, texture);
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
        return (uint32)flag & m_Flags;
    }
    
    void VulkanMaterial::SetFlag(MaterialFlag flag, bool value)
    {
        if (value)
        {
            m_Flags |= (uint32)flag;
        }
        else
        {
            m_Flags &= ~(uint32)flag;
        }
    }
    
    void VulkanMaterial::UpdateForRendering()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        std::vector<VkDescriptorImageInfo> arrayImageInfos;
        uint32 frameIndex = Renderer::GetCurrentFrameIndex();

        for (auto &&[binding, descriptor] : m_ResidentDescriptors)
        {
            if (descriptor->Type == PendingDescriptorType::Texture2D)
            {
                Ref<VulkanTexture2D> texture = descriptor->Texture.As<VulkanTexture2D>();
                HL_ASSERT(texture->GetTextureInfo().ImageView, "Image View is null!");
                if (descriptor->WriteDescriptor.pImageInfo && texture->GetTextureInfo().ImageView != descriptor->WriteDescriptor.pImageInfo->imageView)
                {
                    m_PendingDescriptors.emplace_back(descriptor);
                    InvalidateDescriptorSets();
                }
            }
        }

        m_DirtyDescriptorSets[frameIndex] = false;
        m_WriteDescriptorSets[frameIndex].clear();

        if (!m_UniformBufferWriteDescriptorSets.empty())
        {
            for (auto &wd : m_UniformBufferWriteDescriptorSets[frameIndex])
                m_WriteDescriptorSets[frameIndex].push_back(wd);
        }

        for (auto &&[binding, pd] : m_ResidentDescriptors)
        {
            if (pd->Type == PendingDescriptorType::Texture2D)
            {
                Ref<VulkanTexture2D> texture = pd->Texture.As<VulkanTexture2D>();
                pd->ImageInfo = texture->GetDescriptorInfo();
                pd->WriteDescriptor.pImageInfo = &pd->ImageInfo;
            }
            else if (pd->Type == PendingDescriptorType::Texture3D)
            {
                Ref<VulkanTexture3D> texture = pd->Texture.As<VulkanTexture3D>();
                pd->ImageInfo = texture->GetDescriptorInfo();
                pd->WriteDescriptor.pImageInfo = &pd->ImageInfo;
            }

            m_WriteDescriptorSets[frameIndex].push_back(pd->WriteDescriptor);
        }

        for (auto &&[binding, pd] : m_ResidentDescriptorArrays)
        {
            if (pd->Type == PendingDescriptorType::Texture2D)
            {
                for (auto tex : pd->Textures)
                {
                    Ref<VulkanTexture2D> texture = tex.As<VulkanTexture2D>();
                    arrayImageInfos.push_back(texture->GetDescriptorInfo());
                }
            }

            pd->WriteDescriptor.pImageInfo = arrayImageInfos.data();
            pd->WriteDescriptor.descriptorCount = (uint32)arrayImageInfos.size();
            m_WriteDescriptorSets[frameIndex].push_back(pd->WriteDescriptor);
        }

        auto vulkanShader = m_Shader.As<VulkanShader>();
        auto descriptorSet = vulkanShader->AllocateDescriptorSet();
        m_DescriptorSets[frameIndex] = descriptorSet;

        for (auto &writeDescriptor : m_WriteDescriptorSets[frameIndex])
            writeDescriptor.dstSet = descriptorSet.Descriptors[0];

        vkUpdateDescriptorSets(device, (uint32)m_WriteDescriptorSets[frameIndex].size(), m_WriteDescriptorSets[frameIndex].data(), 0, nullptr);
        m_PendingDescriptors.clear();
    }

    void VulkanMaterial::InvalidateDescriptorSets()
    {
        const uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
        for (uint32 i = 0; i < framesInFlight; ++i)
            m_DirtyDescriptorSets[i] = true;
    }

    void VulkanMaterial::AllocateStorage()
    {
        const auto &shaderBuffers = m_Shader->GetShaderBuffers();

        if (shaderBuffers.size() > 0)
        {
            uint32 size = 0;
            for (auto &[name, shaderBuffer] : shaderBuffers)
                size += shaderBuffer.Size;

            m_LocalData.Allocate(size);
            m_LocalData.ZeroInitialize();
        }
    }

    void VulkanMaterial::OnShaderReloaded()
    {
        std::unordered_map<uint32, Ref<PendingDescriptor>> newDescriptors;
        std::unordered_map<uint32, Ref<PendingDescriptorArray>> newDescriptorArrays;

        for (auto &[name, resource] : m_Shader->GetResources())
        {
            const uint32 binding = resource.GetRegister();

            if (m_ResidentDescriptors.find(binding) != m_ResidentDescriptors.end())
            {
                newDescriptors[binding] = std::move(m_ResidentDescriptors.at(binding));
            }
            else if (m_ResidentDescriptorArrays.find(binding) != m_ResidentDescriptorArrays.end())
            {
                newDescriptorArrays[binding] = std::move(m_ResidentDescriptorArrays.at(binding));
            }
        }

        m_ResidentDescriptors = std::move(newDescriptors);
        m_ResidentDescriptorArrays = std::move(newDescriptorArrays);

        Invalidate();
        InvalidateDescriptorSets();
    }

    void VulkanMaterial::SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture)
    {
        const ShaderResourceDeclaration *resource = FindResourceDeclaration(name);
        HL_ASSERT(resource);

        uint32 binding = resource->GetRegister();

        if (binding < m_Textures.size() 
            && m_Textures[binding] 
            && texture->GetHash() == m_Textures[binding]->GetHash()
            && m_ResidentDescriptors.find(binding) != m_ResidentDescriptors.end())
            return;

        if (binding >= m_Textures.size())
            m_Textures.resize(binding + 1);

        m_Textures[binding] = texture;

        const VkWriteDescriptorSet *wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
        HL_ASSERT(wds);

        PendingDescriptor desc;
        desc.Type = PendingDescriptorType::Texture2D;
        desc.WriteDescriptor = *wds;
        desc.ImageInfo = {};
        desc.Texture = texture.As<Texture>();
        desc.SubmittedImageInfo = {};
        m_ResidentDescriptors[binding] = Ref<PendingDescriptor>::Create(desc);
        m_PendingDescriptors.push_back(m_ResidentDescriptors.at(binding));

        InvalidateDescriptorSets();
    }

    void VulkanMaterial::SetVulkanDescriptor(const HLString &name, const Ref<Texture2D> &texture, uint32 arrayIndex)
    {
        const ShaderResourceDeclaration *resource = FindResourceDeclaration(name);
        HL_ASSERT(resource);

        uint32 binding = resource->GetRegister();
        if (binding < m_TextureArrays.size()
            && m_TextureArrays[binding].size() < arrayIndex
            && texture->GetHash() == m_TextureArrays[binding][arrayIndex]->GetHash())
            return;

        if (binding >= m_TextureArrays.size())
            m_TextureArrays.resize(binding + 1);

        if (arrayIndex >= m_TextureArrays[binding].size())
            m_TextureArrays[binding].resize(arrayIndex + 1);

        m_TextureArrays[binding][arrayIndex] = texture;

        const VkWriteDescriptorSet *wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
        HL_ASSERT(wds);

        if (m_ResidentDescriptorArrays.find(binding) == m_ResidentDescriptorArrays.end())
        {
            PendingDescriptorArray desc;
            desc.Type = PendingDescriptorType::Texture2D;
            desc.WriteDescriptor = *wds;
            desc.ImageInfos = {};
            desc.Textures = {};
            desc.SubmittedImageInfo = {};
            m_ResidentDescriptorArrays[binding] = Ref<PendingDescriptorArray>::Create(desc);
        }

        auto &residentDescriptorArray = m_ResidentDescriptorArrays.at(binding);
        if (arrayIndex >= residentDescriptorArray->Textures.size())
            residentDescriptorArray->Textures.resize(arrayIndex + 1);

        residentDescriptorArray->Textures[arrayIndex] = texture;

        InvalidateDescriptorSets();
    }

    void VulkanMaterial::SetVulkanDescriptor(const HLString &name, const Ref<Texture3D> &texture)
    {
        const ShaderResourceDeclaration *resource = FindResourceDeclaration(name);
        HL_ASSERT(resource);

        uint32 binding = resource->GetRegister();
        if (binding < m_Textures.size()
            && m_Textures[binding]
            && texture->GetHash() == m_Textures[binding]->GetHash()
            && m_ResidentDescriptors.find(binding) != m_ResidentDescriptors.end())
            return;

        if (binding >= m_Textures.size())
            m_Textures.resize(binding + 1);

        m_Textures[binding] = texture;

        const VkWriteDescriptorSet *wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
        HL_ASSERT(wds);

        PendingDescriptor desc;
        desc.Type = PendingDescriptorType::Texture2D;
        desc.WriteDescriptor = *wds;
        desc.ImageInfo = {};
        desc.Texture = texture.As<Texture>();
        desc.SubmittedImageInfo = {};

        m_ResidentDescriptors[binding] = Ref<PendingDescriptor>::Create(desc);
        m_PendingDescriptors.push_back(m_ResidentDescriptors.at(binding));

        InvalidateDescriptorSets();
    }

    const ShaderUniform *VulkanMaterial::FindUniformDeclaration(const HLString &name)
    {
        const auto &shaderBuffers = m_Shader->GetShaderBuffers();
        for (const auto &[n, buffer] : shaderBuffers)
        {
            if (buffer.Uniforms.find(name) != buffer.Uniforms.end())
            {
                return &buffer.Uniforms.at(name);
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

