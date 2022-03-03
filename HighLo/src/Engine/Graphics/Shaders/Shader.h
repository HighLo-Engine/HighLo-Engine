// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <map>

#include "Engine/Core/FileSystemPath.h"
#include "ShaderUniform.h"
#include "ShaderBuffers.h"

namespace highlo
{
	class Shader : public IsSharedReference
	{
	public:

		using ShaderReloadedCallback = std::function<void()>;

		HLAPI virtual void Reload(bool forceCompile = false) = 0;
		HLAPI virtual uint64 GetHash() const = 0;

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() = 0;

		HLAPI virtual void AddShaderReloadedCallback(const ShaderReloadedCallback &callback) = 0;
		HLAPI virtual const HLString &GetName() const = 0;
		HLAPI virtual HLRendererID GetRendererID() const = 0;

		HLAPI virtual const std::unordered_map<HLString, ShaderBuffer> &GetShaderBuffers() const = 0;
		HLAPI virtual const std::unordered_map<HLString, ShaderResourceDeclaration> &GetResources() const = 0;

		HLAPI static Ref<Shader> Create(const FileSystemPath &filePath, bool forceCompile = false);
		HLAPI static Ref<Shader> CreateFromString(const HLString &source);
	};
}

