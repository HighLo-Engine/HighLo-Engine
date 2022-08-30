// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLShader.h"

#ifdef HIGHLO_API_OPENGL

#include "Engine/Core/FileSystem.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Graphics/Shaders/ShaderCache.h"

#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <libshaderc_util/file_finder.h>

#include "Engine/Graphics/Shaders/GLSLIncluder.h"
#include "OpenGLUtils.h"

namespace highlo
{
#define GL_SHADER_LOG_PREFIX "Shader>       "
#define PRINT_DEBUG_ALL 0

#ifdef HL_DEBUG
#define PRINT_DEBUG_OUTPUTS 1
#endif

#if PRINT_DEBUG_ALL
#define PRINT_DEBUG_OUTPUTS 1
#define PRINT_PREPROCESSING_RESULT 1
#endif

	namespace utils
	{
		static FileSystemPath GetCacheDirectory()
		{
			FileSystemPath path = "assets/cache/shaders/OpenGL/";

			if (!FileSystem::Get()->FolderExists(path))
				FileSystem::Get()->CreateFolder(path);

			return path;
		}

		static const char *GLShaderStageCachedOpenGLFileExtension(uint32 stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:		return ".cached_opengl.frag";
				case GL_COMPUTE_SHADER:			return ".cached_opengl.comp";
				case GL_TESS_CONTROL_SHADER:	return ".cached_opengl.tesscon";
				case GL_TESS_EVALUATION_SHADER:	return ".cached_opengl.tesseval";
				case GL_GEOMETRY_SHADER:		return ".cached_opengl.geo";
			}

			HL_ASSERT(false);
			return "";
		}

		static const char *GLShaderStageCachedVulkanFileExtension(uint32 stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:		return ".cached_vulkan.frag";
				case GL_COMPUTE_SHADER:			return ".cached_vulkan.comp";
				case GL_TESS_CONTROL_SHADER:	return ".cached_vulkan.tesscon";
				case GL_TESS_EVALUATION_SHADER: return ".cached_vulkan.tesseval";
				case GL_GEOMETRY_SHADER:		return ".cached_vulkan.geo";
			}

			HL_ASSERT(false);
			return "";
		}

		static const char *GLShaderTypeToString(uint32 stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return "Vertex";
				case GL_FRAGMENT_SHADER:		return "Pixel";
				case GL_COMPUTE_SHADER:			return "Compute";
				case GL_TESS_CONTROL_SHADER:	return "TessControl";
				case GL_TESS_EVALUATION_SHADER: return "TessEvalulation";
				case GL_GEOMETRY_SHADER:		return "Geometry";
			}

			HL_ASSERT(false);
			return "";
		}

		static shaderc_shader_kind GLShaderStageToShaderC(uint32 stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return shaderc_vertex_shader;
				case GL_FRAGMENT_SHADER:		return shaderc_fragment_shader;
				case GL_COMPUTE_SHADER:			return shaderc_compute_shader;
				case GL_TESS_CONTROL_SHADER:	return shaderc_tess_control_shader;
				case GL_TESS_EVALUATION_SHADER:	return shaderc_tess_evaluation_shader;
				case GL_GEOMETRY_SHADER:		return shaderc_geometry_shader;
			}

			HL_ASSERT(false);
			return (shaderc_shader_kind)0;
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

		static shaderc_shader_kind ShaderStageToShaderC(const GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return shaderc_vertex_shader;
				case GL_FRAGMENT_SHADER:		return shaderc_fragment_shader;
				case GL_COMPUTE_SHADER:			return shaderc_compute_shader;
				case GL_TESS_CONTROL_SHADER:	return shaderc_tess_control_shader;
				case GL_TESS_EVALUATION_SHADER: return shaderc_tess_evaluation_shader;
				case GL_GEOMETRY_SHADER:		return shaderc_geometry_shader;
			}

			HL_ASSERT(false);
			return {};
		}

		static ShaderLanguage ShaderLanguageFromExtension(const HLString &extension)
		{
			if (extension == "glsl")
				return ShaderLanguage::GLSL;

			if (extension == "hlsl")
				return ShaderLanguage::HLSL;

			return ShaderLanguage::None;
		}

		static HLString ShaderStageCachedFileExtension(const GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:		return ".cached_opengl.frag";
				case GL_COMPUTE_SHADER:			return ".cached_opengl.comp";
				case GL_TESS_CONTROL_SHADER:	return ".cached_opengl.tessconn";
				case GL_TESS_EVALUATION_SHADER:	return ".cached_opengl.tessval";
				case GL_GEOMETRY_SHADER:		return ".cached_opengl.geo";
			}

			HL_ASSERT(false);
			return "";
		}

		static HLString ShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:			return "vertex";
				case GL_FRAGMENT_SHADER:		return "fragment";
				case GL_COMPUTE_SHADER:			return "compute";
				case GL_TESS_EVALUATION_SHADER:	return "tess_eval";
				case GL_TESS_CONTROL_SHADER:	return "tess_control";
				case GL_GEOMETRY_SHADER:		return "geometry";
			}

			HL_ASSERT(false);
			return "";
		}

		static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
		{
			switch (type.basetype)
			{
			case spirv_cross::SPIRType::Struct: return ShaderUniformType::Struct;
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

		static HLString ShaderStageToMacro(const GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:          return "__VERTEX_STAGE__";
				case GL_FRAGMENT_SHADER:        return "__FRAGMENT_STAGE__";
				case GL_COMPUTE_SHADER:         return "__COMPUTE_STAGE__";
				case GL_TESS_CONTROL_SHADER:	return "__TESS_CONTROL_STAGE__";
				case GL_TESS_EVALUATION_SHADER:	return "__TESS_EVAL_STAGE__";
				case GL_GEOMETRY_SHADER:		return "__GEOMETRY_STAGE__";
			}

			HL_ASSERT(false);
			return "";
		}
	}

	static std::unordered_map<uint32, std::unordered_map<uint32, OpenGLShaderUniformBuffer*>> s_UniformBuffers;
	static std::unordered_map<uint32, std::unordered_map<uint32, OpenGLShaderStorageBuffer*>> s_StorageBuffers;

	OpenGLShader::OpenGLShader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
		m_Language = utils::ShaderLanguageFromExtension(filePath.Extension());
		HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
		Load(source, forceCompile);
	}

	OpenGLShader::OpenGLShader(const HLString &source, const HLString &name, ShaderLanguage language)
	{
		m_Name = name;
		m_Language = language;
		Load(source, true);
	}

	OpenGLShader::~OpenGLShader()
	{
		Release();
	}

	void OpenGLShader::Reload(bool forceCompile)
	{
		HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Reloading shader {0}... [+]", **m_AssetPath);
		m_Loaded = false; // Reflect current stage: Shader is being reloaded

		HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
		Load(source, forceCompile);

		Renderer::OnShaderReloaded(GetHash());

		for (ShaderReloadedCallback callback : m_ReloadedCallbacks)
			callback();
	}

	void OpenGLShader::Release()
	{
		glUseProgram(0);
		glDeleteProgram(m_RendererID);
		m_RendererID = 0;
	}

	uint64 OpenGLShader::GetHash() const
	{
		return m_AssetPath.Hash();
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::AddShaderReloadedCallback(const ShaderReloadedCallback &callback)
	{
		m_ReloadedCallbacks.push_back(callback);
	}

	void OpenGLShader::SetMacro(const HLString &name, const HLString &value)
	{
		m_Macros[name] = value;
	}
	
	void OpenGLShader::ClearUniformBuffers()
	{
		s_UniformBuffers.clear();
	}
	
	void OpenGLShader::Load(const HLString &source, bool forceCompile)
	{
		if (FileSystem::Get()->FileExists(m_AssetPath))
		{
			HL_CORE_TRACE(GL_SHADER_LOG_PREFIX "[+] Trying to create shader {0}... [+]", **m_AssetPath);

			m_ShaderSources = PreProcess(source);
			bool shaderCacheHasChanged = ShaderCache::HasChanged(m_AssetPath, source);

			std::unordered_map<uint32, std::vector<uint32>> shaderData;
			CompileOrGetOpenGLBinary(shaderData, forceCompile || shaderCacheHasChanged);
			LoadAndCreateShaders(shaderData);
			ReflectAllShaderStages(shaderData);

			HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Shader {0} loaded [+]", **m_AssetPath);
			m_Loaded = true;
		}
		else
		{
			HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-] Shader {0} not found! [-]", **m_AssetPath);
		}
	}
	
	void OpenGLShader::Reflect(GLenum shaderStage, const std::vector<uint32> &shaderData)
	{
	#if PRINT_DEBUG_OUTPUTS
		HL_CORE_TRACE("===========================");
		HL_CORE_TRACE(" OpenGL Shader Reflection ");
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
				const auto &name = compiler.get_name(resource.id); // this is necessary to get the instance name instead of the structure name
				const auto &uniformBlockName = compiler.get_name(resource.base_type_id);
				auto &bufferType = compiler.get_type(resource.base_type_id);
				uint32 memberCount = (uint32)bufferType.member_types.size();
				uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
				uint32 descriptorSet = (uint32)compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				uint32 size = (uint32)compiler.get_declared_struct_size(bufferType);
				uint32 bufferOffset = 0;

				if (descriptorSet >= m_ShaderDescriptorSets.size())
					m_ShaderDescriptorSets.resize(descriptorSet + 1);

				OpenGLShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
				if (s_UniformBuffers[descriptorSet].find(binding) == s_UniformBuffers[descriptorSet].end())
				{
					OpenGLShaderUniformBuffer *uniformBuffer = new OpenGLShaderUniformBuffer();
					uniformBuffer->BindingPoint = binding;
					uniformBuffer->Size = size;
					uniformBuffer->Name = name;
					uniformBuffer->Stage = GL_UNIFORM_BUFFER;
					s_UniformBuffers.at(descriptorSet)[binding] = uniformBuffer;
				}
				else
				{
					OpenGLShaderUniformBuffer *uniformBuffer = s_UniformBuffers.at(descriptorSet).at(binding);
					if (size > uniformBuffer->Size)
						uniformBuffer->Size = size;
				}

				shaderDescriptorSet.UniformBuffers[binding] = s_UniformBuffers.at(descriptorSet).at(binding);

				ShaderBuffer &shaderBuffer = m_Buffers[uniformBlockName];
				shaderBuffer.Name = uniformBlockName;
				shaderBuffer.Size = size - bufferOffset;

				for (uint32 i = 0; i < memberCount; ++i)
				{
					auto &memberType = compiler.get_type(bufferType.member_types[i]);
					const auto &memberName = compiler.get_member_name(bufferType.self, i);
					uint32 memberSize = (uint32)compiler.get_declared_struct_member_size(bufferType, i);
					uint32 memberOffset = (uint32)compiler.type_struct_member_offset(bufferType, i) - bufferOffset;

					const HLString &uniformName = fmt::format("{}.{}", name, memberName);
					shaderBuffer.Uniforms[uniformName] = ShaderUniform(uniformName, binding, utils::SPIRTypeToShaderUniformType(memberType), memberSize, memberOffset);
				}

			#if PRINT_DEBUG_OUTPUTS
				HL_CORE_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
				HL_CORE_TRACE("  Member Count: {0}", memberCount);
				HL_CORE_TRACE("  Size: {0} bytes", size);
				HL_CORE_TRACE("  Members:");
				for (uint32 i = 0; i < memberCount; ++i)
				{
					const auto &memberName = compiler.get_member_name(bufferType.self, i);
					uint32 memberSize = (uint32)compiler.get_declared_struct_member_size(bufferType, i);
					uint32 memberOffset = (uint32)compiler.type_struct_member_offset(bufferType, i) - bufferOffset;
					const HLString &uniformName = fmt::format("{}.{}", name, memberName);

					HL_CORE_TRACE("    Name: {0}, Full name: {1}", memberName, *uniformName);
					HL_CORE_TRACE("    Size: {0} bytes", memberSize);
					HL_CORE_TRACE("    Offset: {0} bytes", memberOffset);
					HL_CORE_TRACE("");
				}

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

				OpenGLShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
				if (s_StorageBuffers[descriptorSet].find(binding) == s_StorageBuffers[descriptorSet].end())
				{
					OpenGLShaderStorageBuffer *storageBuffer = new OpenGLShaderStorageBuffer();
					storageBuffer->Name = name;
					storageBuffer->Size = size;
					storageBuffer->BindingPoint = binding;
					storageBuffer->Stage = GL_SHADER_STORAGE_BUFFER;
					s_StorageBuffers.at(descriptorSet)[binding] = storageBuffer;
				}
				else
				{
					OpenGLShaderStorageBuffer *storageBuffer = s_StorageBuffers.at(descriptorSet).at(binding);
					if (size > storageBuffer->Size)
						storageBuffer->Size = size;
				}

				shaderDescriptorSet.StorageBuffers[binding] = s_StorageBuffers.at(descriptorSet).at(binding);

			#if PRINT_DEBUG_OUTPUTS
				HL_CORE_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
				HL_CORE_TRACE("  Member Count: {0}", memberCount);
				HL_CORE_TRACE("  Size: {0} bytes", size);
				HL_CORE_TRACE("  Members:");
				for (uint32 i = 0; i < memberCount; ++i)
				{
					const auto &memberName = compiler.get_member_name(bufferType.self, i);
					uint32 memberSize = (uint32)compiler.get_declared_struct_member_size(bufferType, i);
					uint32 memberOffset = (uint32)compiler.type_struct_member_offset(bufferType, i);
					const HLString &uniformName = fmt::format("{}.{}", name, memberName);

					HL_CORE_TRACE("    Name: {0}, Full name: {1}", memberName, *uniformName);
					HL_CORE_TRACE("    Size: {0} bytes", memberSize);
					HL_CORE_TRACE("    Offset: {0} bytes", memberOffset);
					HL_CORE_TRACE("");
				}

				HL_CORE_TRACE("-------------------");
			#endif // PRINT_DEBUG_OUTPUTS
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

			OpenGLShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.ImageSamplers[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.ArraySize = arraySize;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Stage = shaderStage;
			imageSampler.Name = name;

			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

		#if PRINT_DEBUG_OUTPUTS
			HL_CORE_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
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

			OpenGLShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.SeparateTextures[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.ArraySize = arraySize;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.Stage = shaderStage;

			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

		#if PRINT_DEBUG_OUTPUTS
			HL_CORE_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
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

			OpenGLShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.SeparateSamplers[binding];
			imageSampler.ArraySize = arraySize;
			imageSampler.Name = name;
			imageSampler.BindingPoint = binding;
			imageSampler.Stage = shaderStage;
			imageSampler.DescriptorSet = descriptorSet;

			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

		#if PRINT_DEBUG_OUTPUTS
			HL_CORE_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
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

			OpenGLShaderDescriptorSet &shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];

			auto &imageSampler = shaderDescriptorSet.StorageImages[binding];
			imageSampler.ArraySize = arraySize;
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.Stage = shaderStage;

			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);

		#if PRINT_DEBUG_OUTPUTS
			HL_CORE_TRACE("  {0} (descriptor set: {1}, binding: {2})", name, descriptorSet, binding);
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

	void OpenGLShader::ReflectAllShaderStages(const std::unordered_map<uint32, std::vector<uint32>> &shaderData)
	{
		m_Resources.clear();

		for (auto [stage, data] : shaderData)
		{
			Reflect(stage, data);
		}
	}
	
	HLString OpenGLShader::Compile(std::unordered_map<GLenum, std::vector<uint32>> &outputBinary, const GLenum stage) const
	{
		const HLString &stageSource = m_ShaderSources.at(stage);

		if (m_Language == ShaderLanguage::GLSL)
		{
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
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

	void OpenGLShader::TryGetOpenGLCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<GLenum, std::vector<uint32>> &outBinary, GLenum stage) const
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

	void OpenGLShader::LoadAndCreateShaders(const std::unordered_map<GLenum, std::vector<uint32>> &shaderData)
	{
		if (m_RendererID)
			glDeleteProgram(m_RendererID);

		std::vector<GLuint> shaderRendererIds;
		shaderRendererIds.reserve(shaderData.size());
		
		GLuint program = glCreateProgram();

		m_ConstantBufferOffset = 0;
		for (auto &[stage, data] : shaderData)
		{
			GLuint shaderId = glCreateShader(stage);
			glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), uint32(data.size() * sizeof(uint32)));
			glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);

			HL_CORE_TRACE(GL_SHADER_LOG_PREFIX "[+]     Compiling {0} shader ({1}) [+]", *utils::ShaderStageToString(stage), **m_AssetPath);

			GLint isCompiled = 0;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxInfoLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxInfoLength);

				if (maxInfoLength > 0)
				{
					std::vector<GLchar> infoLog(maxInfoLength);
					glGetShaderInfoLog(shaderId, maxInfoLength, &maxInfoLength, &infoLog[0]);
					HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-]     Shader Compilation failed ({0}):\n{1} [-]", **m_AssetPath, &infoLog[0]);

					glDeleteShader(shaderId);
					for (auto id : shaderRendererIds)
						glDeleteShader(id);

					glDeleteProgram(program);
					program = 0;
				}
				else
				{
					HL_ASSERT(false, "Compilation failed but no infoLog accessible!");
					HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-]     Compilation failed but no infoLog accessible! [-]");
				}
			}
			else
			{
				HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+]     {0} shader has been successfully compiled! ({1}) [+]", *utils::ShaderStageToString(stage), **m_AssetPath);
			}

			glAttachShader(program, shaderId);
			shaderRendererIds.emplace_back(shaderId);
		}

		// Link shader program
		HL_CORE_TRACE(GL_SHADER_LOG_PREFIX "[+]     Linking Shader {0} [+]", **m_AssetPath);
		
		HL_ASSERT(program != 0);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (GLint*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxInfoLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxInfoLength);

			if (maxInfoLength > 0)
			{
				std::vector<GLchar> infoLog(maxInfoLength);
				glGetProgramInfoLog(program, maxInfoLength, &maxInfoLength, &infoLog[0]);
				HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-]     Shader Linking failed ({0}):\n{1} [-]", **m_AssetPath, &infoLog[0]);

				glDeleteProgram(program);
				program = 0;
				for (auto id : shaderRendererIds)
					glDeleteShader(id);
			}
			else
			{
			//	HL_ASSERT(false, "Linking failed but no infoLog accessible!");
				HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-]     Linking failed but no infolog was accessible! [-]");
			}
		}
		else
		{
			HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+]     Shader has been successfully linked! [+]");
		}

		for (auto id : shaderRendererIds)
			glDetachShader(m_RendererID, id);

		if (program)
			m_RendererID = program;
	}
	
	void OpenGLShader::CompileOrGetOpenGLBinary(std::unordered_map<uint32, std::vector<uint32>> &shaderData, bool forceCompile)
	{
		FileSystemPath cacheDirectory = utils::GetCacheDirectory();

		for (auto &[stage, source] : m_ShaderSources)
		{
			if (source.IsEmpty())
				continue;

			const HLString &extension = utils::ShaderStageCachedFileExtension(stage);
			if (!forceCompile)
			{
				TryGetOpenGLCachedBinary(cacheDirectory, extension, shaderData, stage);
			}

			if (shaderData[stage].empty())
			{
				HLString error = Compile(shaderData, stage);
				if (error.IsEmpty())
				{
					// Compile success
					FileSystemPath p = cacheDirectory / (m_AssetPath.Filename() + extension);

					FILE *f;
					errno_t fileError = fopen_s(&f, **p, "wb");
					if (fileError)
						HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-]     Failed to cache shader binary! [-]");

					fwrite(shaderData[stage].data(), sizeof(uint32), shaderData[stage].size(), f);
					fclose(f);
				}
				else
				{
					HL_CORE_ERROR("{0}", *error);

					TryGetOpenGLCachedBinary(cacheDirectory, extension, shaderData, stage);
					if (shaderData[stage].empty())
					{
						HL_ASSERT(false, "Failed to compile shader and could not find any cached binary!");
						HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-]     Failed to compile shader and could not find any cached binary! [-]");
					}
					else
					{
						HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-]     Failed to compile {0}:{1}, so loaded a cached shader binary instead. [-]", **m_AssetPath, *utils::ShaderStageToString(stage));
					}
				}
			}
		}
	}
	
	std::unordered_map<GLenum, HLString> OpenGLShader::PreProcess(const HLString &source)
	{
		std::unordered_map<GLenum, HLString> result;

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

	std::unordered_map<GLenum, HLString> OpenGLShader::PreProcessGLSL(const HLString &source)
	{
		std::unordered_map<ShaderType, HLString> shaderSources = ShaderPreProcessor::PreprocessShader<ShaderLanguage::GLSL>(source, m_AcknowledgedMacros);
		std::unordered_map<GLenum, HLString> glShaderSources = utils::ConvertShaderTypeToOpenGLStage(shaderSources);
		shaderc::Compiler compiler;

		HL_CORE_TRACE(GL_SHADER_LOG_PREFIX "[+]     Pre-processing GLSL: {0} [+]", **m_AssetPath);

		shaderc_util::FileFinder fileFinder;
		fileFinder.search_path().emplace_back("assets/shaders/Include/GLSL/");
		fileFinder.search_path().emplace_back("assets/shaders/Include/Shared/");

		for (auto &[stage, shaderSource] : glShaderSources)
		{
			shaderc::CompileOptions options;
			options.AddMacroDefinition("__GLSL__");
			options.AddMacroDefinition("__OPENGL__");
			options.AddMacroDefinition(utils::ShaderStageToMacro(stage).C_Str());

			for (auto &[name, value] : m_Macros)
				options.AddMacroDefinition(*name, *value);

			GLSLIncluder *includer = new GLSLIncluder(&fileFinder);

			options.SetIncluder(std::unique_ptr<GLSLIncluder>(includer));

			shaderc::PreprocessedSourceCompilationResult preProcessingResult = compiler.PreprocessGlsl(*shaderSource, utils::ShaderStageToShaderC(stage), **m_AssetPath, options);
			if (preProcessingResult.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-]     Failed to pre-process Shader {0} with error {1} [-]", **m_AssetPath, preProcessingResult.GetErrorMessage());
			}

			m_StagesMetaData[stage].HashValue = shaderSource.Hash();
			m_StagesMetaData[stage].Headers = std::move(includer->GetIncludeData());

			m_AcknowledgedMacros.merge(includer->GetParsedSpecialMacros());

			shaderSource = std::string(preProcessingResult.begin(), preProcessingResult.end());

		#if PRINT_PREPROCESSING_RESULT
			HL_CORE_TRACE("{0}", *shaderSource);
		#endif // PRINT_PREPROCESSING_RESULT
		}

		return glShaderSources;
	}

	std::unordered_map<GLenum, HLString> OpenGLShader::PreProcessHLSL(const HLString &source)
	{
		// TODO
		std::unordered_map<GLenum, HLString> result;
		return result;
	}
	
	void OpenGLShader::ParseConstantBuffers(const spirv_cross::CompilerGLSL &compiler)
	{
		// Push constant ranges
		spirv_cross::ShaderResources res = compiler.get_shader_resources();
		for (const spirv_cross::Resource &resource : res.push_constant_buffers)
		{
			const HLString &name = resource.name;
			auto &type = compiler.get_type(resource.base_type_id);
			auto bufferSize = (uint32)compiler.get_declared_struct_size(type);
			uint32 binding = (uint32)compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32 memberCount = (uint32)type.member_types.size();
			uint32 bufferOffset = 0;

			if (m_PushConstantRanges.size())
				bufferOffset = m_PushConstantRanges.back().Offset + m_PushConstantRanges.back().Size;

			auto &pushConstantRange = m_PushConstantRanges.emplace_back();
			pushConstantRange.Size = bufferSize - bufferOffset;
			pushConstantRange.Offset = bufferOffset;

			if (name.IsEmpty())
			{
				m_ConstantBufferOffset += bufferSize;
				continue;
			}

			ShaderBuffer &buffer = m_Buffers[name];
			buffer.Name = name;
			buffer.Size = bufferSize - bufferOffset;

			for (uint32 i = 0; i < memberCount; ++i)
			{
				auto memberType = compiler.get_type(type.member_types[i]);
				const HLString &memberName = compiler.get_member_name(type.self, i);
				uint32 size = (uint32)compiler.get_declared_struct_member_size(type, i);
				uint32 offset = compiler.type_struct_member_offset(type, i) - bufferOffset;

				HLString uniformName = fmt::format("{}.{}", *name, *memberName);

			#if PRINT_DEBUG_OUTPUTS
				HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Registering push_constant with uniform name {0} [+]", *uniformName);
			#endif // PRINT_DEBUG_OUTPUTS

				buffer.Uniforms[uniformName] = ShaderUniform(uniformName, binding, utils::SpirvTypeToShaderUniformType(type), size, offset);
			}

			m_ConstantBufferOffset += bufferSize;
		}
	}
}

#endif

