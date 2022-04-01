// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Shader.h"
#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	class ShaderLibrary : public IsSharedReference
	{
	public:

		HLAPI ShaderLibrary();
		HLAPI ~ShaderLibrary();

		HLAPI void Add(const Ref<Shader> &shader);
		HLAPI void Add(const HLString &name, const Ref<Shader> &shader);
		HLAPI void Load(const FileSystemPath &filePath, bool forceCompile = false);
		HLAPI void Load(const HLString &name, const FileSystemPath &filePath, bool forceCompile = false);
		
		HLAPI const Ref<Shader> &Get(const HLString &name);

	private:

		std::unordered_map<HLString, Ref<Shader>> m_Shaders;
	};
}
