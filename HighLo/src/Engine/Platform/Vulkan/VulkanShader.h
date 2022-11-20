// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source, const HLString &name = "undefined", ShaderLanguage language = ShaderLanguage::GLSL);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual void Release() override;
		virtual uint64 GetHash() const override;

		virtual void Bind() const override {}
		virtual void Unbind() override {}

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override;
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override;

		virtual void SetMacro(const HLString &name, const HLString &value) override;

	private:

		FileSystemPath m_FilePath;
		HLString m_Name;
		HLRendererID m_RendererID = 0;
	};
}

