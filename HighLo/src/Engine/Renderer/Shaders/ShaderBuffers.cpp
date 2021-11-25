#include "HighLoPch.h"
#include "ShaderBuffers.h"

namespace highlo
{
	ShaderUniform::ShaderUniform(const HLString &name, ShaderUniformType type, uint32 size, uint32 offset)
		: m_Name(name), m_Type(type), m_Size(size), m_Offset(offset)
	{
	}
}

