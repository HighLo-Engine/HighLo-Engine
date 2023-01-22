// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderBuffers.h"

namespace highlo
{
	ShaderUniform::ShaderUniform(const HLString &name, uint32 binding, ShaderUniformType type, uint32 size, uint32 offset)
		: m_Name(name), m_Binding(binding), m_Type(type), m_Size(size), m_Offset(offset)
	{
	}
}

