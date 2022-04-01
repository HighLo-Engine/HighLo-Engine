#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const FileSystemPath &filePath, bool forceCompile = false);
		VulkanShader(const HLString &source);
		virtual ~VulkanShader();

		virtual void Reload(bool forceCompile = false) override;
		virtual uint64 GetHash() const override { return m_AssetPath.Hash(); }

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) override;
		virtual const HLString &GetName() const override { return m_Name; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const override { return m_Resources; }

	private:

		HLRendererID m_RendererID = 0;
		HLString m_Name;
		FileSystemPath m_AssetPath;

		std::unordered_map<uint32, HLString> m_ShaderSources;

		std::unordered_map<HLString, ShaderBuffer> m_Buffers;
		std::unordered_map<HLString, ShaderResourceDeclaration> m_Resources;
		std::vector<ShaderReloadedCallback> m_ReloadedCallbacks;
	};
}

#endif // HIGHLO_API_VULKAN