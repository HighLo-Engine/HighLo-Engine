#include "HighLoPch.h"
#include "VulkanShader.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/FileSystem.h"

#include "Engine/Graphics/Shaders/ShaderCache.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    static std::unordered_map<uint32, std::unordered_map<uint32, VulkanShaderUniformBuffer*>> s_UniformBuffers;
    static std::unordered_map<uint32, std::unordered_map<uint32, VulkanShaderStorageBuffer*>> s_StorageBuffers;

    VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
        : m_AssetPath(filePath)
    {
        m_Name = filePath.Filename();
        HLString source = FileSystem::Get()->ReadTextFile(filePath);
        Load(source, forceCompile);
    }

    VulkanShader::VulkanShader(const HLString &source)
    {
        m_Name = "undefined";
        Load(source, true);
    }
    
    VulkanShader::~VulkanShader()
    {
        Release();
    }
    
    void VulkanShader::Reload(bool forceCompile)
    {
        Release();

        m_PipelineShaderStageCreateInfos.clear();
        m_ShaderDescriptorSets.clear();
        m_Resources.clear();
        m_PushConstantRanges.clear();
        m_DescriptorSetLayouts.clear();
        m_ShaderSources.clear();
        m_Buffers.clear();
        m_TypeCounts.clear();
        
        const HLString &source = FileSystem::Get()->ReadTextFile(m_AssetPath);
        HL_ASSERT(!source.IsEmpty(), "Failed to load source!");
        m_ShaderSources = PreProcess(source);
        bool shaderCacheHasChanged = ShaderCache::HasChanged(m_AssetPath, source);

        std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> shaderData;
        CompileOrGetVulkanBinary(shaderData, shaderCacheHasChanged, forceCompile);
        LoadAndCreateShaders(shaderData);
        ReflectAllShaderStages(shaderData);
        CreateDescriptors();

        Renderer::AcknowledgeParsedGlobalMacros(m_AcknowledgedMacros, Ref<VulkanShader>(this));
        Renderer::OnShaderReloaded(GetHash());
    }

    void VulkanShader::Release()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        for (const auto &ci : m_PipelineShaderStageCreateInfos)
        {
            if (ci.module)
                vkDestroyShaderModule(device, ci.module, nullptr);
        }

        for (auto &ci : m_PipelineShaderStageCreateInfos)
        {
            ci.module = nullptr;
        }
    }
    
    void VulkanShader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
    {
        m_ReloadedCallbacks.push_back(callback);
    }
    
    void VulkanShader::SetMacro(const HLString &name, const HLString &value)
    {
        m_Macros[name] = value;
    }

    void VulkanShader::ClearUniformBuffers()
    {
        s_UniformBuffers.clear();
        s_StorageBuffers.clear();
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
        m_Resources.clear();

        for (auto &[stage, data] : shaderData)
        {
            Reflect(stage, data);
        }
    }
    
    void VulkanShader::Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData)
    {
    }

    void VulkanShader::CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, bool cacheChanged, bool forceCompile)
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

