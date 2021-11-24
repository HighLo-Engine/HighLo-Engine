// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "ShaderSource.h"
#include "Engine/Renderer/BufferLayout.h"
#include "Shader.h"
#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX_SHADER = 0,
		TESS_CONTROL_SHADER = 2,
		TESS_EVAL_SHADER = 3,
		GEOMETRY_SHADER = 4,
		PIXEL_SHADER = 5,
		COMPUTE_SHADER = 6
	};

	enum class ShaderCompilerState
	{
		None = 0,	// Compiler has been instantiated
		Ready,		// ShaderSource has been loaded
		Compiled,	// Shaders have been compiled
		Linked		// Shaders have been linked, GetShaderSource can be called
	};

	class ShaderCompiler : public IsSharedReference
	{
	public:

		HLAPI virtual ~ShaderCompiler() = default;

		HLAPI virtual void Compile() = 0;
		HLAPI virtual void Link() = 0;

		HLAPI virtual const ShaderSource &GetShaderSource() const = 0;
		HLAPI virtual const BufferLayout &GetLayout() const = 0;
		HLAPI virtual const FileSystemPath &GetFilePath() const = 0;

		HLAPI bool IsCompiled() const { return m_State == ShaderCompilerState::Compiled; }
		HLAPI bool IsLinked() const { return m_State == ShaderCompilerState::Linked; }
		HLAPI bool IsReady() const { return m_State == ShaderCompilerState::Ready; }
		HLAPI Ref<Shader> GetCompiledShader() { return IsLinked() ? m_Shader : nullptr; }

		HLAPI static Ref<ShaderCompiler> Create(const FileSystemPath &filePath, const BufferLayout &layout);

	protected:

		ShaderCompilerState m_State = ShaderCompilerState::None;
		Ref<Shader> m_Shader = nullptr;
	};
}

