#include "HighLoPch.h"
#include "ShaderUniform.h"

namespace highlo
{
	ShaderResourceDeclaration::ShaderResourceDeclaration(const HLString &name, uint32 resourceRegister, uint32 count)
		: m_Name(name), m_Register(resourceRegister), m_Count(count)
	{
	}
}

