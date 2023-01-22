// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/FileSystem.h"

#include "Engine/Graphics/Shaders/ShaderCache.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanRenderingAPI.h"

#include <spirv_cross/spirv_glsl.hpp>
#include <spirv-tools/libspirv.h>

#include <shaderc/shaderc.hpp>
#include <libshaderc_util/file_finder.h>

#include "Engine/Graphics/Shaders/GLSLIncluder.h"

#define VULKAN_SHADER_LOG_PREFIX "Shader>       "
#define PRINT_DEBUG_ALL 0

#ifdef HL_DEBUG
#define PRINT_DEBUG_OUTPUTS 1
#endif

#if PRINT_DEBUG_ALL
#define PRINT_DEBUG_OUTPUTS 1
#define PRINT_PREPROCESSING_RESULT 1
#endif

namespace highlo
{
    namespace utils
    {
        static shaderc_shader_kind ShaderStageToShaderC(const VkShaderStageFlagBits stage)
        {
            switch (stage)
            {
            case VK_SHADER_STAGE_VERTEX_BIT:
                return shaderc_vertex_shader;

            case VK_SHADER_STAGE_FRAGMENT_BIT:
                return shaderc_fragment_shader;

            case VK_SHADER_STAGE_COMPUTE_BIT:
                return shaderc_compute_shader;

            case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                return shaderc_tess_control_shader;

            case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                return shaderc_tess_evaluation_shader;

            case VK_SHADER_STAGE_GEOMETRY_BIT:
                return shaderc_geometry_shader;
            }

            HL_ASSERT(false);
            return {};
        }

        static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
        {
            switch (type.basetype)
            {
                case spirv_cross::SPIRType::Boolean: return ShaderUniformType::Bool;
                case spirv_cross::SPIRType::UInt: return ShaderUniformType::Uint;
                case spirv_cross::SPIRType::Int:
                {
                    if (type.vecsize == 1)
                        return ShaderUniformType::Int;

                    if (type.vecsize == 2)
                        return ShaderUniformType::IVec2;

                    if (type.vecsize == 3)
                        return ShaderUniformType::IVec3;

                    if (type.vecsize == 4)
                        return ShaderUniformType::IVec4;

                    break;
                }

                case spirv_cross::SPIRType::Float:
                {
                    if (type.columns == 2)
                        return ShaderUniformType::Mat2;

                    if (type.columns == 3)
                        return ShaderUniformType::Mat3;

                    if (type.columns == 4)
                        return ShaderUniformType::Mat4;

                    if (type.vecsize == 1)
                        return ShaderUniformType::Float;

                    if (type.vecsize == 2)
                        return ShaderUniformType::Vec2;

                    if (type.vecsize == 3)
                        return ShaderUniformType::Vec3;

                    if (type.vecsize == 4)
                        return ShaderUniformType::Vec4;

                    break;
                }
            }

            HL_ASSERT(false);
            return ShaderUniformType::None;
        }

        static ShaderLanguage ShaderLanguageFromExtension(const HLString &str)
        {
            if (str == "glsl")
                return ShaderLanguage::GLSL;

            if (str == "hlsl")
                return ShaderLanguage::HLSL;

            HL_ASSERT(false);
            return ShaderLanguage::None;
        }

        static FileSystemPath GetCacheDirectory()
        {
            FileSystemPath path = "assets/cache/shaders/Vulkan/";

            if (!FileSystem::Get()->FolderExists(path))
                FileSystem::Get()->CreateFolder(path);

            return path;
        }

        static HLString ShaderStageCachedFileExtension(const VkShaderStageFlagBits stage)
        {
            switch (stage)
            {
                case VK_SHADER_STAGE_VERTEX_BIT:                    return ".cached_vulkan.vert";
                case VK_SHADER_STAGE_FRAGMENT_BIT:                  return ".cached_vulkan.frag";
                case VK_SHADER_STAGE_COMPUTE_BIT:                   return ".cached_vulkan.comp";
                case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:      return ".cached_vulkan.tessconn";
                case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:   return ".cached_vulkan.tessval";
                case VK_SHADER_STAGE_GEOMETRY_BIT:                  return ".cached_vulkan.geo";
            }

            HL_ASSERT(false);
            return "";
        }
    }

    static std::unordered_map<uint32, std::unordered_map<uint32, VulkanShaderUniformBuffer*>> s_UniformBuffers;
    static std::unordered_map<uint32, std::unordered_map<uint32, VulkanShaderStorageBuffer*>> s_StorageBuffers;

    VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
        : m_AssetPath(filePath)
    {
        m_Name = filePath.Filename();
        m_Language = utils::ShaderLanguageFromExtension(filePath.Extension());
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
        HL_CORE_INFO(VULKAN_SHADER_LOG_PREFIX "[+] Reloading shader {0}... [+]", **m_AssetPath);

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
        Load(source, forceCompile);

        Renderer::OnShaderReloaded(GetHash());
        for (auto &callback : m_ReloadedCallbacks)
            callback();
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
        std::vector<VkDescriptorSetLayout> result;
        result.reserve(m_DescriptorSetLayouts.size());
        for (auto &layout : m_DescriptorSetLayouts)
            result.emplace_back(layout);

        return result;
    }
    
    VulkanShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32 set)
    {
        HL_ASSERT(set < m_DescriptorSetLayouts.size());
        VulkanShaderMaterialDescriptorSet result;

        if (m_ShaderDescriptorSets.empty())
            return result;

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

        VkDescriptorSet descriptorSet = VulkanRenderingAPI::AllocateDescriptorSet(allocInfo);
        HL_ASSERT(descriptorSet);

        result.Descriptors.push_back(descriptorSet);
        result.Pool = nullptr;
        return result;
    }
    
    VulkanShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set)
    {
        HL_ASSERT(m_TypeCounts.find(set) != m_TypeCounts.end());

        VulkanShaderMaterialDescriptorSet result;
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext = nullptr;
        descriptorPoolInfo.poolSizeCount = (uint32)m_TypeCounts.at(set).size();
        descriptorPoolInfo.pPoolSizes = m_TypeCounts.at(set).data();
        descriptorPoolInfo.maxSets = 1;
        VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &result.Pool));
        
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorSetCount = 1;
        allocInfo.descriptorPool = result.Pool;
        allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

        result.Descriptors.emplace_back();
        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, result.Descriptors.data()));
        return result;
    }
    
    VulkanShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set, uint32 numberOfSets)
    {
        VulkanShaderMaterialDescriptorSet result;
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> poolSizes;

        for (uint32 i = 0; i < m_ShaderDescriptorSets.size(); ++i)
        {
            auto &shaderDescriptorSet = m_ShaderDescriptorSets[i];
            if (!shaderDescriptorSet)
                continue;

            if (shaderDescriptorSet.UniformBuffers.size())
            {
                VkDescriptorPoolSize &typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.UniformBuffers.size() * numberOfSets;
            }

            if (shaderDescriptorSet.StorageBuffers.size())
            {
                VkDescriptorPoolSize &typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageBuffers.size() * numberOfSets;
            }
        
            if (shaderDescriptorSet.ImageSamplers.size())
            {
                VkDescriptorPoolSize &typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                
                uint32 descriptorSetCount = 0;
                for (auto &&[binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
                    descriptorSetCount += imageSampler.ArraySize;

                typeCount.descriptorCount = descriptorSetCount * numberOfSets;
            }
        
            if (shaderDescriptorSet.SeparateTextures.size())
            {
                VkDescriptorPoolSize &typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

                uint32 descriptorSetCount = 0;
                for (auto &&[binding, imageSampler] : shaderDescriptorSet.SeparateTextures)
                    descriptorSetCount += imageSampler.ArraySize;

                typeCount.descriptorCount = descriptorSetCount * numberOfSets;
            }
        
            if (shaderDescriptorSet.SeparateSamplers.size())
            {
                VkDescriptorPoolSize &typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLER;

                uint32 descriptorSetCount = 0;
                for (auto &&[binding, imageSampler] : shaderDescriptorSet.SeparateSamplers)
                    descriptorSetCount += imageSampler.ArraySize;

                typeCount.descriptorCount = descriptorSetCount * numberOfSets;
            }

            if (shaderDescriptorSet.StorageImages.size())
            {
                VkDescriptorPoolSize &typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageImages.size() * numberOfSets;
            }
        }

        HL_ASSERT(poolSizes.find(set) != poolSizes.end());

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext = nullptr;
        descriptorPoolInfo.maxSets = numberOfSets;
        descriptorPoolInfo.poolSizeCount = (uint32)poolSizes.at(set).size();
        descriptorPoolInfo.pPoolSizes = poolSizes.at(set).data();
        VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &result.Pool));

        result.Descriptors.resize(numberOfSets);
        for (uint32 i = 0; i < numberOfSets; ++i)
        {
            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.pNext = nullptr;
            allocInfo.descriptorPool = result.Pool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];
            VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result.Descriptors[i]));
        }

        return result;
    }
    
    const VkWriteDescriptorSet *VulkanShader::GetDescriptorSet(const HLString &name, uint32 set) const
    {
        HL_ASSERT(set < m_ShaderDescriptorSets.size());
        HL_ASSERT(m_ShaderDescriptorSets[set]);

        if (m_ShaderDescriptorSets.at(set).WriteDescriptorSets.find(name) == m_ShaderDescriptorSets.at(set).WriteDescriptorSets.end())
        {
            HL_CORE_WARN("Shader {0} does not contain requested descriptor set {1}", *m_Name, *name);
            return nullptr;
        }

        return &m_ShaderDescriptorSets.at(set).WriteDescriptorSets.at(name);
    }
    
    void VulkanShader::Load(const HLString &source, bool forceCompile)
    {
        if (FileSystem::Get()->FileExists(m_AssetPath))
        {
            HL_CORE_INFO(VULKAN_SHADER_LOG_PREFIX "[+] Trying to create shader {0} [+]", **m_AssetPath);

            m_ShaderSources = PreProcess(source);

            std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> shaderData;
            CompileOrGetVulkanBinary(shaderData, forceCompile);
            LoadAndCreateShaders(shaderData);
            ReflectAllShaderStages(shaderData);
            CreateDescriptors();

            HL_CORE_INFO(VULKAN_SHADER_LOG_PREFIX "[+] Shader {0} loaded [+]", **m_AssetPath);
            //    Renderer::AcknowledgeParsedGlobalMacros(m_AcknowledgedMacros, Ref<VulkanShader>(this));
        }
        else
        {
            HL_CORE_WARN(VULKAN_SHADER_LOG_PREFIX "[-] Shader {0} not found! [-]", **m_AssetPath);
        }
    }

    std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcess(const HLString &source)
    {
        // Extract vertex and fragment shaders

        switch (m_Language)
        {
            case ShaderLanguage::GLSL:
                return PreProcessGLSL(source);

            case ShaderLanguage::HLSL:
                return PreProcessHLSL(source);
        }

        HL_ASSERT(false);
        return {};
    }
    
    std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcessGLSL(const HLString &source)
    {
        std::unordered_map<ShaderType, HLString> shaderSources = ShaderPreProcessor::PreprocessShader<ShaderLanguage::GLSL>(source, m_AcknowledgedMacros);
        std::unordered_map<VkShaderStageFlagBits, HLString> vkShaderSources = utils::ConvertShaderTypeToVulkanStage(shaderSources);
        shaderc::Compiler compiler;

        shaderc_util::FileFinder fileFinder;
        fileFinder.search_path().emplace_back("assets/shaders/Include/GLSL/");
        fileFinder.search_path().emplace_back("assets/shaders/Include/Shared/");

        HL_CORE_INFO(VULKAN_SHADER_LOG_PREFIX "[+] Pre-processing GLSL: {0} [+]", **m_AssetPath);

        for (auto &[stage, shaderSource] : vkShaderSources)
        {
            shaderc::CompileOptions options;
            options.AddMacroDefinition("__GLSL__");
            options.AddMacroDefinition("__VULKAN__");
            options.AddMacroDefinition(utils::ShaderStageToMacro(stage).C_Str());

            for (auto &[name, value] : m_Macros)
                options.AddMacroDefinition(*name, *value);

            GLSLIncluder *includer = new GLSLIncluder(&fileFinder);

            options.SetIncluder(std::unique_ptr<GLSLIncluder>(includer));

            const auto preProcessingResult = compiler.PreprocessGlsl(*shaderSource, utils::ShaderStageToShaderC(stage), **m_AssetPath, options);
            if (preProcessingResult.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                HL_CORE_ERROR("Failed to pre-process Shader {0} with error {1}", **m_AssetPath, preProcessingResult.GetErrorMessage());
            }

            m_StagesMetaData[stage].HashValue = shaderSource.Hash();
            m_StagesMetaData[stage].Headers = std::move(includer->GetIncludeData());

            m_AcknowledgedMacros.merge(includer->GetParsedSpecialMacros());

            shaderSource = std::string(preProcessingResult.begin(), preProcessingResult.end());

        #if PRINT_PREPROCESSING_RESULT
            HL_CORE_TRACE("{0}", *shaderSource);
        #endif // PRINT_PREPROCESSING_RESULT
        }

        return vkShaderSources;
    }
    
    std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcessHLSL(const HLString &source)
    {
        std::unordered_map<VkShaderStageFlagBits, HLString> result;
        // TODO
        return result;
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
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("===========================");
        HL_CORE_TRACE(" Vulkan Shader Reflection ");
        HL_CORE_TRACE("===========================");
    #endif // PRINT_DEBUG_OUTPUTS

        spirv_cross::Compiler compiler(shaderData);
        auto &resources = compiler.get_shader_resources();

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Uniform Buffers: {0}", resources.uniform_buffers.size());
    #endif // PRINT_DEBUG_OUTPUTS
        for (const auto &resource : resources.uniform_buffers)
        {
            auto &activeBuffers = compiler.get_active_buffer_ranges(resource.id);
            if (activeBuffers.size())
            {
                const auto &name = resource.name;
                auto &bufferType = compiler.get_type(resource.base_type_id);
                uint32 memberCount = (uint32)bufferType.member_types.size();
                uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
                uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
                uint32 size = (uint32)compiler.get_declared_struct_size(bufferType);

                if (descriptorSet >= m_ShaderDescriptorSets.size())
                    m_ShaderDescriptorSets.resize(descriptorSet + 1);

                VulkanShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
                if (s_UniformBuffers[descriptorSet].find(binding) == s_UniformBuffers[descriptorSet].end())
                {
                    VulkanShaderUniformBuffer *uniformBuffer = new VulkanShaderUniformBuffer();
                    uniformBuffer->BindingPoint = binding;
                    uniformBuffer->Size = size;
                    uniformBuffer->Name = name;
                    uniformBuffer->Stage = VK_SHADER_STAGE_ALL;
                    s_UniformBuffers.at(descriptorSet)[binding] = uniformBuffer;
                }
                else
                {
                    VulkanShaderUniformBuffer *uniformBuffer = s_UniformBuffers.at(descriptorSet).at(binding);
                    if (size > uniformBuffer->Size)
                        uniformBuffer->Size = size;
                }

                shaderDescriptorSet.UniformBuffers[binding] = s_UniformBuffers.at(descriptorSet).at(binding);

            #if PRINT_DEBUG_OUTPUTS
                HL_CORE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
                HL_CORE_TRACE("  Member Count: {0}", memberCount);
                HL_CORE_TRACE("  Size: {0}", size);
                HL_CORE_TRACE("-------------------");
            #endif // PRINT_DEBUG_OUTPUTS
            }
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Storage Buffers: {0}", resources.storage_buffers.size());
    #endif // PRINT_DEBUG_OUTPUTS

        for (const auto &resource : resources.storage_buffers)
        {
            auto &activeBuffers = compiler.get_active_buffer_ranges(resource.id);
            if (activeBuffers.size())
            {
                const auto &name = resource.name;
                auto &bufferType = compiler.get_type(resource.base_type_id);
                uint32 memberCount = (uint32)bufferType.member_types.size();
                uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
                uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
                uint32 size = (uint32)compiler.get_declared_struct_size(bufferType);

                if (descriptorSet >= m_ShaderDescriptorSets.size())
                    m_ShaderDescriptorSets.resize(descriptorSet + 1);

                VulkanShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
                if (s_StorageBuffers[descriptorSet].find(binding) == s_StorageBuffers[descriptorSet].end())
                {
                    VulkanShaderStorageBuffer *storageBuffer = new VulkanShaderStorageBuffer();
                    storageBuffer->Name = name;
                    storageBuffer->Size = size;
                    storageBuffer->BindingPoint = binding;
                    storageBuffer->Stage = VK_SHADER_STAGE_ALL;
                    s_StorageBuffers.at(descriptorSet)[binding] = storageBuffer;
                }
                else
                {
                    VulkanShaderStorageBuffer *storageBuffer = s_StorageBuffers.at(descriptorSet).at(binding);
                    if (size > storageBuffer->Size)
                        storageBuffer->Size = size;
                }

                shaderDescriptorSet.StorageBuffers[binding] = s_StorageBuffers.at(descriptorSet).at(binding);

            #if PRINT_DEBUG_OUTPUTS
                HL_CORE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
                HL_CORE_TRACE("  Member Count: {0}", memberCount);
                HL_CORE_TRACE("  Size: {0}", size);
                HL_CORE_TRACE("-------------------");
            #endif // PRINT_DEBUG_OUTPUTS
            }
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Push Constant Buffers: {0}", resources.push_constant_buffers.size());
    #endif // PRINT_DEBUG_OUTPUTS

        for (const auto &resource : resources.push_constant_buffers)
        {
            const auto &bufferName = resource.name;
            auto &bufferType = compiler.get_type(resource.type_id);
            uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32 bufferSize = (uint32)compiler.get_declared_struct_size(bufferType);
            uint32 memberCount = (uint32)bufferType.member_types.size();
            uint32 bufferOffset = 0;
            if (m_PushConstantRanges.size())
                bufferOffset = m_PushConstantRanges.back().Offset + m_PushConstantRanges.back().Size;

            auto &pushConstantRange = m_PushConstantRanges.emplace_back();
            pushConstantRange.Stage = shaderStage;
            pushConstantRange.Size = bufferSize - bufferOffset;
            pushConstantRange.Offset = bufferOffset;

            if (bufferName.empty() || bufferName == "u_Renderer")
                continue;

            ShaderBuffer &shaderBuffer = m_Buffers[bufferName];
            shaderBuffer.Name = bufferName;
            shaderBuffer.Size = bufferSize - bufferOffset;

        #if PRINT_DEBUG_OUTPUTS
            HL_CORE_TRACE("  Name: {0}", bufferName);
            HL_CORE_TRACE("  Member Count: {0}", memberCount);
            HL_CORE_TRACE("  Size: {0}", bufferSize);
        #endif // PRINT_DEBUG_OUTPUTS

            for (uint32 i = 0; i < memberCount; ++i)
            {
                auto &type = compiler.get_type(bufferType.member_types[i]);
                const auto &memberName = compiler.get_member_name(bufferType.self, i);
                uint32 size = (uint32)compiler.get_declared_struct_member_size(bufferType, i);
                uint32 offset = (uint32)compiler.type_struct_member_offset(bufferType, i) - bufferOffset;

                HLString uniformName = fmt::format("{}.{}", bufferName, memberName);
                shaderBuffer.Uniforms[uniformName] = ShaderUniform(uniformName, binding, utils::SPIRTypeToShaderUniformType(type), size, offset);
            }
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Sampled Images: {0}", resources.sampled_images.size());
    #endif // PRINT_DEBUG_OUTPUTS

        for (const auto &resource : resources.sampled_images)
        {
            const auto &name = resource.name;
            auto &baseType = compiler.get_type(resource.base_type_id);
            auto &type = compiler.get_type(resource.type_id);
            uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32 arraySize = (uint32)type.array[0];
            
            if (arraySize == 0)
                arraySize = 1;

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            VulkanShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];

            auto &imageSampler = shaderDescriptorSet.ImageSamplers[binding];
            imageSampler.BindingPoint = binding;
            imageSampler.ArraySize = arraySize;
            imageSampler.DescriptorSet = descriptorSet;
            imageSampler.Stage = shaderStage;
            imageSampler.Name = name;

            m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

        #if PRINT_DEBUG_OUTPUTS
            HL_CORE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
        #endif // PRINT_DEBUG_OUTPUTS
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Separate Images: {0}", resources.separate_images.size());
    #endif // PRINT_DEBUG_OUTPUTS

        for (const auto &resource : resources.separate_images)
        {
            const auto &name = resource.name;
            auto &baseType = compiler.get_type(resource.base_type_id);
            auto &type = compiler.get_type(resource.type_id);
            uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32 arraySize = (uint32)type.array[0];

            if (arraySize == 0)
                arraySize = 1;

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            VulkanShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            
            auto &imageSampler = shaderDescriptorSet.SeparateTextures[binding];
            imageSampler.BindingPoint = binding;
            imageSampler.ArraySize = arraySize;
            imageSampler.DescriptorSet = descriptorSet;
            imageSampler.Name = name;
            imageSampler.Stage = shaderStage;

            m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

        #if PRINT_DEBUG_OUTPUTS
            HL_CORE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
        #endif // PRINT_DEBUG_OUTPUTS
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Separate Samplers: {0}", resources.separate_samplers.size());
    #endif // PRINT_DEBUG_OUTPUTS

        for (const auto &resource : resources.separate_samplers)
        {
            const auto &name = resource.name;
            auto &baseType = compiler.get_type(resource.base_type_id);
            auto &type = compiler.get_type(resource.type_id);
            uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32 arraySize = (uint32)type.array[0];

            if (arraySize == 0)
                arraySize = 1;

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            VulkanShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            
            auto &imageSampler = shaderDescriptorSet.SeparateSamplers[binding];
            imageSampler.ArraySize = arraySize;
            imageSampler.Name = name;
            imageSampler.BindingPoint = binding;
            imageSampler.Stage = shaderStage;
            imageSampler.DescriptorSet = descriptorSet;

            m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

        #if PRINT_DEBUG_OUTPUTS
            HL_CORE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
        #endif // PRINT_DEBUG_OUTPUTS
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Storage Images: {0}", resources.storage_images.size());
    #endif // PRINT_DEBUG_OUTPUTS

        for (const auto &resource : resources.storage_images)
        {
            const auto &name = resource.name;
            auto &type = compiler.get_type(resource.type_id);
            uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32 arraySize = (uint32)type.array[0];

            if (arraySize == 0)
                arraySize = 1;

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            VulkanShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            
            auto &imageSampler = shaderDescriptorSet.StorageImages[binding];
            imageSampler.ArraySize = arraySize;
            imageSampler.BindingPoint = binding;
            imageSampler.DescriptorSet = descriptorSet;
            imageSampler.Name = name;
            imageSampler.Stage = shaderStage;

            m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

        #if PRINT_DEBUG_OUTPUTS
            HL_CORE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
        #endif // PRINT_DEBUG_OUTPUTS
        }

    #if PRINT_DEBUG_OUTPUTS
        HL_CORE_TRACE("Special macros: {0}", m_AcknowledgedMacros.size());
        for (const auto &macro : m_AcknowledgedMacros)
        {
            HL_CORE_TRACE("{0}", *macro);
        }

        HL_CORE_TRACE("===========================");
    #endif // PRINT_DEBUG_OUTPUTS
    }

    HLString VulkanShader::Compile(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, const VkShaderStageFlagBits stage) const
    {
        const HLString &stageSource = m_ShaderSources.at(stage);

        if (m_Language == ShaderLanguage::GLSL)
        {
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
            options.SetWarningsAsErrors();
            options.SetGenerateDebugInfo();

            bool optimize = false;
        #ifdef HL_RELEASE
            optimize = true;
        #endif // HL_RELEASE

            if (optimize)
                options.SetOptimizationLevel(shaderc_optimization_level_performance);

            // Compile shader
            const shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(*stageSource, utils::ShaderStageToShaderC(stage), **m_AssetPath, options);

            if (result.GetCompilationStatus() != shaderc_compilation_status_success)
                return fmt::format("Shader compilation error: {} ({})", result.GetErrorMessage(), **m_AssetPath).c_str();

            outputBinary[stage] = std::vector<uint32>(result.begin(), result.end());
            return {};
        }
        else if (m_Language == ShaderLanguage::HLSL)
        {
            // TODO
        }

        return "unknown Language!";
    }

    void VulkanShader::CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, bool forceCompile)
    {
        FileSystemPath cacheDirectory = utils::GetCacheDirectory();

        for (auto &[stage, source] : m_ShaderSources)
        {
            if (source.IsEmpty())
                continue;

            bool shaderCacheHasChanged = ShaderCache::HasChanged(m_AssetPath, source);
            const HLString &extension = utils::ShaderStageCachedFileExtension(stage);
            if (!forceCompile && !shaderCacheHasChanged)
            {
                TryGetVulkanCachedBinary(cacheDirectory, extension, outBinary, stage);
            }

            if (outBinary[stage].empty())
            {
                HLString error = Compile(outBinary, stage);
                if (error.IsEmpty())
                {
                    // Compile success
                    FileSystemPath p = cacheDirectory / (m_AssetPath.Filename() + extension);
                    
                    FILE *f;
                    errno_t error = fopen_s(&f, **p, "wb");
                    if (error)
                        HL_CORE_ERROR("Failed to cache shader binary!");

                    fwrite(outBinary[stage].data(), sizeof(uint32), outBinary[stage].size(), f);
                    fclose(f);
                }
                else
                {
                    HL_CORE_ERROR("{0}", *error);

                    TryGetVulkanCachedBinary(cacheDirectory, extension, outBinary, stage);
                    if (outBinary[stage].empty())
                    {
                        HL_ASSERT(false, "Failed to compile shader and could not find any cached binary");
                    }
                    else
                    {
                        HL_CORE_ERROR("Failed to compile {0}:{1}, so loaded a cached shader binary instead.", **m_AssetPath, *utils::ShaderStageToString(stage));
                    }
                }
            }
        }
    }
    
    void VulkanShader::TryGetVulkanCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, VkShaderStageFlagBits stage) const
    {
        FileSystemPath p = cacheDirectory / (m_AssetPath.Filename() + extension);

        FILE *f;
        errno_t err = fopen_s(&f, **p, "rb");
        if (err)
            return;

        fseek(f, 0, SEEK_END);
        uint64 size = ftell(f);
        fseek(f, 0, SEEK_SET);

        outBinary[stage] = std::vector<uint32>(size / sizeof(uint32));
        fread(outBinary[stage].data(), sizeof(uint32), outBinary[stage].size(), f);
        fclose(f);
    }
    
    void VulkanShader::LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        m_PipelineShaderStageCreateInfos.clear();

        for (auto &[stage, data] : shaderData)
        {
            HL_ASSERT(data.size());

            VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
            shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shaderModuleCreateInfo.codeSize = data.size() * sizeof(uint32);
            shaderModuleCreateInfo.pCode = data.data();

            VkShaderModule shaderModule;
            VK_CHECK_RESULT(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SHADER_MODULE, fmt::format("{}:{}", m_Name, utils::ShaderStageToString(stage)).c_str(), shaderModule);

            VkPipelineShaderStageCreateInfo &shaderStage = m_PipelineShaderStageCreateInfos.emplace_back();
            shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStage.stage = stage;
            shaderStage.module = shaderModule;
            shaderStage.pName = "main";
        }
    }
    
    void VulkanShader::CreateDescriptors()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        m_TypeCounts.clear();

        // Descriptor Pool
        for (uint32 set = 0; set < m_ShaderDescriptorSets.size(); ++set)
        {
            auto &shaderDescriptorSet = m_ShaderDescriptorSets[set];
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

            if (shaderDescriptorSet.UniformBuffers.size())
            {
                VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.UniformBuffers.size();
            }

            if (shaderDescriptorSet.StorageBuffers.size())
            {
                VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageBuffers.size();
            }

            if (shaderDescriptorSet.ImageSamplers.size())
            {
                VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.ImageSamplers.size();
            }

            if (shaderDescriptorSet.SeparateTextures.size())
            {
                VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.SeparateTextures.size();
            }

            if (shaderDescriptorSet.SeparateSamplers.size())
            {
                VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLER;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.SeparateSamplers.size();
            }

            if (shaderDescriptorSet.StorageImages.size())
            {
                VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageImages.size();
            }

            // Descriptor Set Layout
            for (auto &[binding, uniformBuffer] : shaderDescriptorSet.UniformBuffers)
            {
                VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = uniformBuffer->Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[uniformBuffer->Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = 1;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto &[binding, storageBuffer] : shaderDescriptorSet.StorageBuffers)
            {
                VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = storageBuffer->Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already taken!");

                VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[storageBuffer->Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorCount = 1;
                set.descriptorType = layoutBinding.descriptorType;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto &[binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
            {
                VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                layoutBinding.descriptorCount = imageSampler.ArraySize;
                layoutBinding.stageFlags = imageSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already taken!");

                VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = imageSampler.ArraySize;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto &[binding, imageSampler] : shaderDescriptorSet.SeparateTextures)
            {
                VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                layoutBinding.descriptorCount = imageSampler.ArraySize;
                layoutBinding.stageFlags = imageSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already taken!");

                VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = imageSampler.ArraySize;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto &[binding, imageSampler] : shaderDescriptorSet.SeparateSamplers)
            {
                VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                layoutBinding.descriptorCount = imageSampler.ArraySize;
                layoutBinding.stageFlags = imageSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.SeparateTextures.find(binding) == shaderDescriptorSet.SeparateTextures.end(), "Binding is already taken!");

                VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = imageSampler.ArraySize;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto &[bindingAndSet, imageSampler] : shaderDescriptorSet.StorageImages)
            {
                VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                layoutBinding.descriptorCount = imageSampler.ArraySize;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = imageSampler.Stage;

                uint32 binding = bindingAndSet & 0xffffffff;
                layoutBinding.binding = binding;

                HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.SeparateTextures.find(binding) == shaderDescriptorSet.SeparateTextures.end(), "Binding is already taken!");
                HL_ASSERT(shaderDescriptorSet.SeparateSamplers.find(binding) == shaderDescriptorSet.SeparateSamplers.end(), "Binding is already taken!");

                VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorCount = 1;
                set.descriptorType = layoutBinding.descriptorType;
                set.dstBinding = layoutBinding.binding;
            }

            VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
            descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorLayout.pNext = nullptr;
            descriptorLayout.bindingCount = (uint32)layoutBindings.size();
            descriptorLayout.pBindings = layoutBindings.data();

            if (set >= m_DescriptorSetLayouts.size())
                m_DescriptorSetLayouts.resize((uint64)(set + 1));

            VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &m_DescriptorSetLayouts[set]));
        }
    }
}

#endif // HIGHLO_API_VULKAN

