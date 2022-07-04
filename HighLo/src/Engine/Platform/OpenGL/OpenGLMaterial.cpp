#include "HighLoPch.h"
#include "OpenGLMaterial.h"

#ifdef HIGHLO_API_OPENGL

#include "OpenGLShader.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTexture3D.h"

#include <glm/gtc/type_ptr.hpp>

namespace highlo
{
	namespace utils
	{
		static GLenum GetUniformLocation(const Ref<Shader> &shader, const HLString &name)
		{
			int32 location = glGetUniformLocation(shader->GetRendererID(), *name);
			if (location == -1)
			{
				HL_CORE_WARN("Could not retrieve uniform location for {0}", *name);
				return 0;
			}

			return location;
		}

		static GLuint GetUniformBufferBlockIndex(const Ref<Shader> &shader, const HLString &name)
		{
			uint32 location = glGetUniformBlockIndex(shader->GetRendererID(), *name);
			if (location == GL_INVALID_INDEX)
			{
				HL_CORE_WARN("Could not retrieve uniform block index for {0}", *name);
				return 0;
			}

			return location;
		}
	}

	OpenGLMaterial::OpenGLMaterial(const Ref<Shader> &shader, const HLString &name)
		: m_Shader(shader), m_Name(name)
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&OpenGLMaterial::OnShaderReloaded, this));
		AllocateStorage();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;
	}
	
	OpenGLMaterial::OpenGLMaterial(const Ref<Material> &other, const HLString &name)
		: m_Name(name), m_Shader(other->GetShader())
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&OpenGLMaterial::OnShaderReloaded, this));
		if (name.IsEmpty())
			m_Name = other->GetShader()->GetName();

		m_Flags |= (uint32)MaterialFlag::DepthTest;
		m_Flags |= (uint32)MaterialFlag::Blend;
	}

	OpenGLMaterial::~OpenGLMaterial()
	{
	}
	
	void OpenGLMaterial::Invalidate()
	{
	}
	
	void OpenGLMaterial::Set(const HLString &name, float value)
	{
		Set<float>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, int32 value)
	{
		Set<int32>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, uint32 value)
	{
		Set<uint32>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, bool value)
	{
		Set<bool>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::vec2 &value)
	{
		Set<glm::vec2>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::vec3 &value)
	{
		Set<glm::vec3>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::vec4 &value)
	{
		Set<glm::vec4>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::ivec2 &value)
	{
		Set<glm::ivec2>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::ivec3 &value)
	{
		Set<glm::ivec3>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::ivec4 &value)
	{
		Set<glm::ivec4>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::mat2 &value)
	{
		Set<glm::mat2>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::mat3 &value)
	{
		Set<glm::mat3>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const glm::mat4 &value)
	{
		Set<glm::mat4>(name, value);
	}
	
	void OpenGLMaterial::Set(const HLString &name, const Ref<Texture2D> &texture)
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

	void OpenGLMaterial::Set(const HLString &name, const Ref<Texture2D> &texture, uint32 slot)
	{
		auto decl = FindResourceDeclaration(name);
		if (!decl)
		{
			HL_CORE_WARN("Cannot find material property: {0}", *name);
			return;
		}

		m_Texture2Ds[slot] = texture;
	}

	void OpenGLMaterial::Set(const HLString &name, const Ref<Texture3D> &texture)
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
	
	float &OpenGLMaterial::GetFloat(const HLString &name)
	{
		return Get<float>(name);
	}
	
	int32 &OpenGLMaterial::GetInt(const HLString &name)
	{
		return Get<int32>(name);
	}
	
	uint32 &OpenGLMaterial::GetUInt(const HLString &name)
	{
		return Get<uint32>(name);
	}

	bool &OpenGLMaterial::GetBool(const HLString &name)
	{
		return Get<bool>(name);
	}
	
	glm::vec2 &OpenGLMaterial::GetVector2(const HLString &name)
	{
		return Get<glm::vec2>(name);
	}
	
	glm::vec3 &OpenGLMaterial::GetVector3(const HLString &name)
	{
		return Get<glm::vec3>(name);
	}
	
	glm::vec4 &OpenGLMaterial::GetVector4(const HLString &name)
	{
		return Get<glm::vec4>(name);
	}
	
	glm::ivec2 &OpenGLMaterial::GetIVector2(const HLString &name)
	{
		return Get<glm::ivec2>(name);
	}
	
	glm::ivec3 &OpenGLMaterial::GetIVector3(const HLString &name)
	{
		return Get<glm::ivec3>(name);
	}
	
	glm::ivec4 &OpenGLMaterial::GetIVector4(const HLString &name)
	{
		return Get<glm::ivec4>(name);
	}
	
	glm::mat2 &OpenGLMaterial::GetMatrix2(const HLString &name)
	{
		return Get<glm::mat2>(name);
	}
	
	glm::mat3 &OpenGLMaterial::GetMatrix3(const HLString &name)
	{
		return Get<glm::mat3>(name);
	}
	
	glm::mat4 &OpenGLMaterial::GetMatrix4(const HLString &name)
	{
		return Get<glm::mat4>(name);
	}
	
	Ref<Texture2D> OpenGLMaterial::GetTexture2D(const HLString &name)
	{
		return GetResource<Texture2D>(name);
	}
	
	Ref<Texture3D> OpenGLMaterial::GetTexture3D(const HLString &name)
	{
		return GetResource<Texture3D>(name);
	}
	
	Ref<Texture2D> OpenGLMaterial::TryGetTexture2D(const HLString &name)
	{
		return TryGetResource<Texture2D>(name);
	}
	
	Ref<Texture3D> OpenGLMaterial::TryGetTexture3D(const HLString &name)
	{
		return TryGetResource<Texture3D>(name);
	}
	
	bool OpenGLMaterial::GetFlag(MaterialFlag flag) const
	{
		return (uint32)flag & m_Flags;
	}

	void OpenGLMaterial::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
			m_Flags |= (uint32)flag;
		else
			m_Flags &= ~(uint32)flag;
	}

	void OpenGLMaterial::UpdateForRendering(const Ref<UniformBufferSet> &uniformBufferSet)
	{
		Ref<OpenGLShader> shader = m_Shader.As<OpenGLShader>();
		shader->Bind();

		const auto &shaderBuffers = GetShader()->GetShaderBuffers();
		if (shaderBuffers.size() > 0)
		{
			for (auto &[bufferName, buffer] : shaderBuffers)
			{
				for (auto &[name, uniform] : buffer.Uniforms)
				{
					uint32 location = utils::GetUniformBufferBlockIndex(m_Shader, buffer.Name);
					HL_CORE_TRACE("Trying to bind uniform buffer block {0} with location {1} and binding {2}", *buffer.Name, location, uniform.GetBinding());
					glUniformBlockBinding(m_Shader->GetRendererID(), location, uniform.GetBinding());
				}
			}
		}


		// TODO: This is a workaround until we fully switch to uniform buffer blocks
		/*
		if (shaderBuffers.size() > 0)
		{
			for (auto &[bufferName, buffer] : shaderBuffers)
			{
				for (auto &[name, uniform] : buffer.Uniforms)
				{
					switch (uniform.GetType())
					{
						case ShaderUniformType::Bool:
						case ShaderUniformType::Uint:
						{
							const uint32 value = m_LocalData.Read<uint32>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform1ui(location, value);
							break;
						}

						case ShaderUniformType::Int:
						{
							const int32 value = m_LocalData.Read<int32>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform1i(location, value);
							break;
						}

						case ShaderUniformType::Float:
						{
							const float value = m_LocalData.Read<float>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform1f(location, value);
							break;
						}

						case ShaderUniformType::IVec2:
						{
							const glm::ivec2 &value = m_LocalData.Read<glm::ivec2>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform2i(location, value.x, value.y);
							break;
						}

						case ShaderUniformType::IVec3:
						{
							const glm::ivec3 &value = m_LocalData.Read<glm::ivec3>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform3i(location, value.x, value.y, value.z);
							break;
						}

						case ShaderUniformType::IVec4:
						{
							const glm::ivec4 &value = m_LocalData.Read<glm::ivec4>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform4i(location, value.x, value.y, value.z, value.w);
							break;
						}

						case ShaderUniformType::Vec2:
						{
							const glm::vec2 &value = m_LocalData.Read<glm::vec2>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform2f(location, value.x, value.y);
							break;
						}

						case ShaderUniformType::Vec3:
						{
							const glm::vec3 &value = m_LocalData.Read<glm::vec3>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform3f(location, value.x, value.y, value.z);
							break;
						}

						case ShaderUniformType::Vec4:
						{
							const glm::vec4 &value = m_LocalData.Read<glm::vec4>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniform4f(location, value.x, value.y, value.z, value.w);
							break;
						}

						case ShaderUniformType::Mat2:
						{
							const glm::mat2 &value = m_LocalData.Read<glm::mat2>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
							break;
						}

						case ShaderUniformType::Mat3:
						{
							const glm::mat3 &value = m_LocalData.Read<glm::mat3>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
							break;
						}

						case ShaderUniformType::Mat4:
						{
							const glm::mat4 &value = m_LocalData.Read<glm::mat4>(uniform.GetOffset());
							int32 location = utils::GetUniformLocation(m_Shader, name);
							if (location)
								glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
							break;
						}

						case ShaderUniformType::Struct:
						{
							std::vector<OpenGLShader::ShaderUniformStruct> structs = shader->GetUniformStructs();
							for (OpenGLShader::ShaderUniformStruct &currentStruct : structs)
							{
								std::vector<OpenGLShader::ShaderUniformStructMember> members = currentStruct.Members;
								for (OpenGLShader::ShaderUniformStructMember &currentMember : members)
								{
									HLString uniformName = fmt::format("{}.{}", currentStruct.Name, currentMember.Name);

									switch (currentMember.Type)
									{
									case ShaderUniformType::Float:
									{
										const float value = m_LocalData.Read<float>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Int:
									{
										const int32 value = m_LocalData.Read<int32>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Uint:
									case ShaderUniformType::Bool:
									{
										const uint32 value = m_LocalData.Read<uint32>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Vec2:
									{
										const glm::vec2 &value = m_LocalData.Read<glm::vec2>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Vec3:
									{
										const glm::vec3 &value = m_LocalData.Read<glm::vec3>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Vec4:
									{
										const glm::vec4 &value = m_LocalData.Read<glm::vec4>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::IVec2:
									{
										const glm::ivec2 &value = m_LocalData.Read<glm::ivec2>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::IVec3:
									{
										const glm::ivec3 &value = m_LocalData.Read<glm::ivec3>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::IVec4:
									{
										const glm::ivec4 &value = m_LocalData.Read<glm::ivec4>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Mat2:
									{
										const glm::mat2 &value = m_LocalData.Read<glm::mat2>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Mat3:
									{
										const glm::mat3 &value = m_LocalData.Read<glm::mat3>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									case ShaderUniformType::Mat4:
									{
										const glm::mat4 &value = m_LocalData.Read<glm::mat4>(uniform.GetOffset());
										shader->SetUniform(uniformName, value);
										break;
									}

									default:
									{
										HL_ASSERT(false);
										break;
									}
								}
							}
						}

						break;
						}

						default:
						{
							HL_ASSERT(false);
							break;
						}
					}
				}
			}
		}
		*/


		for (uint32 i = 0; i < (uint32)m_Textures.size(); ++i)
		{
			auto &texture = m_Textures[i];
			if (texture)
			{
				HL_ASSERT(texture->GetType() == TextureType::Texture3D);
				Ref<OpenGLTexture3D> glTexture = texture.As<OpenGLTexture3D>();
				glBindTextureUnit(i, glTexture->GetRendererID());
			}
		}

		for (auto &[textureSlot, texture] : m_Texture2Ds)
		{
			if (texture)
			{
				Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();
				if (glTexture->GetSamplerRendererID() != 0)
					glBindSampler(textureSlot, glTexture->GetSamplerRendererID());
				glBindTextureUnit(textureSlot, glTexture->GetRendererID());
			}
		}
	}

	void OpenGLMaterial::AllocateStorage()
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

	void OpenGLMaterial::OnShaderReloaded()
	{
		// TODO
		HL_CORE_INFO("Reloading material {0}", *m_Name);
	}

	const ShaderUniform *OpenGLMaterial::FindUniformDeclaration(const HLString &name)
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

	const ShaderResourceDeclaration *OpenGLMaterial::FindResourceDeclaration(const HLString &name)
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

#endif // HIGHLO_API_OPENGL

