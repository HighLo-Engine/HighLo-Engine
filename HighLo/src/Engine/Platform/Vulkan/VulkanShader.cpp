// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/ShaderUtils.h"

#include "Engine/Renderer/Renderer.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanRenderingAPI.h"

#include "Engine/Graphics/Shaders/ShaderCache.h"

#include <shaderc/shaderc.hpp>
#include <libshaderc_util/file_finder.h>

#include "Engine/Graphics/Shaders/GLSLIncluder.h"

namespace highlo
{
	namespace utils
	{
		static FileSystemPath GetCacheDirectory()
		{
			FileSystemPath path = "assets/cache/shaders/Vulkan/";

			if (!FileSystem::Get()->FolderExists(path))
				FileSystem::Get()->CreateFolder(path);

			return path;
		}

		static HLString ShaderStageToMacro(const VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:					return "__VERTEX_STAGE__";
				case VK_SHADER_STAGE_FRAGMENT_BIT:					return "__FRAGMENT_STAGE__";
				case VK_SHADER_STAGE_COMPUTE_BIT:					return "__COMPUTE_STAGE__";
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return "__TESS_CONTROL_STAGE__";
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return "__TESS_EVAL_STAGE__";
				case VK_SHADER_STAGE_GEOMETRY_BIT:					return "__GEOMETRY_STAGE__";
			}

			HL_ASSERT(false);
			return "";
		}

		static HLString ShaderStageToString(const VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:					return "vertex";
				case VK_SHADER_STAGE_FRAGMENT_BIT:					return "fragment";
				case VK_SHADER_STAGE_COMPUTE_BIT:					return "compute";
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return "tess_control";
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return "tess_eval";
				case VK_SHADER_STAGE_GEOMETRY_BIT:					return "geometry";
			}

			HL_ASSERT(false);
			return "";
		}

		static VkShaderStageFlagBits VulkanShaderTypeFromShaderStage(const ShaderType type)
		{
			switch (type)
			{
				case ShaderType::Vertex:
					return VK_SHADER_STAGE_VERTEX_BIT;

				case ShaderType::Fragment:
					return VK_SHADER_STAGE_FRAGMENT_BIT;

				case ShaderType::Compute:
					return VK_SHADER_STAGE_COMPUTE_BIT;

				case ShaderType::TessControl:
					return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

				case ShaderType::TessEvaluation:
					return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

				case ShaderType::Geometry:
					return VK_SHADER_STAGE_GEOMETRY_BIT;
			}

			HL_ASSERT(false);
			return VK_SHADER_STAGE_ALL;
		}

		static shaderc_shader_kind VulkanShaderStageToShaderC(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:					return shaderc_vertex_shader;
				case VK_SHADER_STAGE_FRAGMENT_BIT:					return shaderc_fragment_shader;
				case VK_SHADER_STAGE_COMPUTE_BIT:					return shaderc_compute_shader;
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return shaderc_tess_control_shader;
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return shaderc_tess_evaluation_shader;
				case VK_SHADER_STAGE_GEOMETRY_BIT:					return shaderc_geometry_shader;
			}

			HL_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static HLString ShaderStageCachedFileExtension(const VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:					return ".cached_opengl.vert";
				case VK_SHADER_STAGE_FRAGMENT_BIT:					return ".cached_opengl.frag";
				case VK_SHADER_STAGE_COMPUTE_BIT:					return ".cached_opengl.comp";
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return ".cached_opengl.tessconn";
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return ".cached_opengl.tessval";
				case VK_SHADER_STAGE_GEOMETRY_BIT:					return ".cached_opengl.geo";
			}

			HL_ASSERT(false);
			return "";
		}

		static ShaderUniformType SpirvTypeToShaderUniformType(spirv_cross::SPIRType type)
		{
			switch (type.basetype)
			{
			case spirv_cross::SPIRType::Struct:		return ShaderUniformType::Struct;
			case spirv_cross::SPIRType::Boolean:	return ShaderUniformType::Bool;
			case spirv_cross::SPIRType::UInt:		return ShaderUniformType::Uint;
			case spirv_cross::SPIRType::Int:
			{
				if (type.vecsize == 1)				return ShaderUniformType::Int;
				if (type.vecsize == 2)				return ShaderUniformType::IVec2;
				if (type.vecsize == 3)				return ShaderUniformType::IVec3;
				if (type.vecsize == 4)				return ShaderUniformType::IVec4;
				break;
			}

			case spirv_cross::SPIRType::Float:
			{
				if (type.columns == 2)				return ShaderUniformType::Mat2;
				if (type.columns == 3)				return ShaderUniformType::Mat3;
				if (type.columns == 4)				return ShaderUniformType::Mat4;

				if (type.vecsize == 1)				return ShaderUniformType::Float;
				if (type.vecsize == 2)				return ShaderUniformType::Vec2;
				if (type.vecsize == 3)				return ShaderUniformType::Vec3;
				if (type.vecsize == 4)				return ShaderUniformType::Vec4;
				break;
			}
			}

			HL_ASSERT(false);
			return ShaderUniformType::None;
		}

		static std::unordered_map<VkShaderStageFlagBits, HLString> ConvertShaderTypeToVulkanStage(const std::unordered_map<ShaderType, HLString> &shaders)
		{
			std::unordered_map<VkShaderStageFlagBits, HLString> result;
			for (auto &[stage, source] : shaders)
			{
				result.insert({ utils::VulkanShaderTypeFromShaderStage(stage), source });
			}

			return result;
		}
	}

	static std::unordered_map<uint32, std::unordered_map<uint32, VkShaderResource::UniformBuffer*>> s_UniformBuffers;
	static std::unordered_map<uint32, std::unordered_map<uint32, VkShaderResource::StorageBuffer*>> s_StorageBuffers;

	VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
		: m_FilePath(filePath)
	{
		m_Name = filePath.Filename();
		m_Language = utils::ShaderLanguageFromExtension(filePath.Extension());
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

		m_Loaded = false;
		m_IsCompute = false;
		m_Name.Clear();
		m_Language = ShaderLanguage::None;
		m_ConstantBufferOffset = 0;

		m_Macros.clear();
		m_AcknowledgedMacros.clear();

		m_ShaderSources.clear();
		m_ReloadedCallbacks.clear();

		m_StagesMetaData.clear();

		m_ReflectionData.ConstantBuffers.clear();
		m_ReflectionData.PushConstantRanges.clear();
		m_ReflectionData.Resources.clear();
		m_ReflectionData.ShaderDescriptorSets.clear();

		m_PipelineShaderStageCreateInfos.clear();
		m_DescriptorSetLayouts.clear();
		m_TypeCounts.clear();
	}

	void VulkanShader::Reload(bool forceCompile)
	{
		HL_CORE_INFO("[+] Reloading shader {0}... [+]", **m_FilePath);
		m_Loaded = false; // Reflect current stage: Shader is being reloaded
		
		if (!FileSystem::Get()->FileExists(m_FilePath))
		{
			return;
		}

		HLString source = FileSystem::Get()->ReadTextFile(m_FilePath);
		Load(source, forceCompile);

		Renderer::OnShaderReloaded(GetHash());

		for (ShaderReloadedCallback callback : m_ReloadedCallbacks)
			callback();
	}

	void VulkanShader::Release()
	{
		VkDevice device = VulkanContext::Get()->GetDevice()->GetVulkanDevice();
		auto &pipelineCIs = m_PipelineShaderStageCreateInfos;
		Renderer::SubmitWithoutResources([device, pipelineCIs]()
		{
			for (const auto &ci : pipelineCIs)
				if (ci.module)
					vkDestroyShaderModule(device, ci.module, nullptr);
		});

		for (auto &ci : pipelineCIs)
			ci.module = nullptr;
	}

	void VulkanShader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
	{
		m_ReloadedCallbacks.push_back(callback);
	}

	const ShaderResourceDeclaration *VulkanShader::GetResource(const HLString &name) const
	{
		if (m_ReflectionData.Resources.find(name) == m_ReflectionData.Resources.end())
		{
			return nullptr;
		}

		return &m_ReflectionData.Resources.at(name);
	}

	void VulkanShader::SetMacro(const HLString &name, const HLString &value)
	{
		m_Macros[name] = value;
	}

	std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
	{
		std::vector<VkDescriptorSetLayout> result;
		result.reserve(m_DescriptorSetLayouts.size());

		for (auto &layout : m_DescriptorSetLayouts)
			result.emplace_back(layout);

		return result;
	}

	VkShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32 set)
	{
		HL_ASSERT(set < m_DescriptorSetLayouts.size());
		VkShaderMaterialDescriptorSet result;

		if (m_ReflectionData.ShaderDescriptorSets.empty())
			return result;

		// TODO: remove
		result.Pool = nullptr;

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];
		VkDescriptorSet descriptorSet = VulkanRenderingAPI::AllocateDescriptorSet(allocInfo);
		HL_ASSERT(descriptorSet);
		result.DescriptorSets.push_back(descriptorSet);
		return result;
	}

	VkShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set)
	{
		VkShaderMaterialDescriptorSet result;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		HL_ASSERT(m_TypeCounts.find(set) != m_TypeCounts.end());

		// TODO: Move this to the centralized renderer
		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.poolSizeCount = (uint32)m_TypeCounts.at(set).size();
		descriptorPoolInfo.pPoolSizes = m_TypeCounts.at(set).data();
		descriptorPoolInfo.maxSets = 1;

		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &result.Pool));

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = result.Pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

		result.DescriptorSets.emplace_back();
		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, result.DescriptorSets.data()));
		return result;
	}

	VkShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set, uint32 numberOfSets)
	{
		VkShaderMaterialDescriptorSet result;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		std::unordered_map<uint32, std::vector<VkDescriptorPoolSize>> poolSizes;
		for (uint32 set = 0; set < m_ReflectionData.ShaderDescriptorSets.size(); set++)
		{
			auto &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[set];
			if (!shaderDescriptorSet) // Empty descriptor set
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
			if (shaderDescriptorSet.SeparateTextures.size())
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

		// TODO: Move this to the centralized renderer
		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.poolSizeCount = (uint32)poolSizes.at(set).size();
		descriptorPoolInfo.pPoolSizes = poolSizes.at(set).data();
		descriptorPoolInfo.maxSets = numberOfSets;

		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &result.Pool));

		result.DescriptorSets.resize(numberOfSets);

		for (uint32 i = 0; i < numberOfSets; i++)
		{
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = result.Pool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

			VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result.DescriptorSets[i]));
		}
		return result;
	}

	const VkWriteDescriptorSet *VulkanShader::GetDescriptorSet(const HLString &name, uint32 set) const
	{
		HL_ASSERT(set < m_ReflectionData.ShaderDescriptorSets.size());
		HL_ASSERT(m_ReflectionData.ShaderDescriptorSets[set]);

		if (m_ReflectionData.ShaderDescriptorSets.at(set).WriteDescriptorSets.find(name) == m_ReflectionData.ShaderDescriptorSets.at(set).WriteDescriptorSets.end())
		{
			HL_CORE_WARN("Shader {0} does not contain requested descriptor set {1}", *m_Name, *name);
			return nullptr;
		}

		return &m_ReflectionData.ShaderDescriptorSets.at(set).WriteDescriptorSets.at(name);
	}

	void VulkanShader::Load(const HLString &source, bool forceCompile)
	{
		if (FileSystem::Get()->FileExists(m_FilePath))
		{
			HL_CORE_TRACE("[+] Trying to create shader {0}... [+]", **m_FilePath);

			bool shaderCacheHasChanged = ShaderCache::HasChanged(m_FilePath, source);

			m_ShaderSources = PreProcess(source);
			std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> shaderData;
			CompileOrGetVulkanBinary(shaderData, forceCompile || shaderCacheHasChanged);
			LoadAndCreateShaders(shaderData);
			ReflectAllShaderStages(shaderData);

			HL_CORE_INFO("[+] Shader {0} loaded [+]", **m_FilePath);
			m_Loaded = true;
		}
		else
		{
			HL_CORE_WARN("[-] Shader {0} not found! [-]", **m_FilePath);
		}
	}

	HLString VulkanShader::Compile(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, const VkShaderStageFlagBits stage) const
	{
		return HLString();
	}

	void VulkanShader::CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, bool forceCompile)
	{
		FileSystemPath cacheDirectory = utils::GetCacheDirectory();

		for (auto &[stage, source] : m_ShaderSources)
		{
			if (source.IsEmpty())
				continue;

			bool shaderCacheHasChanged = ShaderCache::HasChanged(m_FilePath, source);
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
					FileSystemPath p = cacheDirectory / (m_FilePath.Filename() + extension);

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
						HL_CORE_ERROR("Failed to compile {0}:{1}, so loaded a cached shader binary instead.", **m_FilePath, *utils::ShaderStageToString(stage));
					}
				}
			}
		}
	}

	void VulkanShader::TryGetVulkanCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outBinary, VkShaderStageFlagBits stage) const
	{
		FileSystemPath p = cacheDirectory / (m_FilePath.Filename() + extension);

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
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
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
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		//////////////////////////////////////////////////////////////////////
		// Descriptor Pool
		//////////////////////////////////////////////////////////////////////

		m_TypeCounts.clear();
		for (uint32 set = 0; set < m_ReflectionData.ShaderDescriptorSets.size(); set++)
		{
			auto &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[set];

			if (shaderDescriptorSet.UniformBuffers.size())
			{
				VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				typeCount.descriptorCount = (uint32)(shaderDescriptorSet.UniformBuffers.size());
			}
			if (shaderDescriptorSet.StorageBuffers.size())
			{
				VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				typeCount.descriptorCount = (uint32)(shaderDescriptorSet.StorageBuffers.size());
			}
			if (shaderDescriptorSet.ImageSamplers.size())
			{
				VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				typeCount.descriptorCount = (uint32)(shaderDescriptorSet.ImageSamplers.size());
			}
			if (shaderDescriptorSet.SeparateTextures.size())
			{
				VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				typeCount.descriptorCount = (uint32)(shaderDescriptorSet.SeparateTextures.size());
			}
			if (shaderDescriptorSet.SeparateSamplers.size())
			{
				VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLER;
				typeCount.descriptorCount = (uint32)(shaderDescriptorSet.SeparateSamplers.size());
			}
			if (shaderDescriptorSet.StorageImages.size())
			{
				VkDescriptorPoolSize &typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				typeCount.descriptorCount = (uint32)(shaderDescriptorSet.StorageImages.size());
			}

			//////////////////////////////////////////////////////////////////////
			// Descriptor Set Layout
			//////////////////////////////////////////////////////////////////////


			std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
			for (auto &[binding, uniformBuffer] : shaderDescriptorSet.UniformBuffers)
			{
				VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = uniformBuffer.Stage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[uniformBuffer.Name];
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
				layoutBinding.stageFlags = storageBuffer.Stage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;
				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[storageBuffer.Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = 1;
				set.dstBinding = layoutBinding.binding;
			}

			for (auto &[binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
			{
				auto &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				layoutBinding.descriptorCount = imageSampler.ArraySize;
				layoutBinding.stageFlags = imageSampler.Stage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already present!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = imageSampler.ArraySize;
				set.dstBinding = layoutBinding.binding;
			}

			for (auto &[binding, imageSampler] : shaderDescriptorSet.SeparateTextures)
			{
				auto &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				layoutBinding.descriptorCount = imageSampler.ArraySize;
				layoutBinding.stageFlags = imageSampler.Stage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already present!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = imageSampler.ArraySize;
				set.dstBinding = layoutBinding.binding;
			}

			for (auto &[binding, imageSampler] : shaderDescriptorSet.SeparateSamplers)
			{
				auto &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				layoutBinding.descriptorCount = imageSampler.ArraySize;
				layoutBinding.stageFlags = imageSampler.Stage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.SeparateTextures.find(binding) == shaderDescriptorSet.SeparateTextures.end(), "Binding is already present!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = imageSampler.ArraySize;
				set.dstBinding = layoutBinding.binding;
			}

			for (auto &[bindingAndSet, imageSampler] : shaderDescriptorSet.StorageImages)
			{
				auto &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				layoutBinding.descriptorCount = imageSampler.ArraySize;
				layoutBinding.stageFlags = imageSampler.Stage;
				layoutBinding.pImmutableSamplers = nullptr;

				uint32 binding = bindingAndSet & 0xffffffff;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.SeparateTextures.find(binding) == shaderDescriptorSet.SeparateTextures.end(), "Binding is already present!");
				HL_ASSERT(shaderDescriptorSet.SeparateSamplers.find(binding) == shaderDescriptorSet.SeparateSamplers.end(), "Binding is already present!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = 1;
				set.dstBinding = layoutBinding.binding;
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
			descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorLayout.pNext = nullptr;
			descriptorLayout.bindingCount = (uint32)(layoutBindings.size());
			descriptorLayout.pBindings = layoutBindings.data();

			HL_CORE_INFO("Creating descriptor set {0} with {1} ubo's, {2} ssbo's, {3} samplers, {4} separate textures, {5} separate samplers and {6} storage images", set,
							 shaderDescriptorSet.UniformBuffers.size(),
							 shaderDescriptorSet.StorageBuffers.size(),
							 shaderDescriptorSet.ImageSamplers.size(),
							 shaderDescriptorSet.SeparateTextures.size(),
							 shaderDescriptorSet.SeparateSamplers.size(),
							 shaderDescriptorSet.StorageImages.size());
			if (set >= m_DescriptorSetLayouts.size())
				m_DescriptorSetLayouts.resize((size_t)(set + 1));
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &m_DescriptorSetLayouts[set]));
		}
	}

	std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcess(const HLString &source)
	{
		std::unordered_map<VkShaderStageFlagBits, HLString> result;

		switch (m_Language)
		{
			case ShaderLanguage::GLSL:
				result = PreProcessGLSL(source);
				break;

			case ShaderLanguage::HLSL:
				result = PreProcessHLSL(source);
				break;
		}

		return result;
	}

	std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcessGLSL(const HLString &source)
	{
		std::unordered_map<ShaderType, HLString> shaderSources = ShaderPreProcessor::PreprocessShader<ShaderLanguage::GLSL>(source, m_AcknowledgedMacros);
		std::unordered_map<VkShaderStageFlagBits, HLString> vkShaderSources = utils::ConvertShaderTypeToVulkanStage(shaderSources);
		shaderc::Compiler compiler;

		// If any shader source is compute and the shader source count is 1, the whole shader is considered a compute shader
		if (shaderSources.size() == 1)
		{
			for (auto &[type, shaderSource] : shaderSources)
			{
				if (type == ShaderType::Compute)
				{
					m_IsCompute = true;
					break;
				}
			}
		}

		HL_CORE_TRACE("[+]     Pre-processing GLSL: {0} [+]", **m_FilePath);

		shaderc_util::FileFinder fileFinder;
		fileFinder.search_path().emplace_back("assets/shaders/Include/GLSL/");
		fileFinder.search_path().emplace_back("assets/shaders/Include/Shared/");

		for (auto &[stage, shaderSource] : vkShaderSources)
		{
			shaderc::CompileOptions options;

			if (m_Language == ShaderLanguage::GLSL)
				options.AddMacroDefinition("__GLSL__");
			else if (m_Language == ShaderLanguage::HLSL)
				options.AddMacroDefinition("__HLSL__");

			options.AddMacroDefinition("__OPENGL__");
			options.AddMacroDefinition("__GPU_IS_DEDICATED__"); // TODO: make configurable via Renderer::Options
			options.AddMacroDefinition(utils::ShaderStageToMacro(stage).C_Str());

			for (auto &[name, value] : m_Macros)
				options.AddMacroDefinition(*name, *value);

			GLSLIncluder *includer = new GLSLIncluder(&fileFinder);

			options.SetIncluder(std::unique_ptr<GLSLIncluder>(includer));

			shaderc::PreprocessedSourceCompilationResult preProcessingResult = compiler.PreprocessGlsl(*shaderSource, utils::VulkanShaderStageToShaderC(stage), **m_FilePath, options);
			if (preProcessingResult.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				HL_CORE_ERROR("[-]     Failed to pre-process Shader {0} with error {1} [-]", **m_FilePath, preProcessingResult.GetErrorMessage());
				HL_ASSERT(false);
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
		// TODO
		std::unordered_map<VkShaderStageFlagBits, HLString> result;
		return result;
	}

	void VulkanShader::ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
	{
		m_ReflectionData.Resources.clear();

		for (auto [stage, data] : shaderData)
		{
			Reflect(stage, data);
		}
	}

	void VulkanShader::Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData)
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

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

				if (descriptorSet >= m_ReflectionData.ShaderDescriptorSets.size())
					m_ReflectionData.ShaderDescriptorSets.resize(descriptorSet + 1);

				VkShaderResource::ShaderDescriptorSet &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[descriptorSet];
				if (s_UniformBuffers[descriptorSet].find(binding) == s_UniformBuffers[descriptorSet].end())
				{
					VkShaderResource::UniformBuffer *uniformBuffer = new VkShaderResource::UniformBuffer();
					uniformBuffer->BindingPoint = binding;
					uniformBuffer->Size = size;
					uniformBuffer->Name = name;
					uniformBuffer->Stage = VK_SHADER_STAGE_ALL;
					s_UniformBuffers.at(descriptorSet)[binding] = uniformBuffer;
				}
				else
				{
					VkShaderResource::UniformBuffer *uniformBuffer = s_UniformBuffers.at(descriptorSet).at(binding);
					if (size > uniformBuffer->Size)
						uniformBuffer->Size = size;
				}

				shaderDescriptorSet.UniformBuffers[binding] = *s_UniformBuffers.at(descriptorSet).at(binding);

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

				if (descriptorSet >= m_ReflectionData.ShaderDescriptorSets.size())
					m_ReflectionData.ShaderDescriptorSets.resize(descriptorSet + 1);

				VkShaderResource::ShaderDescriptorSet &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[descriptorSet];
				if (s_StorageBuffers[descriptorSet].find(binding) == s_StorageBuffers[descriptorSet].end())
				{
					VkShaderResource::StorageBuffer *storageBuffer = new VkShaderResource::StorageBuffer();
					storageBuffer->Name = name;
					storageBuffer->Size = size;
					storageBuffer->BindingPoint = binding;
					storageBuffer->Stage = VK_SHADER_STAGE_ALL;
					s_StorageBuffers.at(descriptorSet)[binding] = storageBuffer;
				}
				else
				{
					VkShaderResource::StorageBuffer *storageBuffer = s_StorageBuffers.at(descriptorSet).at(binding);
					if (size > storageBuffer->Size)
						storageBuffer->Size = size;
				}

				shaderDescriptorSet.StorageBuffers[binding] = *s_StorageBuffers.at(descriptorSet).at(binding);

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
			if (m_ReflectionData.PushConstantRanges.size())
				bufferOffset = m_ReflectionData.PushConstantRanges.back().Offset + m_ReflectionData.PushConstantRanges.back().Size;

			auto &pushConstantRange = m_ReflectionData.PushConstantRanges.emplace_back();
			pushConstantRange.Stage = shaderStage;
			pushConstantRange.Size = bufferSize - bufferOffset;
			pushConstantRange.Offset = bufferOffset;

			if (bufferName.empty() || bufferName == "u_Renderer")
				continue;

			ShaderBuffer &shaderBuffer = m_ReflectionData.ConstantBuffers[bufferName];
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
				shaderBuffer.Uniforms[uniformName] = ShaderUniform(uniformName, binding, utils::SpirvTypeToShaderUniformType(type), size, offset);
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

			if (descriptorSet >= m_ReflectionData.ShaderDescriptorSets.size())
				m_ReflectionData.ShaderDescriptorSets.resize(descriptorSet + 1);

			VkShaderResource::ShaderDescriptorSet &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.ImageSamplers[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.ArraySize = arraySize;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Stage = shaderStage;
			imageSampler.Name = name;

			m_ReflectionData.Resources[name] = ShaderResourceDeclaration(name, binding, 1);

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

			if (descriptorSet >= m_ReflectionData.ShaderDescriptorSets.size())
				m_ReflectionData.ShaderDescriptorSets.resize(descriptorSet + 1);

			VkShaderResource::ShaderDescriptorSet &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.SeparateTextures[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.ArraySize = arraySize;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.Stage = shaderStage;

			m_ReflectionData.Resources[name] = ShaderResourceDeclaration(name, binding, 1);

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

			if (descriptorSet >= m_ReflectionData.ShaderDescriptorSets.size())
				m_ReflectionData.ShaderDescriptorSets.resize(descriptorSet + 1);

			VkShaderResource::ShaderDescriptorSet &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.SeparateSamplers[binding];
			imageSampler.ArraySize = arraySize;
			imageSampler.Name = name;
			imageSampler.BindingPoint = binding;
			imageSampler.Stage = shaderStage;
			imageSampler.DescriptorSet = descriptorSet;

			m_ReflectionData.Resources[name] = ShaderResourceDeclaration(name, binding, 1);

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

			if (descriptorSet >= m_ReflectionData.ShaderDescriptorSets.size())
				m_ReflectionData.ShaderDescriptorSets.resize(descriptorSet + 1);

			VkShaderResource::ShaderDescriptorSet &shaderDescriptorSet = m_ReflectionData.ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.StorageImages[binding];
			imageSampler.ArraySize = arraySize;
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.Stage = shaderStage;

			m_ReflectionData.Resources[name] = ShaderResourceDeclaration(name, binding, 1);

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
}

#endif // HIGHLO_API_VULKAN

