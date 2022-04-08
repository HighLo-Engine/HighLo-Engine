// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-10-16) fixed indentations and added verification check whether the shader file exists
//     - 1.1 (2021-09-15) Changed m_Name implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Core/Defines/BaseTypes.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>
#include <spirv_cross/spirv_glsl.hpp>

namespace highlo
{
	class OpenGLShader : public Shader
	{
	public:

		struct PushConstantRange
		{
			uint32 Offset = 0;
			uint32 Size = 0;
		};

		struct ShaderUniformStructMember
		{
			HLString Name;
			uint32 Size;
			uint32 Offset;
			ShaderUniformType Type;
		};

		struct ShaderUniformStruct
		{
			HLString Name;
			std::vector<ShaderUniformStructMember> Members;
		};

	public:

		OpenGLShader(const FileSystemPath &filePath, bool forceCompile);
		OpenGLShader(const HLString &source);
		virtual ~OpenGLShader();

		// Inherited via Shader
		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override;

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual void SetMacro(const HLString &name, const HLString &value) override;

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_Resources; }

		// OpenGL-specific
		static const ShaderUniformBuffer &FindUniformBuffer(const HLString &name);
		static void SetUniformBuffer(const ShaderUniformBuffer &buffer, const void *data, uint32 size, uint32 offset = 0);
		static void SetStorageBuffer(const ShaderStorageBuffer &buffer, const void *data, uint32 size, uint32 offset = 0);

		virtual void SetUniformBuffer(const HLString &name, const void *data, uint32 size);
		virtual void SetStorageBuffer(const HLString &name, const void *data, uint32 size);

		virtual void ResizeStorageBuffer(uint32 bindingPoint, uint32 newSize);

		virtual void SetUniform(const HLString &fullname, float value);
		virtual void SetUniform(const HLString &fullname, int32 value);
		virtual void SetUniform(const HLString &fullname, const glm::ivec2 &value);
		virtual void SetUniform(const HLString &fullname, const glm::ivec3 &value);
		virtual void SetUniform(const HLString &fullname, const glm::ivec4 &value);
		virtual void SetUniform(const HLString &fullname, uint32 value);
		virtual void SetUniform(const HLString &fullname, const glm::vec2 &value);
		virtual void SetUniform(const HLString &fullname, const glm::vec3 &value);
		virtual void SetUniform(const HLString &fullname, const glm::vec4 &value);
		virtual void SetUniform(const HLString &fullname, const glm::mat2 &value);
		virtual void SetUniform(const HLString &fullname, const glm::mat3 &value);
		virtual void SetUniform(const HLString &fullname, const glm::mat4 &value);

		const ShaderResourceDeclaration *GetShaderResource(const HLString &name);
		std::vector<ShaderUniformStruct> &GetUniformStructs() { return m_ShaderUniformStructs; }
		const std::vector<ShaderUniformStruct> &GetUniformStructs() const { return m_ShaderUniformStructs; }

		static void ClearUniformBuffers();

	private:

		void Load(const HLString &source, bool forceCompile);
		void Reflect(GLenum stage, std::vector<uint32> &data);
		void ReflectAllShaderStages(const std::unordered_map<uint32, std::vector<uint32>> &shaderData);

		void CompileOrGetVulkanBinary(std::unordered_map<uint32, std::vector<uint32>> &outputBinary, bool forceCompile = false);
		void CompileOrGetOpenGLBinary(const std::unordered_map<uint32, std::vector<uint32>> &, bool forceCompile = false);

		std::unordered_map<GLenum, HLString> PreProcess(const HLString &source);

		void ParseConstantBuffers(const spirv_cross::CompilerGLSL &compiler);

		int32 GetUniformLocation(const HLString &name) const;

		static GLenum ShaderTypeFromString(const HLString &type);

		void UploadUniformInt(uint32 location, int32 value);
		void UploadUniformIntArray(uint32 location, int32 *values, int32 count);
		void UploadUniformFloat(uint32 location, float value);
		void UploadUniformFloat2(uint32 location, const glm::vec2 &value);
		void UploadUniformFloat3(uint32 location, const glm::vec3 &value);
		void UploadUniformFloat4(uint32 location, const glm::vec4 &value);
		void UploadUniformMat2(uint32 location, const glm::mat2 &values);
		void UploadUniformMat3(uint32 location, const glm::mat3 &values);
		void UploadUniformMat4(uint32 location, const glm::mat4 &values);
		void UploadUniformMat4Array(uint32 location, const glm::mat4 &values, uint32 count);

		void UploadUniformInt(const HLString &name, int32 value);
		void UploadUniformUInt(const HLString &name, uint32 value);
		void UploadUniformIntArray(const HLString &name, int32 *values, uint32 count);

		void UploadUniformFloat(const HLString &name, float value);
		void UploadUniformFloat2(const HLString &name, const glm::vec2 &value);
		void UploadUniformFloat3(const HLString &name, const glm::vec3 &value);
		void UploadUniformFloat4(const HLString &name, const glm::vec4 &value);

		void UploadUniformMat2(const HLString &name, const glm::mat2 &value);
		void UploadUniformMat3(const HLString &name, const glm::mat3 &value);
		void UploadUniformMat4(const HLString &name, const glm::mat4 &value);

	private:

		HLString m_Name;
		HLRendererID m_RendererID = 0;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		FileSystemPath m_AssetPath;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;

		uint32 m_ConstantBufferOffset = 0;
		inline static std::unordered_map<uint32, ShaderUniformBuffer> s_UniformBuffers;
		inline static std::unordered_map<uint32, ShaderStorageBuffer> s_StorageBuffers;

		std::unordered_map<uint32, HLString> m_ShaderSources;
		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::unordered_map<HLString, int32> m_UniformLocations;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;
		std::vector<PushConstantRange> m_PushConstantRanges;
		std::vector<ShaderUniformStruct> m_ShaderUniformStructs;
	};
}

#endif

