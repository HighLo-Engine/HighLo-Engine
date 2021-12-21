// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

namespace highlo
{
	enum class ShaderUniformType
	{
		None = 0,
		Bool,
		Int, Uint,
		Float,
		Vec2, Vec3, Vec4,
		IVec2, IVec3, IVec4,
		Mat2, Mat3, Mat4,
	};

	class ShaderUniform
	{
	public:

		HLAPI ShaderUniform() = default;
		HLAPI ShaderUniform(const HLString &name, ShaderUniformType type, uint32 size, uint32 offset);

		HLAPI const HLString &GetName() const { return m_Name; }
		HLAPI ShaderUniformType GetType() const { return m_Type; }
		HLAPI uint32 GetSize() const { return m_Size; }
		HLAPI uint32 GetOffset() const { return m_Offset; }

	private:

		HLString m_Name;
		ShaderUniformType m_Type = ShaderUniformType::None;
		uint32 m_Size = 0;
		uint32 m_Offset = 0;
	};

	struct ShaderUniformBuffer
	{
		HLString Name;
		uint32 Index;
		uint32 BindingPoint;
		uint32 Size;
		uint32 RendererID;
		std::vector<ShaderUniform> Uniforms;
	};

	struct ShaderStorageBuffer
	{
		HLString Name;
		uint32 Index;
		uint32 BindingPoint;
		uint32 Size;
		uint32 RendererID;
	};

	struct ShaderBuffer
	{
		HLString Name;
		uint32 Size = 0;
		std::unordered_map<HLString, ShaderUniform> Uniforms;
	};
}

