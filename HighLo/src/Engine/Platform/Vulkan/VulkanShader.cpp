// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShader.h"

#include "VulkanRenderingAPI.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv-tools/libspirv.hpp>

#include "VulkanContext.h"
#include "Engine/Renderer/Shaders/ShaderCache.h"
#include "Engine/Core/FileSystem.h"

#ifdef HIGHLO_API_VULKAN

#define PRINT_SHADERS 1
#define GL_SHADER_LOG_PREFIX "Shader>       "

namespace highlo
{
	namespace utils
	{
		static FileSystemPath GetCacheDirectory()
		{
			return "assets/cache/shaders/Vulkan/";
		}

		static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
		{
			switch (type.basetype)
			{
				case spirv_cross::SPIRType::Boolean:
					return ShaderUniformType::Bool;

				case spirv_cross::SPIRType::Int:
				{
					if (type.vecsize == 1)		return ShaderUniformType::Int;
					else if (type.vecsize == 2) return ShaderUniformType::IVec2;
					else if (type.vecsize == 3) return ShaderUniformType::IVec3;
					else if (type.vecsize == 4) return ShaderUniformType::IVec4;

					break;
				}

				case spirv_cross::SPIRType::UInt:
					return ShaderUniformType::Uint;

				case spirv_cross::SPIRType::Float:
				{
					if (type.vecsize == 1)		return ShaderUniformType::Float;
					else if (type.vecsize == 2) return ShaderUniformType::Vec2;
					else if (type.vecsize == 3) return ShaderUniformType::Vec3;
					else if (type.vecsize == 4) return ShaderUniformType::Vec4;

					if (type.columns == 2)		return ShaderUniformType::Mat2;
					else if (type.columns == 3) return ShaderUniformType::Mat3;
					else if (type.columns == 4) return ShaderUniformType::Mat4;

					break;
				}
			}

			HL_ASSERT(false);
			return ShaderUniformType::None;
		}

		static const char *GetCacheFileExtensionBasedOnShaderType(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:					return ".cached_vulkan.vert";
				case VK_SHADER_STAGE_FRAGMENT_BIT:					return ".cached_vulkan.frag";
				case VK_SHADER_STAGE_COMPUTE_BIT:					return ".cached_vulkan.comp";
				case VK_SHADER_STAGE_GEOMETRY_BIT:					return ".cached_vulkan.geo";
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return ".cached_vulkan.tesscon";
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return ".cached_vulkan.tessval";
			}

			HL_ASSERT(false);
			return "";
		}

		static shaderc_shader_kind VulkanShaderStageToShaderCShaderKind(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:					return shaderc_vertex_shader;
				case VK_SHADER_STAGE_FRAGMENT_BIT:					return shaderc_fragment_shader;
				case VK_SHADER_STAGE_COMPUTE_BIT:					return shaderc_compute_shader;
				case VK_SHADER_STAGE_GEOMETRY_BIT:					return shaderc_geometry_shader;
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return shaderc_tess_control_shader;
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return shaderc_tess_evaluation_shader;
			}

			HL_ASSERT(false);
			return (shaderc_shader_kind)0;
		}
	}

	static std::unordered_map<uint32, std::unordered_map<uint32, VulkanShader::UniformBuffer*>> s_UniformBuffers; // set -> binding point -> buffer
	static std::unordered_map<uint32, std::unordered_map<uint32, VulkanShader::StorageBuffer*>> s_StorageBuffers; // set -> binding point -> buffer

	VulkanShader::VulkanShader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
		HLString source = FileSystem::Get()->ReadTextFile(filePath);
		Load(source, forceCompile);
	}

	VulkanShader::VulkanShader(const HLString &source)
	{
		Load(source, true);
	}

	VulkanShader::~VulkanShader()
	{
	}

	void VulkanShader::Reload(bool forceCompile)
	{
		HL_CORE_TRACE("Reloading shader {0}...", **m_AssetPath);

		// Clear old shader
		m_ShaderDescriptorSets.clear();
		m_Resources.clear();
		m_PushConstantRanges.clear();
		m_PipelineShaderStageCreateInfos.clear();
		m_DescriptorSetLayouts.clear();
		m_ShaderSource.clear();
		m_Buffers.clear();
		m_TypeCounts.clear();

		HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
		Load(source, forceCompile);

		Renderer::OnShaderReloaded(GetHash());

		for (ShaderReloadedCallback &callback : m_ReloadedCallbacks)
			callback();
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

	std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
	{
		std::vector<VkDescriptorSetLayout> result;

		result.reserve(m_DescriptorSetLayouts.size());
		for (auto &layout : m_DescriptorSetLayouts)
			result.emplace_back(layout);

		return result;
	}
	
	VulkanShader::UniformBuffer &VulkanShader::GetUniformBuffer(uint32 binding, uint32 set)
	{
		HL_ASSERT(m_ShaderDescriptorSets.at(set).UniformBuffers.size() > binding);
		return *m_ShaderDescriptorSets.at(set).UniformBuffers.at(binding);
	}
	
	uint32 VulkanShader::GetUniformBufferCount(uint32 set)
	{
		if (m_ShaderDescriptorSets.size() < set)
			return 0;

		return (uint32)m_ShaderDescriptorSets[set].UniformBuffers.size();
	}
	
	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32 set)
	{
		HL_ASSERT(set < m_DescriptorSetLayouts.size());
		ShaderMaterialDescriptorSet result;
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		if (m_ShaderDescriptorSets.empty())
			return result;

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];
		
		VkDescriptorSet descriptorSet = VulkanRenderingAPI::AllocateDescriptorSet(allocInfo);
		HL_ASSERT(descriptorSet);

		result.Pool = nullptr;
		result.DescriptorSets.push_back(descriptorSet);
		return result;
	}
	
	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set)
	{
		ShaderMaterialDescriptorSet result;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		HL_ASSERT(m_TypeCounts.find(set) != m_TypeCounts.end());

		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.poolSizeCount = (uint32)m_TypeCounts.at(set).size();
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
	
	void VulkanShader::CreateDescriptors()
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		m_TypeCounts.clear();
		for (uint32 set = 0; set < m_ShaderDescriptorSets.size(); ++set)
		{
			auto &shaderDescriptorSet = m_ShaderDescriptorSets[set];

			if (shaderDescriptorSet.UniformBuffers.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.UniformBuffers.size();
				m_TypeCounts[set].push_back(typeCount);
			}

			if (shaderDescriptorSet.StorageBuffers.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageBuffers.size();
				m_TypeCounts[set].push_back(typeCount);
			}

			if (shaderDescriptorSet.ImageSamplers.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.ImageSamplers.size();
				m_TypeCounts[set].push_back(typeCount);
			}

			if (shaderDescriptorSet.StorageImages.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageImages.size();
				m_TypeCounts[set].push_back(typeCount);
			}

			std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
			
			// Make Uniform buffers
			for (auto &[binding, uniformBuffer] : shaderDescriptorSet.UniformBuffers)
			{
				VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = uniformBuffer->ShaderStage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[uniformBuffer->Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = 1;
				set.dstBinding = layoutBinding.binding;
			}

			// Make Storage buffers
			for (auto &[binding, storageBuffer] : shaderDescriptorSet.StorageBuffers)
			{
				VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = storageBuffer->ShaderStage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already by a UniformBuffer in use!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[storageBuffer->Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = 1;
				set.dstBinding = layoutBinding.binding;
			}

			// Make Image Samplers
			for (auto &[binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
			{
				VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				layoutBinding.descriptorCount = imageSampler.ArraySize;
				layoutBinding.stageFlags = imageSampler.ShaderStage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already by a UniformBuffer in use!");
				HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already by a StorageBuffer in use!");

				VkWriteDescriptorSet &set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				set = {};
				set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				set.descriptorType = layoutBinding.descriptorType;
				set.descriptorCount = imageSampler.ArraySize;
				set.dstBinding = layoutBinding.binding;
			}

			// Make Storage Images
			for (auto &[bindingAndSet, imageSampler] : shaderDescriptorSet.StorageImages)
			{
				VkDescriptorSetLayoutBinding &layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = imageSampler.ShaderStage;
				layoutBinding.pImmutableSamplers = nullptr;

				uint32 binding = bindingAndSet & 0xffffffff;
				layoutBinding.binding = binding;

				HL_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already by a UniformBuffer in use!");
				HL_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already by a StorageBuffer in use!");
				HL_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already by a ImageSampler in use!");

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
			descriptorLayout.bindingCount = (uint32)layoutBindings.size();
			descriptorLayout.pBindings = layoutBindings.data();

			HL_CORE_INFO("Creating descriptor set {0} with {1} ubo's, {2} ssbo's, {3} samplers and {4} storage images!", 
						 set, 
						 shaderDescriptorSet.UniformBuffers.size(),
						 shaderDescriptorSet.StorageBuffers.size(),
						 shaderDescriptorSet.ImageSamplers.size(),
						 shaderDescriptorSet.StorageImages.size());

			if (set >= m_DescriptorSetLayouts.size())
				m_DescriptorSetLayouts.resize((uint64)set + 1);
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &m_DescriptorSetLayouts[set]));
		}
	}

	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32 set, uint32 numberOfSets)
	{
		ShaderMaterialDescriptorSet result;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		std::unordered_map<uint32, std::vector<VkDescriptorPoolSize>> poolSizes;

		for (uint32 set = 0; set < m_ShaderDescriptorSets.size(); ++set)
		{
			auto &shaderDescriptorSet = m_ShaderDescriptorSets[set];
			if (!shaderDescriptorSet)
				continue;

			if (shaderDescriptorSet.UniformBuffers.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.UniformBuffers.size() * numberOfSets;
				poolSizes[set].push_back(typeCount);
			}

			if (shaderDescriptorSet.StorageBuffers.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageBuffers.size() * numberOfSets;
				poolSizes[set].push_back(typeCount);
			}

			if (shaderDescriptorSet.ImageSamplers.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

				uint32 descriptorSetCount = 0;
				for (auto &&[binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
					descriptorSetCount += imageSampler.ArraySize;

				typeCount.descriptorCount = descriptorSetCount * numberOfSets;
				poolSizes[set].push_back(typeCount);
			}

			if (shaderDescriptorSet.StorageImages.size())
			{
				VkDescriptorPoolSize typeCount;
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				typeCount.descriptorCount = (uint32)shaderDescriptorSet.StorageImages.size() * numberOfSets;
				poolSizes[set].push_back(typeCount);
			}
		}

		HL_ASSERT(poolSizes.find(set) != poolSizes.end());

		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.poolSizeCount = (uint32)poolSizes.at(set).size();
		descriptorPoolInfo.pPoolSizes = poolSizes.at(set).data();
		descriptorPoolInfo.maxSets = numberOfSets;
		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &result.Pool));

		result.DescriptorSets.resize(numberOfSets);
		for (uint32 i = 0; i < numberOfSets; ++i)
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
		HL_ASSERT(set < m_ShaderDescriptorSets.size());
	//	HL_ASSERT(m_ShaderDescriptorSets[set]);

		if (m_ShaderDescriptorSets.at(set).WriteDescriptorSets.find(name) == m_ShaderDescriptorSets.at(set).WriteDescriptorSets.end())
		{
			HL_CORE_WARN("Shader {0} does not contain requested descriptor set {1}", *m_Name, *name);
			return nullptr;
		}

		return &m_ShaderDescriptorSets.at(set).WriteDescriptorSets.at(name);
	}
	
	void VulkanShader::ClearUniformBuffers()
	{
		s_UniformBuffers.clear();
		s_StorageBuffers.clear();
	}
	
	void VulkanShader::Load(const HLString &source, bool forceCompile)
	{
		if (FileSystem::Get()->FileExists(m_AssetPath))
		{
			HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Shader {0} loaded [+]", **m_AssetPath);
			m_ShaderSource = PreProcess(source);

			std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> shaderData;
			CompileOrGetVulkanBinary(shaderData, forceCompile);
			LoadAndCreateShaders(shaderData);
			ReflectAllShaderStages(shaderData);
			CreateDescriptors();
		}
		else
		{
			HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-] Shader {0} not found! [-]", **m_AssetPath);
		}
	}

	std::unordered_map<VkShaderStageFlagBits, HLString> VulkanShader::PreProcess(const HLString &source)
	{
		std::unordered_map<VkShaderStageFlagBits, HLString> result;
		std::unordered_map<VkShaderStageFlagBits, std::stringstream> temp;
		std::stringstream s(*source);
		std::string line;
		VkShaderStageFlagBits type;

		// Parse Shader
		temp[VK_SHADER_STAGE_VERTEX_BIT] = std::stringstream();
		temp[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = std::stringstream();
		temp[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = std::stringstream();
		temp[VK_SHADER_STAGE_GEOMETRY_BIT] = std::stringstream();
		temp[VK_SHADER_STAGE_FRAGMENT_BIT] = std::stringstream();
		temp[VK_SHADER_STAGE_COMPUTE_BIT] = std::stringstream();

		while (std::getline(s, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = VK_SHADER_STAGE_VERTEX_BIT;
				else if (line.find("tess_control") != std::string::npos)
					type = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				else if (line.find("tess_eval") != std::string::npos)
					type = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				else if (line.find("geometry") != std::string::npos)
					type = VK_SHADER_STAGE_GEOMETRY_BIT;
				else if (line.find("pixel") != std::string::npos)
					type = VK_SHADER_STAGE_FRAGMENT_BIT;
				else if (line.find("fragment") != std::string::npos)
					type = VK_SHADER_STAGE_FRAGMENT_BIT;
				else if (line.find("compute") != std::string::npos)
					type = VK_SHADER_STAGE_COMPUTE_BIT;
			}
			else
			{
				temp[type] << line << "\n";
			}
		}

		result[VK_SHADER_STAGE_VERTEX_BIT] = HLString(temp[VK_SHADER_STAGE_VERTEX_BIT].str().c_str());
		result[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = HLString(temp[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].str().c_str());
		result[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = HLString(temp[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].str().c_str());
		result[VK_SHADER_STAGE_GEOMETRY_BIT] = HLString(temp[VK_SHADER_STAGE_GEOMETRY_BIT].str().c_str());
		result[VK_SHADER_STAGE_FRAGMENT_BIT] = HLString(temp[VK_SHADER_STAGE_FRAGMENT_BIT].str().c_str());
		result[VK_SHADER_STAGE_COMPUTE_BIT] = HLString(temp[VK_SHADER_STAGE_COMPUTE_BIT].str().c_str());

		// Replace \r\n with \n
		result[VK_SHADER_STAGE_VERTEX_BIT] = result[VK_SHADER_STAGE_VERTEX_BIT].Replace("\r\n", "\n");
		result[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = result[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].Replace("\r\n", "\n");
		result[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = result[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].Replace("\r\n", "\n");
		result[VK_SHADER_STAGE_GEOMETRY_BIT] = result[VK_SHADER_STAGE_GEOMETRY_BIT].Replace("\r\n", "\n");
		result[VK_SHADER_STAGE_FRAGMENT_BIT] = result[VK_SHADER_STAGE_FRAGMENT_BIT].Replace("\r\n", "\n");
		result[VK_SHADER_STAGE_COMPUTE_BIT] = result[VK_SHADER_STAGE_COMPUTE_BIT].Replace("\r\n", "\n");

		return result;
	}
	
	void VulkanShader::CompileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &outputBinary, bool forceCompile)
	{
		FileSystemPath cacheDir = utils::GetCacheDirectory();
		for (auto [stage, source] : m_ShaderSource)
		{
			const char *extension = utils::GetCacheFileExtensionBasedOnShaderType(stage);
			if (!forceCompile)
			{
				FileSystemPath p = cacheDir / (m_AssetPath.Filename() + extension);
				HLString cachedFilePath = p.String();

				FILE *f;
				errno_t error = fopen_s(&f, *cachedFilePath, "rb");
				if (!error)
				{
					// Get File Size
					fseek(f, 0, SEEK_END);
					uint64 size = ftell(f);
					fseek(f, 0, SEEK_SET);

					outputBinary[stage] = std::vector<uint32>(size / sizeof(uint32));
					fread(outputBinary[stage].data(), sizeof(uint32), outputBinary[stage].size(), f);
					fclose(f);
				}

			}

			if (outputBinary[stage].size() == 0)
			{
				shaderc::Compiler compiler;
				shaderc::CompileOptions options;
				options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
				options.SetWarningsAsErrors();
				options.SetGenerateDebugInfo();

				const bool optimize = false;
				if (optimize)
					options.SetOptimizationLevel(shaderc_optimization_level_performance);

				// Compile shader
				auto &shaderSource = m_ShaderSource.at(stage);
				if (shaderSource.IsEmpty())
					continue;

				shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(*shaderSource, utils::VulkanShaderStageToShaderCShaderKind(stage), **m_AssetPath, options);
				if (result.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					HL_CORE_ERROR("{0}", result.GetErrorMessage().c_str());
					HL_ASSERT(false);
				}

				outputBinary[stage] = std::vector<uint32>(result.cbegin(), result.cend());

				// Cache compiled shader
				FileSystemPath p = cacheDir / (m_AssetPath.Filename() + extension);
				HLString cachedFilePath = p.String();

				FILE *f;
				fopen_s(&f, *cachedFilePath, "wb");
				fwrite(outputBinary[stage].data(), sizeof(uint32), outputBinary[stage].size(), f);
				fclose(f);
			}
		}
	}
	
	void VulkanShader::LoadAndCreateShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		m_PipelineShaderStageCreateInfos.clear();

		for (auto [stage, data] : shaderData)
		{
			if (!data.size())
				continue;
			
			VkShaderModuleCreateInfo moduleCreateInto = {};
			moduleCreateInto.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInto.codeSize = data.size() * sizeof(uint32);
			moduleCreateInto.pCode = data.data();

			VkShaderModule shaderModule;
			VK_CHECK_RESULT(vkCreateShaderModule(device, &moduleCreateInto, nullptr, &shaderModule));

			VkPipelineShaderStageCreateInfo &shaderStage = m_PipelineShaderStageCreateInfos.emplace_back();
			shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStage.stage = stage;
			shaderStage.module = shaderModule;
			shaderStage.pName = "main";
		}
	}
	
	void VulkanShader::Reflect(VkShaderStageFlagBits shaderStage, const std::vector<uint32> &shaderData)
	{
		if (!shaderData.size())
			return;
			
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		HL_CORE_TRACE("===========================");
		HL_CORE_TRACE(" Vulkan Shader Reflection");
		HL_CORE_TRACE(" {0}", **m_AssetPath);
		HL_CORE_TRACE("===========================");

		spirv_cross::Compiler compiler(shaderData);
		auto resources = compiler.get_shader_resources();

		uint32 uniformBufferSize = (uint32)resources.uniform_buffers.size();
		HL_CORE_TRACE("Uniform Buffers: {0}", uniformBufferSize);
		for (const auto &resource : resources.uniform_buffers)
		{
			const auto &name = resource.name;
			auto &bufferType = compiler.get_type(resource.base_type_id);
			int32 memberCount = (int32)bufferType.member_types.size();
			uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32 descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32 size = (uint32)compiler.get_declared_struct_size(bufferType);

			if (descriptorSet >= m_ShaderDescriptorSets.size())
				m_ShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet &set = m_ShaderDescriptorSets[descriptorSet];
			if (s_UniformBuffers[descriptorSet].find(binding) == s_UniformBuffers[descriptorSet].end())
			{
				UniformBuffer *uniformBuffer = new UniformBuffer();
				uniformBuffer->BindingPoint = binding;
				uniformBuffer->Size = size;
				uniformBuffer->Name = name;
				uniformBuffer->ShaderStage = VK_SHADER_STAGE_ALL;
				s_UniformBuffers.at(descriptorSet)[binding] = uniformBuffer;
			}
			else
			{
				UniformBuffer *uniformBuffer = s_UniformBuffers.at(descriptorSet).at(binding);
				if (size > uniformBuffer->Size)
					uniformBuffer->Size = size;
			}

			set.UniformBuffers[binding] = s_UniformBuffers.at(descriptorSet).at(binding);
			HL_CORE_TRACE("  {0} ({1}, {2})", name.c_str(), descriptorSet, binding);
			HL_CORE_TRACE("  Member Count: {0}", memberCount);
			HL_CORE_TRACE("  Size: {0}", size);
			HL_CORE_TRACE("-------------------");
		}

		uint32 storageBufferSize = (uint32)resources.storage_buffers.size();
		HL_CORE_TRACE("Storage Buffers: {0}", storageBufferSize);
		for (const auto &resource : resources.storage_buffers)
		{
			const auto &name = resource.name;
			auto &bufferType = compiler.get_type(resource.base_type_id);
			uint32 memberCount = (uint32)bufferType.member_types.size();
			uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32 descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32 size = (uint32)compiler.get_declared_struct_size(bufferType);

			if (descriptorSet >= m_ShaderDescriptorSets.size())
				m_ShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet &set = m_ShaderDescriptorSets[descriptorSet];
			if (s_StorageBuffers[descriptorSet].find(binding) == s_StorageBuffers[descriptorSet].end())
			{
				StorageBuffer *storageBuffer = new StorageBuffer();
				storageBuffer->BindingPoint = binding;
				storageBuffer->Size = size;
				storageBuffer->Name = name;
				storageBuffer->ShaderStage = VK_SHADER_STAGE_ALL;
				s_StorageBuffers.at(descriptorSet)[binding] = storageBuffer;
			}
			else
			{
				StorageBuffer *storageBuffer = s_StorageBuffers.at(descriptorSet).at(binding);
				if (size > storageBuffer->Size)
					storageBuffer->Size = size;
			}

			set.StorageBuffers[binding] = s_StorageBuffers.at(descriptorSet).at(binding);
			HL_CORE_TRACE("  {0} ({1}, {2})", name.c_str(), descriptorSet, binding);
			HL_CORE_TRACE("  Member Count: {0}", memberCount);
			HL_CORE_TRACE("  Size: {0}", size);
			HL_CORE_TRACE("-------------------");
		}

		uint32 pushConstantBufferSize = (uint32)resources.push_constant_buffers.size();
		HL_CORE_TRACE("Push Constant Buffers: {0}", pushConstantBufferSize);
		for (const auto &resource : resources.push_constant_buffers)
		{
			const auto &bufferName = resource.name;
			auto &bufferType = compiler.get_type(resource.base_type_id);
			uint32 bufferSize = (uint32)compiler.get_declared_struct_size(bufferType);
			uint32 memberCount = (uint32)bufferType.member_types.size();
			uint32 bufferOffset = 0;

			if (m_PushConstantRanges.size())
				bufferOffset = m_PushConstantRanges.back().Offset + m_PushConstantRanges.back().Size;

			auto &pushConstantRange = m_PushConstantRanges.emplace_back();
			pushConstantRange.ShaderStage = shaderStage;
			pushConstantRange.Size = bufferSize - bufferOffset;
			pushConstantRange.Offset = bufferOffset;

			if (bufferName.empty() || bufferName == "u_Renderer")
				continue;

			ShaderBuffer &buffer = m_Buffers[bufferName];
			buffer.Name = bufferName;
			buffer.Size = bufferSize - bufferOffset;

			HL_CORE_TRACE("  Name: {0}", bufferName.c_str());
			HL_CORE_TRACE("  Member Count: {0}", memberCount);
			HL_CORE_TRACE("  Size: {0}", bufferSize);

			for (uint32 i = 0; i < memberCount; ++i)
			{
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto &memberName = compiler.get_member_name(bufferType.self, i);
				auto size = (uint32)compiler.get_declared_struct_member_size(bufferType, i);
				auto offset = compiler.type_struct_member_offset(bufferType, i) - bufferOffset;

				HLString uniformName = fmt::format("{}.{}", bufferName, memberName);
				buffer.Uniforms[uniformName] = ShaderUniform(uniformName, utils::SPIRTypeToShaderUniformType(type), size, offset);
			}
		}

		uint32 sampledImagesSize = (uint32)resources.sampled_images.size();
		HL_CORE_TRACE("Sampled Images: {0}", sampledImagesSize);
		for (const auto &resource : resources.sampled_images)
		{
			const auto &name = resource.name;
			auto &baseType = compiler.get_type(resource.base_type_id);
			auto &type = compiler.get_type(resource.type_id);

			uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32 descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32 dimension = baseType.image.dim;
			uint32 arraySize = type.array[0];

			if (arraySize == 0)
				arraySize = 1;

			if (descriptorSet >= m_ShaderDescriptorSets.size())
				m_ShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet &set = m_ShaderDescriptorSets[descriptorSet];
			auto &imageSampler = set.ImageSamplers[binding];

			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.ShaderStage = shaderStage;
			imageSampler.ArraySize = arraySize;

			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);
			HL_CORE_TRACE("  {0} ({1}, {2})", name.c_str(), descriptorSet, binding);
		}

		uint32 storageImagesSize = (uint32)resources.storage_images.size();
		HL_CORE_TRACE("Storage Images: {0}", storageImagesSize);
		for (const auto &resource : resources.storage_images)
		{
			const auto &name = resource.name;
			auto &type = compiler.get_type(resource.base_type_id);
			uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32 descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			uint32 dimension = type.image.dim;

			if (descriptorSet >= m_ShaderDescriptorSets.size())
				m_ShaderDescriptorSets.resize(descriptorSet + 1);

			ShaderDescriptorSet &set = m_ShaderDescriptorSets[descriptorSet];
			auto &imageSampler = set.StorageImages[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.ShaderStage = shaderStage;

			HL_CORE_TRACE("  {0} ({1}, {2})", name.c_str(), descriptorSet, binding);
		}

		HL_CORE_TRACE("===========================");
	}
	
	void VulkanShader::ReflectAllShaderStages(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32>> &shaderData)
	{
		m_Resources.clear();

		for (auto [stage, data] : shaderData)
		{
			Reflect(stage, data);
		}
	}
}

#endif // HIGHLO_API_VULKAN

