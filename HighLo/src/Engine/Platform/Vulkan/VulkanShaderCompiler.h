// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "Engine/Renderer/ShaderCompiler.h"

namespace highlo
{
	class VulkanShaderCompiler : public ShaderCompiler
	{
	public:

		VulkanShaderCompiler(const FileSystemPath &filePath, const BufferLayout &layout);
		virtual ~VulkanShaderCompiler();

		virtual void Compile() override;
		virtual void Link() override;

		virtual const ShaderSource &GetShaderSource() const override { return m_Source; }
		virtual const BufferLayout &GetLayout() const override { return m_Layout; }
		virtual const FileSystemPath &GetFilePath() const override { return m_FilePath; }

	private:

		uint32 CompileGLSLShader(const char *code, uint32 type);

		ShaderSource m_Source;
		BufferLayout m_Layout;
		FileSystemPath m_FilePath;
	};
}

#endif // HIGHLO_API_VULKAN

