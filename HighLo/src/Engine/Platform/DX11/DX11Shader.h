// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once
#include "Engine/Graphics/Shaders/Shader.h"

#ifdef HIGHLO_API_DX11

#include "DX11Resources.h"

namespace highlo
{
	class DX11Shader : public Shader
	{
	public:

		DX11Shader(const FileSystemPath &filePath, bool forceCompile = false, ShaderLanguage language = ShaderLanguage::None);
		DX11Shader(const HLString &source);
		virtual ~DX11Shader();

		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override { return m_AssetPath.Hash(); }

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual void SetMacro(const HLString &name, const HLString &value) override;

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_Resources; }

	private:

		HLString m_Name;
		HLRendererID m_RendererID = 0;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		FileSystemPath m_AssetPath;
		ShaderLanguage m_Language = ShaderLanguage::None;

		std::unordered_map<HLString, HLString> m_Macros;
		std::unordered_set<HLString> m_AcknowledgedMacros;

		std::unordered_map<uint32, HLString> m_ShaderSources;
		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;
	};
}

#endif
