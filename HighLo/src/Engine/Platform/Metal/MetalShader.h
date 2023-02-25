// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/Shaders/Shader.h"

namespace highlo
{
	class MetalShader : public Shader
	{
	public:

		MetalShader(const FileSystemPath &filePath, bool forceCompile = false);
		MetalShader(const HLString &source, const HLString &name = "undefined", ShaderLanguage language = ShaderLanguage::GLSL);
		virtual ~MetalShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override { return m_AssetPath.Hash(); }

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_Resources; }
		virtual const ShaderResourceDeclaration *GetResource(const HLString &name) const override;

		virtual void SetMacro(const HLString &name, const HLString &value) override;

	private:

		void Load(const HLString &source, bool forceCompile);

	private:

		HLString m_Name;
		HLRendererID m_RendererID = 0;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		FileSystemPath m_AssetPath = "";
		ShaderLanguage m_Language = ShaderLanguage::None;
		uint32 m_ConstantBufferOffset = 0;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_map<HLString, int32> m_UniformLocations;
		std::unordered_set<HLString> m_AcknowledgedMacros;
	//	std::vector<OpenGLShaderDescriptorSet> m_ShaderDescriptorSets;
	//	std::vector<OpenGLShaderPushConstantRange> m_PushConstantRanges;

		std::unordered_map<uint32, HLString> m_ShaderSources;
		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;

	//	std::map<GLenum, StageData> m_StagesMetaData;
	};
}

#endif // HIGHLO_API_METAL

