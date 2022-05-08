// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-10-16) fixed indentations and added verification check whether the shader file exists
//     - 1.1 (2021-09-15) Changed m_Name implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Graphics/Shaders/ShaderPreProcessor.h"
#include "Engine/Core/Defines/BaseTypes.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>
#include <spirv_cross/spirv_glsl.hpp>

#include "OpenGLShaderDefs.h"

namespace highlo
{
	class OpenGLShader : public Shader
	{
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
		static void ClearUniformBuffers();

	private:

		void Load(const HLString &source, bool forceCompile);
		
		// Pre-Processing
		std::unordered_map<GLenum, HLString> PreProcess(const HLString &source);
		std::unordered_map<GLenum, HLString> PreProcessGLSL(const HLString &source);
		std::unordered_map<GLenum, HLString> PreProcessHLSL(const HLString &source);

		// Shader-Reflection
		void ReflectAllShaderStages(const std::unordered_map<GLenum, std::vector<uint32>> &shaderData);
		void Reflect(GLenum shaderStage, const std::vector<uint32> &shaderData);

		// Shader-compilation
		HLString Compile(std::unordered_map<GLenum, std::vector<uint32>> &outputBinary, const GLenum stage) const;
		void CompileOrGetOpenGLBinary(std::unordered_map<GLenum, std::vector<uint32>> &outBinary, bool forceCompile);
		void TryGetOpenGLCachedBinary(const FileSystemPath &cacheDirectory, const HLString &extension, std::unordered_map<GLenum, std::vector<uint32>> &outBinary, GLenum stage) const;
		void LoadAndCreateShaders(const std::unordered_map<GLenum, std::vector<uint32>> &shaderData);

		void ParseConstantBuffers(const spirv_cross::CompilerGLSL &compiler);

	private:

		HLString m_Name;
		HLRendererID m_RendererID = 0;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		FileSystemPath m_AssetPath;
		ShaderLanguage m_Language = ShaderLanguage::None;
		uint32 m_ConstantBufferOffset = 0;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;
		std::vector<OpenGLShaderDescriptorSet> m_ShaderDescriptorSets;
		std::vector<OpenGLShaderPushConstantRange> m_PushConstantRanges;

		std::unordered_map<uint32, HLString> m_ShaderSources;
		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;

		std::map<GLenum, StageData> m_StagesMetaData;
	};
}

#endif

