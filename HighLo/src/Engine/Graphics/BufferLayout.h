// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-29) Added GetTextLayout
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}

	enum class BufferElementInputClass
	{
		INPUT_PER_VERTEX_DATA,
		INPUT_PER_INSTANCE_DATA
	};

	struct BufferElement
	{
		HLString Name;
		ShaderDataType Type = ShaderDataType::Float;
		uint32 Size = 0;
		uint32 Offset = 0;
		bool Normalized = false;
		uint32 InputSlot = 0;
		BufferElementInputClass ElementInputClass = BufferElementInputClass::INPUT_PER_VERTEX_DATA;

		BufferElement() {}

		BufferElement(const HLString& name, ShaderDataType type, bool normalized = false, uint32 input_slot = 0, BufferElementInputClass classification = BufferElementInputClass::INPUT_PER_VERTEX_DATA)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized), InputSlot(input_slot), ElementInputClass(classification)
		{
		}

		uint32 GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}
	};

	class BufferLayout
	{
	public:

		static BufferLayout Empty;

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement> &elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		HLAPI inline uint32 GetStride() const { return m_Stride; }
		HLAPI inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		HLAPI inline uint32 GetElementCount() const { return (uint32)m_Elements.size(); }


		HLAPI std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		HLAPI std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		HLAPI std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		HLAPI std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		HLAPI static BufferLayout GetTextureLayout();
		HLAPI static BufferLayout GetLineLayout();
		HLAPI static BufferLayout GetCircleLayout();
		HLAPI static BufferLayout GetTextLayout();
		HLAPI static BufferLayout GetStaticShaderLayout();
		HLAPI static BufferLayout GetAnimatedShaderLayout();
		HLAPI static BufferLayout GetSkyboxLayout();
		HLAPI static BufferLayout GetGridLayout();
		HLAPI static BufferLayout GetCompositeLayout();

		HLAPI static BufferLayout GetTransformBufferLayout();

	private:
		void CalculateOffsetsAndStride();

		std::vector<BufferElement> m_Elements;
		uint32 m_Stride = 0;
	};
}
