// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

namespace highlo
{
	enum class ShaderDomain
	{
		None = 0,
		Vertex = 1,
		Pixel = 2,
		Compute = 3,
		Geometry = 4,
		TessControl = 5,
		TessEvalulation = 6,
	};

	class ShaderResourceDeclaration
	{
	public:

		HLAPI ShaderResourceDeclaration() = default;
		HLAPI ShaderResourceDeclaration(const HLString &name, uint32 resourceRegister, uint32 count);

		HLAPI virtual const HLString &GetName() const { return m_Name; }
		HLAPI virtual uint32 GetRegister() const { return m_Register; }
		HLAPI virtual uint32 GetCount() const { return m_Count; }

	private:

		HLString m_Name;
		uint32 m_Register = 0;
		uint32 m_Count = 0;
	};

	typedef std::vector<ShaderResourceDeclaration*> ShaderResourceList;
}

