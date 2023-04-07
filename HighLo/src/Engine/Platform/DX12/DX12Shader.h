// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12Shader : public Shader
	{
	public:

		DX12Shader(const FileSystemPath &filePath, bool forceCompile);
		DX12Shader(const HLString &source, const HLString &name = "undefined", ShaderLanguage language = ShaderLanguage::GLSL);
		virtual ~DX12Shader();

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
		virtual const ShaderResourceDeclaration *GetResource(const HLString &name) const override;

	private:

		void Load(const HLString &source, bool forceCompile = false);

	private:

		HLRendererID m_RendererID = 0;
		HLString m_Name;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		FileSystemPath m_AssetPath;
		ShaderLanguage m_Language;

		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;

		std::unordered_map<uint32, HLString> m_ShaderSources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;
	};
}

#endif // HIGHLO_API_DX12

