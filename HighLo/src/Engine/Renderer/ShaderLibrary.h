#pragma once

#include "Shader.h"

namespace highlo
{
	class ShaderLibrary : public IsSharedReference
	{
	public:

		HLAPI ShaderLibrary();
		HLAPI ~ShaderLibrary();

		HLAPI void Add(const Ref<Shader> &shader);
		HLAPI void Load(const HLString &path, BufferLayout &layout = BufferLayout::Empty, bool isCompute = false);
		HLAPI void Load(const HLString &name, const HLString &path, BufferLayout &layout = BufferLayout::Empty, bool isCompute = false);
		
		HLAPI const Ref<Shader> &Get(const HLString &name);

	private:

		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}