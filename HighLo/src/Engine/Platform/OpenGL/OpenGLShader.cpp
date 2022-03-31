// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLShader.h"

#ifdef HIGHLO_API_OPENGL

#include "Engine/Core/FileSystem.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Graphics/Shaders/ShaderCache.h"

#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>

namespace highlo
{
#define PRINT_SHADERS 0
#define PRINT_DEBUG_OUTPUTS 0
#define GL_SHADER_LOG_PREFIX "Shader>       "

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
	}

	OpenGLShader::OpenGLShader(const FileSystemPath &filePath, bool forceCompile)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
		HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
		Load(source, forceCompile);
	}

	OpenGLShader::OpenGLShader(const HLString &source)
	{
		Load(source, true);
	}

	OpenGLShader::~OpenGLShader()
	{
	}

	void OpenGLShader::Reload(bool forceCompile)
	{
		HL_CORE_TRACE("Reloading shader {0}...", **m_AssetPath);
		HLString source = FileSystem::Get()->ReadTextFile(m_AssetPath);
		Load(source, forceCompile);

		Renderer::OnShaderReloaded(GetHash());

		for (ShaderReloadedCallback callback : m_ReloadedCallbacks)
			callback();
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
	
	const ShaderUniformBuffer &OpenGLShader::FindUniformBuffer(const HLString &name)
	{
		ShaderUniformBuffer *uniformBuffer = nullptr;

		for (auto &[bindingPoint, ub] : s_UniformBuffers)
		{
			if (ub.Name == name)
			{
				uniformBuffer = &ub;
				break;
			}
		}

		HL_ASSERT(uniformBuffer);
		return *uniformBuffer;
	}
	
	void OpenGLShader::SetUniformBuffer(const ShaderUniformBuffer &buffer, const void *data, uint32 size, uint32 offset)
	{
		glNamedBufferSubData(buffer.RendererID, offset, size, data);
	}
	
	void OpenGLShader::SetStorageBuffer(const ShaderStorageBuffer &buffer, const void *data, uint32 size, uint32 offset)
	{
		glNamedBufferSubData(buffer.RendererID, offset, size, data);
	}
	
	void OpenGLShader::SetUniformBuffer(const HLString &name, const void *data, uint32 size)
	{
		ShaderUniformBuffer *uniformBuffer = nullptr;

		for (auto &[bindingPoint, ub] : s_UniformBuffers)
		{
			if (ub.Name == name)
			{
				uniformBuffer = &ub;
				break;
			}
		}

		HL_ASSERT(uniformBuffer);
		HL_ASSERT(uniformBuffer->Size >= size);
		glNamedBufferSubData(uniformBuffer->RendererID, 0, size, data);
	}
	
	void OpenGLShader::SetStorageBuffer(const HLString &name, const void *data, uint32 size)
	{
		ShaderStorageBuffer *storageBuffer = nullptr;

		for (auto &[bindingPoint, sb] : s_StorageBuffers)
		{
			if (sb.Name == name)
			{
				storageBuffer = &sb;
				break;
			}
		}

		HL_ASSERT(storageBuffer);
		HL_ASSERT(storageBuffer->Size >= size);
		glNamedBufferSubData(storageBuffer->RendererID, 0, size, data);
	}
	
	void OpenGLShader::ResizeStorageBuffer(uint32 bindingPoint, uint32 newSize)
	{
		ShaderStorageBuffer &buffer = s_StorageBuffers.at(bindingPoint);
		if (newSize == buffer.Size)
			return;

		buffer.Size = newSize;
		glNamedBufferData(buffer.RendererID, buffer.Size, nullptr, GL_DYNAMIC_DRAW);

	#if PRINT_DEBUG_OUTPUTS
		HL_CORE_INFO("Resized Storage Buffer at binding point {0} with name '{1}' and a size of {2}", buffer.BindingPoint, *buffer.Name, buffer.Size);
	#endif // PRINT_DEBUG_OUTPUTS
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, float value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform1f(location, value);
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, int32 value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform1i(location, value);
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::ivec2 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform2i(location, value.x, value.y);
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::ivec3 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform3i(location, value.x, value.y, value.z);
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::ivec4 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, uint32 value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform1ui(location, value);
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::vec2 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform2fv(location, 1, glm::value_ptr(value));
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::vec3 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::vec4 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const HLString &fullname, const glm::mat2 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::mat3 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void OpenGLShader::SetUniform(const HLString &fullname, const glm::mat4 &value)
	{
		HL_ASSERT(m_UniformLocations.find(fullname) != m_UniformLocations.end());
		int32 location = m_UniformLocations.at(fullname);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	const ShaderResourceDeclaration *OpenGLShader::GetShaderResource(const HLString &name)
	{
		if (m_Resources.find(name) == m_Resources.end())
			return nullptr;

		return &m_Resources.at(name);
	}
	
	void OpenGLShader::ClearUniformBuffers()
	{
		s_UniformBuffers.clear();
	}
	
	void OpenGLShader::Load(const HLString &source, bool forceCompile)
	{
		if (FileSystem::Get()->FileExists(m_AssetPath))
		{
			HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Trying to create shader {0} [+]", **m_AssetPath);
			bool cacheHasChanged = ShaderCache::HasChanged(m_AssetPath, source);

			m_ShaderSources = PreProcess(source);
			std::unordered_map<uint32, std::vector<uint32>> shaderData;
			CompileOrGetVulkanBinary(shaderData, forceCompile || cacheHasChanged);
			CompileOrGetOpenGLBinary(shaderData, forceCompile || cacheHasChanged);
			ReflectAllShaderStages(shaderData);

			HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Shader {0} loaded [+]", **m_AssetPath);
		}
		else
		{
			HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-] Shader {0} not found! [-]", **m_AssetPath);
		}
	}
	
	void OpenGLShader::Reflect(GLenum stage, std::vector<uint32> &data)
	{
		spirv_cross::Compiler compiler(data);
		spirv_cross::ShaderResources res = compiler.get_shader_resources();

		glUseProgram(m_RendererID);

		// Uniform Buffers

		for (const spirv_cross::Resource &resource : res.uniform_buffers)
		{
			auto &bufferType = compiler.get_type(resource.base_type_id);
			uint32 bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32 bufferSize = (uint32)compiler.get_declared_struct_size(bufferType);

			if (s_UniformBuffers.find(bindingPoint) == s_UniformBuffers.end())
			{
				ShaderUniformBuffer &buffer = s_UniformBuffers[bindingPoint];
				buffer.Name = resource.name;
				buffer.BindingPoint = bindingPoint;
				buffer.Size = bufferSize;
				
				glCreateBuffers(1, &buffer.RendererID);
				glBindBuffer(GL_UNIFORM_BUFFER, buffer.RendererID);
				glBufferData(GL_UNIFORM_BUFFER, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, buffer.BindingPoint, buffer.RendererID);

			#if PRINT_DEBUG_OUTPUTS
				HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Created Uniform Buffer at binding point {0} with name '{1}' and a size of {2} [+]", buffer.BindingPoint, *buffer.Name, buffer.Size);
			#endif // PRINT_DEBUG_OUTPUTS
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
			else
			{
				ShaderUniformBuffer &buffer = s_UniformBuffers.at(bindingPoint);
				HL_ASSERT(buffer.Name == resource.name);

				// the buffer has changed by OpenGL, we need to resize our representation to fit the actual data
				if (bufferSize > buffer.Size)
				{
					buffer.Size = bufferSize;

					// Recreate the buffer
					glDeleteBuffers(1, &buffer.RendererID);
					glCreateBuffers(1, &buffer.RendererID);
					glBindBuffer(GL_UNIFORM_BUFFER, buffer.RendererID);
					glBufferData(GL_UNIFORM_BUFFER, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
					glBindBufferBase(GL_UNIFORM_BUFFER, buffer.BindingPoint, buffer.RendererID);

				#if PRINT_DEBUG_OUTPUTS
					HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[=] Resized Uniform Buffer at binding point {0} with name '{1}' and a size of {2} [=]", buffer.BindingPoint, *buffer.Name, buffer.Size);
				#endif // PRINT_DEBUG_OUTPUTS
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}
			}
		}

		// Storage Buffers

		for (const spirv_cross::Resource &resource : res.storage_buffers)
		{
			const auto &bufferType = compiler.get_type(resource.base_type_id);
			const uint32 bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
			const uint32 bufferSize = (uint32)compiler.get_declared_struct_size(bufferType);

			if (s_StorageBuffers.find(bindingPoint) == s_StorageBuffers.end())
			{
				ShaderStorageBuffer &buffer = s_StorageBuffers[bindingPoint];
				buffer.Name = resource.name;
				buffer.BindingPoint = bindingPoint;
				buffer.Size = bufferSize;

				glCreateBuffers(1, &buffer.RendererID);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer.RendererID);
				glBufferData(GL_SHADER_STORAGE_BUFFER, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer.BindingPoint, buffer.RendererID);

			#if PRINT_DEBUG_OUTPUTS
				HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[+] Created Storage Buffer at binding point {0} with name '{1}' and a size of {2} [+]", buffer.BindingPoint, *buffer.Name, buffer.Size);
			#endif // PRINT_DEBUG_OUTPUTS

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}
			else
			{
				ShaderStorageBuffer &buffer = s_StorageBuffers.at(bindingPoint);
				HL_ASSERT(buffer.Name = resource.name);

				// the buffer has changed by OpenGL, we need to resize our representation to fit the actual data
				if (bufferSize > buffer.Size)
				{
					buffer.Size = bufferSize;

					// Recreate the buffer
					glDeleteBuffers(1, &buffer.RendererID);
					glCreateBuffers(1, &buffer.RendererID);
					glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer.RendererID);
					glBufferData(GL_SHADER_STORAGE_BUFFER, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
					glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer.BindingPoint, buffer.RendererID);

				#if PRINT_DEBUG_OUTPUTS
					HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[=] Resized Storage Buffer at binding point {0} with name '{1}' and a size of {2} [=]", buffer.BindingPoint, *buffer.Name, buffer.Size);
				#endif // PRINT_DEBUG_OUTPUTS

					glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				}
			}
		}

		// Samplers

		for (const spirv_cross::Resource &resource : res.sampled_images)
		{
			auto &type = compiler.get_type(resource.base_type_id);
			uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			const HLString &name = resource.name;
			uint32 dimensions = type.image.dim;

			int32 location = glGetUniformLocation(m_RendererID, *name);
			if (location == -1)
			{
				HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-] Could not find uniform location for {0} [-]", *name);
			//	HL_ASSERT(false);
			}

			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);
			glUniform1i(location, binding);
		}
	}

	void OpenGLShader::ReflectAllShaderStages(const std::unordered_map<uint32, std::vector<uint32>> &shaderData)
	{
		for (auto [stage, data] : shaderData)
		{
			Reflect(stage, data);
		}
	}
	
	void OpenGLShader::CompileOrGetVulkanBinary(std::unordered_map<uint32, std::vector<uint32>> &outputBinary, bool forceCompile)
	{
		FileSystemPath cacheDirectory = utils::GetCacheDirectory();

		for (auto &[stage, source] : m_ShaderSources)
		{
			const char *extension = utils::GLShaderStageCachedVulkanFileExtension(stage);

			if (m_ShaderSources[stage].IsEmpty())
				continue;

			if (!forceCompile)
			{
				FileSystemPath path = cacheDirectory / (m_AssetPath.Filename() + extension);

				FILE *f;
				fopen_s(&f, *path.String(), "rb");
				if (f)
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

			if ((outputBinary[stage].size() == 0 && !m_ShaderSources.at(stage).IsEmpty()))
			{
				shaderc::Compiler compiler;
				shaderc::CompileOptions options;

				options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
				options.AddMacroDefinition("OPENGL");
				const bool optimize = false;

				if (optimize)
					options.SetOptimizationLevel(shaderc_optimization_level_performance);

				// Compile Shader
				{
					auto &shaderSource = m_ShaderSources.at(stage);
					shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(*shaderSource, utils::GLShaderStageToShaderC(stage), **m_AssetPath, options);

					if (result.GetCompilationStatus() != shaderc_compilation_status_success)
					{
						HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-] {0} [-]", result.GetErrorMessage().c_str());
						HL_ASSERT(false);
					}

					outputBinary[stage] = std::vector<uint32>(result.cbegin(), result.cend());
				}

				// Cache compiled shader
				FileSystemPath path = cacheDirectory / (m_AssetPath.Filename() + extension);

				FILE *f;
				fopen_s(&f, *path.String(), "wb");
				if (f)
				{
					fwrite(outputBinary[stage].data(), sizeof(uint32), outputBinary[stage].size(), f);
					fclose(f);
				}
				else
				{
					HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-] Could not write into cache path {0} [-]", *path.String());
				}
			}
		}
	}
	
	void OpenGLShader::CompileOrGetOpenGLBinary(const std::unordered_map<uint32, std::vector<uint32>> &vulkanBinaries, bool forceCompile)
	{
		if (m_RendererID)
			glDeleteProgram(m_RendererID);

		GLuint program = glCreateProgram();
		m_RendererID = program;

		std::vector<GLuint> shaderRendererIds;
		shaderRendererIds.reserve(vulkanBinaries.size());

		FileSystemPath cacheDirectory = utils::GetCacheDirectory();

		m_ConstantBufferOffset = 0;
		std::vector<std::vector<uint32>> shaderData;
		for (auto [stage, binary] : vulkanBinaries)
		{
			if (binary.size() == 0)
				continue;

			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_opengl_compat, shaderc_env_version_opengl_4_5);

			{
				spirv_cross::CompilerGLSL glsl(binary);
				ParseConstantBuffers(glsl);

				FileSystemPath path = cacheDirectory / (m_AssetPath.Filename() + utils::GLShaderStageCachedOpenGLFileExtension(stage));
				std::vector<uint32> &shaderStageData = shaderData.emplace_back();

				if (!forceCompile)
				{
					FILE *f;
					fopen_s(&f, *path.String(), "rb");
					if (f)
					{
						// Get File size
						fseek(f, 0, SEEK_END);
						uint64 size = ftell(f);
						fseek(f, 0, SEEK_SET);

						shaderStageData = std::vector<uint32>(size / sizeof(uint32));
						fread(shaderStageData.data(), sizeof(uint32), shaderStageData.size(), f);
						fclose(f);
						f = nullptr;
					}
					else
					{
						HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-] Could not load Shader from cached binary ({0}), going to compile it from source... [-]", *path.String());
					}
				}

				if (!shaderStageData.size())
				{
					HLString source = glsl.compile();

				#if PRINT_SHADERS
					HL_CORE_INFO(GL_SHADER_LOG_PREFIX "===========================================================================\n");
					HL_CORE_INFO(GL_SHADER_LOG_PREFIX "[=] {0} Shader: [=] \n{1}\n", utils::GLShaderTypeToString(stage), *source);
					HL_CORE_INFO(GL_SHADER_LOG_PREFIX "===========================================================================\n");
				#endif // PRINT_SHADERS

					shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(*source, utils::GLShaderStageToShaderC(stage), **m_AssetPath, options);
					if (result.GetCompilationStatus() != shaderc_compilation_status_success)
					{
						HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-] {0} [-]", result.GetErrorMessage().c_str());
						HL_ASSERT(false);
					}

					shaderStageData = std::vector<uint32>(result.cbegin(), result.cend());

					{
						FILE *f;
						fopen_s(&f, *path.String(), "wb");
						if (f)
						{
							fwrite(shaderStageData.data(), sizeof(uint32), shaderStageData.size(), f);
							fclose(f);
							f = nullptr;
						}
						else
						{
							HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-] Could not write Shader into cache file: {0} [-]", *path.String());
						}
					}
				}

				GLuint shaderId = glCreateShader(stage);
				glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderStageData.data(), uint32(shaderStageData.size() * sizeof(uint32)));
				glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);
				glAttachShader(program, shaderId);

				shaderRendererIds.emplace_back(shaderId);
			}
		}

		// Link shader program
		glLinkProgram(program);

		int32 isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int32*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			int32 maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			
			// This check is here to prevent laptops (integrated gpus) to trigger this error
			// for some reason they seem to not set isLinked properly, so the maxLength will tell us if there was an error.
			if (maxLength > 0)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
				HL_CORE_ERROR(GL_SHADER_LOG_PREFIX "[-] Shader Linking failed ({0}):\n{1} [-]", **m_AssetPath, &infoLog[0]);

				glDeleteProgram(program);
				for (auto id : shaderRendererIds)
					glDeleteShader(id);
			}
		}
		
		for (auto id : shaderRendererIds)
			glDetachShader(program, id);

		// Get Uniform locations
		for (auto &[bufferName, buffer] : m_Buffers)
		{
			for (auto &[name, uniform] : buffer.Uniforms)
			{
				int32 location = GetUniformLocation(name);
				if (location != -1)
				{
				#if PRINT_DEBUG_OUTPUTS
					HL_CORE_INFO("Registering Uniform {0} at location {1}", *name, location);
				#endif // PRINT_DEBUG_OUTPUTS

					m_UniformLocations[name] = location;
				}
			}
		}
	}
	
	std::unordered_map<GLenum, HLString> OpenGLShader::PreProcess(const HLString &source)
	{
		std::unordered_map<GLenum, HLString> result;
		std::unordered_map<GLenum, std::stringstream> temp;
		std::stringstream s(*source);
		std::string line;
		GLenum type = GL_NONE;

		// Parse Shader
		temp[GL_VERTEX_SHADER] = std::stringstream();
		temp[GL_TESS_CONTROL_SHADER] = std::stringstream();
		temp[GL_TESS_EVALUATION_SHADER] = std::stringstream();
		temp[GL_GEOMETRY_SHADER] = std::stringstream();
		temp[GL_FRAGMENT_SHADER] = std::stringstream();
		temp[GL_COMPUTE_SHADER] = std::stringstream();

		while (std::getline(s, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = GL_VERTEX_SHADER;
				else if (line.find("tess_control") != std::string::npos)
					type = GL_TESS_CONTROL_SHADER;
				else if (line.find("tess_eval") != std::string::npos)
					type = GL_TESS_EVALUATION_SHADER;
				else if (line.find("geometry") != std::string::npos)
					type = GL_GEOMETRY_SHADER;
				else if (line.find("pixel") != std::string::npos || line.find("fragment") != std::string::npos)
					type = GL_FRAGMENT_SHADER;
				else if (line.find("compute") != std::string::npos)
					type = GL_COMPUTE_SHADER;
			}
			else
			{
				temp[type] << line << "\n";
			}
		}

		result[GL_VERTEX_SHADER] = HLString(temp[GL_VERTEX_SHADER].str().c_str());
		result[GL_TESS_CONTROL_SHADER] = HLString(temp[GL_TESS_CONTROL_SHADER].str().c_str());
		result[GL_TESS_EVALUATION_SHADER] = HLString(temp[GL_TESS_EVALUATION_SHADER].str().c_str());
		result[GL_GEOMETRY_SHADER] = HLString(temp[GL_GEOMETRY_SHADER].str().c_str());
		result[GL_FRAGMENT_SHADER] = HLString(temp[GL_FRAGMENT_SHADER].str().c_str());
		result[GL_COMPUTE_SHADER] = HLString(temp[GL_COMPUTE_SHADER].str().c_str());

		// Replace \r\n with \n
		result[GL_VERTEX_SHADER] = result[GL_VERTEX_SHADER].Replace("\r\n", "\n");
		result[GL_TESS_CONTROL_SHADER] = result[GL_TESS_CONTROL_SHADER].Replace("\r\n", "\n");
		result[GL_TESS_EVALUATION_SHADER] = result[GL_TESS_EVALUATION_SHADER].Replace("\r\n", "\n");
		result[GL_GEOMETRY_SHADER] = result[GL_GEOMETRY_SHADER].Replace("\r\n", "\n");
		result[GL_FRAGMENT_SHADER] = result[GL_FRAGMENT_SHADER].Replace("\r\n", "\n");
		result[GL_COMPUTE_SHADER] = result[GL_COMPUTE_SHADER].Replace("\r\n", "\n");

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

			ShaderUniformStruct &shaderStruct = m_ShaderUniformStructs.emplace_back();
			shaderStruct.Name = name;

			for (uint32 i = 0; i < memberCount; ++i)
			{
				auto memberType = compiler.get_type(type.member_types[i]);
				const HLString &memberName = compiler.get_member_name(type.self, i);
				uint32 size = (uint32)compiler.get_declared_struct_member_size(type, i);
				uint32 offset = compiler.type_struct_member_offset(type, i) - bufferOffset;

				ShaderUniformStructMember &member = shaderStruct.Members.emplace_back();
				member.Name = memberName;
				member.Size = size;
				member.Offset = offset;
				member.Type = utils::SpirvTypeToShaderUniformType(memberType);

				HLString uniformName = fmt::format("{}.{}", *name, *memberName);

			#if PRINT_DEBUG_OUTPUTS
				HL_CORE_INFO("Registering push_constant with uniform name {0}", *uniformName);
			#endif // PRINT_DEBUG_OUTPUTS

				buffer.Uniforms[uniformName] = ShaderUniform(uniformName, utils::SpirvTypeToShaderUniformType(type), size, offset);
			}

			m_ConstantBufferOffset += bufferSize;
		}
	}
	
	int32 OpenGLShader::GetUniformLocation(const HLString &name) const
	{
		int32 result = glGetUniformLocation(m_RendererID, *name);
		if (result == -1)
			HL_CORE_WARN(GL_SHADER_LOG_PREFIX "[-] Could not find uniform location {0} [-]", *name);

		return result;
	}
	
	GLenum OpenGLShader::ShaderTypeFromString(const HLString &type)
	{
		HLString typeToCheck = type.ToLowerCase();
		if (typeToCheck == "vertex")
			return GL_VERTEX_SHADER;

		if (typeToCheck == "fragment" || typeToCheck == "pixel")
			return GL_FRAGMENT_SHADER;

		if (typeToCheck == "compute")
			return GL_COMPUTE_SHADER;

		if (typeToCheck == "geometry")
			return GL_GEOMETRY_SHADER;

		if (typeToCheck == "tesscontrol" || typeToCheck == "tesselationcontrol")
			return GL_TESS_CONTROL_SHADER;

		if (typeToCheck == "tesseval"
		 || typeToCheck == "tesselationeval"
		 || typeToCheck == "tessevaluation"
		 || typeToCheck == "tesselationevaluation")
			return GL_TESS_EVALUATION_SHADER;

		return GL_NONE;
	}
	
	void OpenGLShader::UploadUniformInt(uint32 location, int32 value)
	{
		glUniform1i(location, value);
	}
	
	void OpenGLShader::UploadUniformIntArray(uint32 location, int32 *values, int32 count)
	{
		glUniform1iv(location, count, values);
	}
	
	void OpenGLShader::UploadUniformFloat(uint32 location, float value)
	{
		glUniform1f(location, value);
	}
	
	void OpenGLShader::UploadUniformFloat2(uint32 location, const glm::vec2 &value)
	{
		glUniform2f(location, value.x, value.y);
	}
	
	void OpenGLShader::UploadUniformFloat3(uint32 location, const glm::vec3 &value)
	{
		glUniform3f(location, value.x, value.y, value.z);
	}
	
	void OpenGLShader::UploadUniformFloat4(uint32 location, const glm::vec4 &value)
	{
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat2(uint32 location, const glm::mat2 &values)
	{
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(values));
	}
	
	void OpenGLShader::UploadUniformMat3(uint32 location, const glm::mat3 &values)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(values));
	}
	
	void OpenGLShader::UploadUniformMat4(uint32 location, const glm::mat4 &values)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(values));
	}
	
	void OpenGLShader::UploadUniformMat4Array(uint32 location, const glm::mat4 &values, uint32 count)
	{
		glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(values));
	}
	
	void OpenGLShader::UploadUniformInt(const HLString &name, int32 value)
	{
		int32 location = GetUniformLocation(name);
		glUniform1i(location, value);
	}
	
	void OpenGLShader::UploadUniformUInt(const HLString &name, uint32 value)
	{
		int32 location = GetUniformLocation(name);
		glUniform1ui(location, value);
	}
	
	void OpenGLShader::UploadUniformIntArray(const HLString &name, int32 *values, uint32 count)
	{
		int32 location = GetUniformLocation(name);
		glUniform1iv(location, count, values);
	}
	
	void OpenGLShader::UploadUniformFloat(const HLString &name, float value)
	{
		int32 location = GetUniformLocation(name);
		glUniform1f(location, value);
	}
	
	void OpenGLShader::UploadUniformFloat2(const HLString &name, const glm::vec2 &value)
	{
		int32 location = GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}
	
	void OpenGLShader::UploadUniformFloat3(const HLString &name, const glm::vec3 &value)
	{
		int32 location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}
	
	void OpenGLShader::UploadUniformFloat4(const HLString &name, const glm::vec4 &value)
	{
		int32 location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat2(const HLString &name, const glm::mat2 &value)
	{
		int32 location = GetUniformLocation(name);
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat3(const HLString &name, const glm::mat3 &value)
	{
		int32 location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void OpenGLShader::UploadUniformMat4(const HLString &name, const glm::mat4 &value)
	{
		int32 location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

#endif

